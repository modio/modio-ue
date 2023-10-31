/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioSubsystem.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Internal/Convert/AuthParams.h"
#include "Internal/Convert/CreateModFileParams.h"
#include "Internal/Convert/CreateModParams.h"
#include "Internal/Convert/EditModParams.h"
#include "Internal/Convert/ErrorCode.h"
#include "Internal/Convert/FilterParams.h"
#include "Internal/Convert/GameInfo.h"
#include "Internal/Convert/GamePlatform.h"
#include "Internal/Convert/InitializeOptions.h"
#include "Internal/Convert/List.h"
#include "Internal/Convert/ModCollectionEntry.h"
#include "Internal/Convert/ModCreationHandle.h"
#include "Internal/Convert/ModDependency.h"
#include "Internal/Convert/ModInfo.h"
#include "Internal/Convert/ModInfoList.h"
#include "Internal/Convert/ModManagementEvent.h"
#include "Internal/Convert/ModProgressInfo.h"
#include "Internal/Convert/ModTagInfo.h"
#include "Internal/Convert/ModTagOptions.h"
#include "Internal/Convert/Rating.h"
#include "Internal/Convert/ReportParams.h"
#include "Internal/Convert/Terms.h"
#include "Internal/Convert/TransactionRecord.h"
#include "Internal/Convert/User.h"
#include "Internal/Convert/UserList.h"
#include "Internal/ModioConvert.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioSettings.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include <map>

template<typename DestKey, typename DestValue, typename SourceKey, typename SourceValue>
TMap<DestKey, DestValue> ToUnreal(std::map<SourceKey, SourceValue>&& OriginalMap);

template<typename Dest, typename Source>
TOptional<Dest> ToUnrealOptional(Source Original);

template<typename Dest, typename Source>
Dest ToBP(Source Original);

FModioBackgroundThread::FModioBackgroundThread(UModioSubsystem* modio)
{
	CurrentModio = modio;
	CurrentRunningThread =
		FRunnableThread::Create(this, TEXT("ModioBackgroundThread"), 0U, EThreadPriority::TPri_Normal);
}

FModioBackgroundThread::~FModioBackgroundThread()
{
	CurrentRunningThread->Kill();
	delete CurrentRunningThread;
	CurrentRunningThread = NULL;
}

bool FModioBackgroundThread::Init()
{
	bStopThread = false;
	return true;
}

uint32 FModioBackgroundThread::Run()
{
	while (!bStopThread)
	{
		if (!CurrentModio)
		{
			FPlatformProcess::Sleep(0.001f);
			continue;
		}

		CurrentModio->RunPendingHandlers();
		FPlatformProcess::Sleep(0.001f);
	}

	UE_LOG(LogModio, Warning, TEXT("MODIO THREAD EXITED"));

	return 0;
}

void FModioBackgroundThread::Stop()
{
	bStopThread = true;
}

void FModioBackgroundThread::Exit()
{
	delete this;
}

void FModioBackgroundThread::EndThread()
{
	Stop();
	CurrentRunningThread->Kill();
	CurrentRunningThread->WaitForCompletion();
}

void UModioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UModioSettings* Settings = GetMutableDefault<UModioSettings>())
	{
		SetLogLevel(Settings->LogLevel);
		UE_LOG(LogModio, Display, TEXT("Setting log level to %d"), (int32) Settings->LogLevel);
	}
	ImageCache = MakeUnique<FModioImageCache>();

	LanguageMap.Add(TEXT("en"), EModioLanguage::English);
	LanguageMap.Add(TEXT("bg"), EModioLanguage::Bulgarian);
	LanguageMap.Add(TEXT("fr"), EModioLanguage::French);
	LanguageMap.Add(TEXT("de"), EModioLanguage::German);
	LanguageMap.Add(TEXT("it"), EModioLanguage::Italian);
	LanguageMap.Add(TEXT("pl"), EModioLanguage::Polish);
	LanguageMap.Add(TEXT("pt"), EModioLanguage::Portuguese);
	LanguageMap.Add(TEXT("hu"), EModioLanguage::Hungarian);
	LanguageMap.Add(TEXT("ja"), EModioLanguage::Japanese);
	LanguageMap.Add(TEXT("ko"), EModioLanguage::Korean);
	LanguageMap.Add(TEXT("ru"), EModioLanguage::Russian);
	LanguageMap.Add(TEXT("es"), EModioLanguage::Spanish);
	LanguageMap.Add(TEXT("th"), EModioLanguage::Thai);
	LanguageMap.Add(TEXT("zh"), EModioLanguage::ChineseSimplified);
}

void UModioSubsystem::Deinitialize()
{
	FModioImageCache* Cache = ImageCache.Release();

	Super::Deinitialize();
}

bool UModioSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// @todo: Add hooks here where the user can decide where the subsystem is valid
	return true;
}

#if WITH_EDITOR
void UModioSubsystem::CheckShutdownBeforePIEClose(UWorld*)
{
	if (bInitializedDuringPIE)
	{
		UE_LOG(LogModio, Warning,
			   TEXT("mod.io plugin initialized during PIE, but not shut down. Please call "
					"UModioSubsystem::ShutdownAsync, ensuring you call RunPendingHandlers until it completes (invokes "
					"your callback) before exiting PIE to ensure cleanup occurs"));
	}

	bool bShutdownComplete = false;
	ShutdownAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([&bShutdownComplete](FModioErrorCode ec) { bShutdownComplete = true; }));
	while (!bShutdownComplete && !bUseBackgroundThread)
	{
		RunPendingHandlers();
	}
}
#endif

void UModioSubsystem::InitializeAsync(const FModioInitializeOptions& Options, FOnErrorOnlyDelegateFast OnInitComplete)
{
	bUseBackgroundThread = Options.bUseBackgroundThread;

	if (bUseBackgroundThread && !FPlatformProcess::SupportsMultithreading())
	{
		UE_LOG(LogModio, Warning, TEXT("PlatformProcess does not support multithreading"));
		bUseBackgroundThread = false;
	}

	if (bUseBackgroundThread && !BackgroundThread)
	{
		BackgroundThread = new FModioBackgroundThread(this);
	}

#if WITH_EDITOR
	if (const UModioSettings* Settings = GetMutableDefault<UModioSettings>())
	{
		UE_LOG(LogModio, Display, TEXT("Setting log level to %d"), (int32) Settings->LogLevel);
		SetLogLevel(Settings->LogLevel);
	}

	if (UWorld* CurrentWorld = GEngine->GetCurrentPlayWorld())
	{
		if (CurrentWorld->IsPlayInEditor())
		{
			bInitializedDuringPIE = true;
			FWorldDelegates::OnPreWorldFinishDestroy.AddUObject(this, &UModioSubsystem::CheckShutdownBeforePIEClose);
		}
	}
#endif

	Modio::InitializeAsync(ToModio(Options),
						   [WeakThis = TWeakObjectPtr<UModioSubsystem>(this), OnInitComplete](Modio::ErrorCode ec) {
							   if (!WeakThis.IsValid())
							   {
								   return;
							   }
							   AsyncTask(ENamedThreads::GameThread, ([WeakThis, OnInitComplete, ec]() {
											 if (WeakThis.IsValid())
											 {
												 TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
												 WeakThis->InvalidateUserSubscriptionCache();
												 OnInitComplete.ExecuteIfBound(ToUnreal(ec));
											 }
										 }));
						   });
}

void UModioSubsystem::K2_InitializeAsync(const FModioInitializeOptions& InitializeOptions,
										 FOnErrorOnlyDelegate InitCallback)
{
	InitializeAsync(InitializeOptions, FOnErrorOnlyDelegateFast::CreateLambda(
										   [InitCallback](FModioErrorCode ec) { InitCallback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegateFast Callback)
{
	Modio::ListAllModsAsync(ToModio(Filter),
							[Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModInfoList> Result) {
								AsyncTask(ENamedThreads::GameThread, ([Callback, ec, Result]() {
											  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
											  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModInfoList>(Result));
										  }));
							});
}

void UModioSubsystem::SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegateFast OnSubscribeComplete)
{
	Modio::SubscribeToModAsync(ToModio(ModToSubscribeTo), [WeakThis = TWeakObjectPtr<UModioSubsystem>(this),
														   OnSubscribeComplete](Modio::ErrorCode ec) {
		if (!WeakThis.IsValid())
		{
			return;
		}
		AsyncTask(ENamedThreads::GameThread, ([WeakThis, OnSubscribeComplete, ec]() {
					  if (WeakThis.IsValid())
					  {
						  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
						  WeakThis->InvalidateUserSubscriptionCache();
						  OnSubscribeComplete.ExecuteIfBound(ToUnreal(ec));
					  }
				  }));
	});
}

void UModioSubsystem::UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
											  FOnErrorOnlyDelegateFast OnUnsubscribeComplete)
{
	Modio::UnsubscribeFromModAsync(ToModio(ModToUnsubscribeFrom), [WeakThis = TWeakObjectPtr<UModioSubsystem>(this),
																   OnUnsubscribeComplete](Modio::ErrorCode ec) {
		if (!WeakThis.IsValid())
		{
			return;
		}
		AsyncTask(ENamedThreads::GameThread, ([WeakThis, OnUnsubscribeComplete, ec]() {
					  if (WeakThis.IsValid())
					  {
						  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
						  WeakThis->InvalidateUserSubscriptionCache();
						  OnUnsubscribeComplete.ExecuteIfBound(ToUnreal(ec));
					  }
				  }));
	});
}

void UModioSubsystem::FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast OnFetchDone)
{
	Modio::FetchExternalUpdatesAsync(
		[WeakThis = TWeakObjectPtr<UModioSubsystem>(this), OnFetchDone](Modio::ErrorCode ec) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			AsyncTask(ENamedThreads::GameThread, ([WeakThis, OnFetchDone, ec]() {
						  if (WeakThis.IsValid())
						  {
							  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
							  WeakThis->InvalidateUserSubscriptionCache();
							  OnFetchDone.ExecuteIfBound(ToUnreal(ec));
						  }
					  }));
		});
}

FModioErrorCode UModioSubsystem::EnableModManagement(FOnModManagementDelegateFast Callback)
{
	return Modio::EnableModManagement(
		[WeakThis = TWeakObjectPtr<UModioSubsystem>(this), Callback](Modio::ModManagementEvent Event) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			AsyncTask(ENamedThreads::GameThread, ([WeakThis, Event, Callback]() {
						  if (WeakThis.IsValid())
						  {
							  // @todo: For some smarter caching, look at the event and see if we should invalidate the
							  // cache
							  WeakThis->InvalidateUserInstallationCache();
							  Callback.ExecuteIfBound(ToUnreal(Event));
						  }
					  }));
		});
}

void UModioSubsystem::K2_ListAllModsAsync(const FModioFilterParams& Filter, FOnListAllModsDelegate Callback)
{
	ListAllModsAsync(Filter, FOnListAllModsDelegateFast::CreateLambda(
								 [Callback](FModioErrorCode ec, TOptional<FModioModInfoList> ModList) {
									 Callback.ExecuteIfBound(ec, ToBP<FModioOptionalModInfoList>(ModList));
								 }));
}

void UModioSubsystem::K2_SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegate OnSubscribeComplete)
{
	SubscribeToModAsync(ModToSubscribeTo,
						FOnErrorOnlyDelegateFast::CreateLambda(
							[OnSubscribeComplete](FModioErrorCode ec) { OnSubscribeComplete.ExecuteIfBound(ec); }));
}

void UModioSubsystem::K2_UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
												 FOnErrorOnlyDelegate OnUnsubscribeComplete)
{
	UnsubscribeFromModAsync(ModToUnsubscribeFrom,
							FOnErrorOnlyDelegateFast::CreateLambda([OnUnsubscribeComplete](FModioErrorCode ec) {
								OnUnsubscribeComplete.ExecuteIfBound(ec);
							}));
}

void UModioSubsystem::K2_FetchExternalUpdatesAsync(FOnErrorOnlyDelegate OnFetchDone)
{
	FetchExternalUpdatesAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([OnFetchDone](FModioErrorCode ec) { OnFetchDone.ExecuteIfBound(ec); }));
}

FModioErrorCode UModioSubsystem::K2_EnableModManagement(FOnModManagementDelegate Callback)
{
	return EnableModManagement(FOnModManagementDelegateFast::CreateLambda(
		[Callback](FModioModManagementEvent Event) { Callback.ExecuteIfBound(Event); }));
}

void UModioSubsystem::DisableModManagement()
{
	Modio::DisableModManagement();
}

void UModioSubsystem::KillBackgroundThread()
{
#if WITH_EDITOR
	return; // thread is not being destoyed between PIE sessions
#endif

	if (bUseBackgroundThread && BackgroundThread)
	{
		BackgroundThread->Stop();
		BackgroundThread->EndThread();
		delete BackgroundThread;
		BackgroundThread = NULL;
	}
}

void UModioSubsystem::ShutdownAsync(FOnErrorOnlyDelegateFast OnShutdownComplete)
{
	Modio::ShutdownAsync([WeakThis = TWeakObjectPtr<UModioSubsystem>(this), OnShutdownComplete](Modio::ErrorCode ec) {
		if (!WeakThis.IsValid())
		{
			return;
		}

#if WITH_EDITOR
		WeakThis->bInitializedDuringPIE = false;
#endif
		WeakThis->KillBackgroundThread();

		if (WeakThis->bUseBackgroundThread)
		{
			AsyncTask(ENamedThreads::GameThread, ([WeakThis, ec, OnShutdownComplete]() {
						  if (WeakThis.IsValid())
						  {
							  WeakThis->InvalidateUserSubscriptionCache();
							  OnShutdownComplete.ExecuteIfBound(ToUnreal(ec));
						  }
					  }));
		}
		else
		{
			WeakThis->InvalidateUserSubscriptionCache();
			OnShutdownComplete.ExecuteIfBound(ToUnreal(ec));
		}
	});
}

void UModioSubsystem::K2_ShutdownAsync(FOnErrorOnlyDelegate OnShutdownComplete)
{
	ShutdownAsync(FOnErrorOnlyDelegateFast::CreateLambda(
		[OnShutdownComplete](FModioErrorCode ec) { OnShutdownComplete.ExecuteIfBound(ec); }));
}

void UModioSubsystem::RunPendingHandlers()
{
	Modio::RunPendingHandlers();
}

void UModioSubsystem::SetLogLevel(EModioLogLevel UnrealLogLevel)
{
	Modio::SetLogLevel(ToModio(UnrealLogLevel));
}

bool UModioSubsystem::IsModManagementBusy()
{
	return Modio::IsModManagementBusy();
}

TOptional<FModioModProgressInfo> UModioSubsystem::QueryCurrentModUpdate()
{
	return ToUnrealOptional<FModioModProgressInfo>(Modio::QueryCurrentModUpdate());
}

FModioOptionalModProgressInfo UModioSubsystem::K2_QueryCurrentModUpdate()
{
	return ToBP<FModioOptionalModProgressInfo>(QueryCurrentModUpdate());
}

const TMap<FModioModID, FModioModCollectionEntry>& UModioSubsystem::QueryUserSubscriptions()
{
	if (!CachedUserSubscriptions || bUseBackgroundThread)
	{
		CachedUserSubscriptions = ToUnreal<FModioModID, FModioModCollectionEntry>(Modio::QueryUserSubscriptions());
	}
	return CachedUserSubscriptions.GetValue();
}

const TMap<FModioModID, FModioModCollectionEntry>& UModioSubsystem::QueryUserInstallations(bool bIncludeOutdatedMods)
{
	TOptional<TMap<FModioModID, FModioModCollectionEntry>>& UserInstallation =
		bIncludeOutdatedMods ? CachedUserInstallationWithOutdatedMods : CachedUserInstallationWithoutOutdatedMods;

	if (!UserInstallation)
	{
		UserInstallation =
			ToUnreal<FModioModID, FModioModCollectionEntry>(Modio::QueryUserInstallations(bIncludeOutdatedMods));
	}
	return UserInstallation.GetValue();
}

TOptional<FModioUser> UModioSubsystem::QueryUserProfile()
{
	return ToUnrealOptional<FModioUser>(Modio::QueryUserProfile());
}

FModioOptionalUser UModioSubsystem::K2_QueryUserProfile()
{
	return ToBP<FModioOptionalUser>(QueryUserProfile());
}

void UModioSubsystem::GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegateFast Callback)
{
	Modio::GetModInfoAsync(ToModio(ModId), [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModInfo> ModInfo) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec, ModInfo]() {
					  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
					  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModInfo>(ModInfo));
				  }));
	});
}

void UModioSubsystem::K2_GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegate Callback)
{
	GetModInfoAsync(ModId, FOnGetModInfoDelegateFast::CreateLambda(
							   [Callback](FModioErrorCode ec, TOptional<FModioModInfo> ModInfo) {
								   Callback.ExecuteIfBound(ec, ToBP<FModioOptionalModInfo>(ModInfo));
							   }));
}

void UModioSubsystem::GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegateFast Callback)
{
	Modio::GetGameInfoAsync(ToModio(GameID),
							[Callback](Modio::ErrorCode ec, Modio::Optional<Modio::GameInfo> GameInfo) {
								AsyncTask(ENamedThreads::GameThread, ([Callback, ec, GameInfo]() {
											  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
											  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioGameInfo>(GameInfo));
										  }));
							});
}

void UModioSubsystem::K2_GetGameInfoAsync(FModioGameID GameID, FOnGetGameInfoDelegate Callback)
{
	GetGameInfoAsync(GameID, FOnGetGameInfoDelegateFast::CreateLambda(
								 [Callback](FModioErrorCode ec, TOptional<FModioGameInfo> GameInfo) {
									 Callback.ExecuteIfBound(ec, ToBP<FModioOptionalGameInfo>(GameInfo));
								 }));
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback)
{
	const TTuple<FModioModID, EModioAvatarSize> ModIdAndSizePair = MakeTuple(ModId, AvatarSize);
	if (FOnGetMediaMulticastDelegateFast* MulticastCallbackPtr = PendingModMediaAvatarRequests.Find(ModIdAndSizePair))
	{
		MulticastCallbackPtr->Add(Callback);
		return;
	}
	PendingModMediaAvatarRequests.Add(ModIdAndSizePair).Add(Callback);
	Modio::GetModMediaAsync(
		ToModio(ModId), ToModio(AvatarSize),
		[WeakThis = TWeakObjectPtr<UModioSubsystem>(this), ModIdAndSizePair](Modio::ErrorCode ec,
																			 Modio::Optional<std::string> Path) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			// Manually calling ToUnreal on the path and assigning to the member of FModioImage
			// because we already have a std::string -> FString overload of ToUnreal
			// TODO: @modio-ue4 Potentially refactor ToUnreal(std::string) as a template
			// function returning type T so we can be explicit about the expected type
			if (Path)
			{
				FModioImageWrapper Out;
				Out.ImagePath = ToUnreal(Path.value());
				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec, Out]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaAvatarRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, Out);
							  }
						  }));
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaAvatarRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, {});
							  }
						  }));
			}
		});
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
									   FOnGetMediaDelegateFast Callback)
{
	const TTuple<FModioModID, EModioGallerySize> ModIdAndSizePair = MakeTuple(ModId, GallerySize);
	if (FOnGetMediaMulticastDelegateFast* MulticastCallbackPtr = PendingModMediaGalleryRequests.Find(ModIdAndSizePair))
	{
		MulticastCallbackPtr->Add(Callback);
		return;
	}
	PendingModMediaGalleryRequests.Add(ModIdAndSizePair).Add(Callback);
	Modio::GetModMediaAsync(
		ToModio(ModId), ToModio(GallerySize), Index,
		[WeakThis = TWeakObjectPtr<UModioSubsystem>(this), ModIdAndSizePair](Modio::ErrorCode ec,
																			 Modio::Optional<std::string> Path) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			// Manually calling ToUnreal on the path and assigning to the member of FModioImage
			// because we already have a std::string -> FString overload of ToUnreal
			// TODO: @modio-ue4 Potentially refactor ToUnreal(std::string) as a template
			// function returning type T so we can be explicit about the expected type
			if (Path)
			{
				FModioImageWrapper Out;
				Out.ImagePath = ToUnreal(Path.value());

				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec, Out]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaGalleryRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, Out);
							  }
						  }));
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaGalleryRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, {});
							  }
						  }));
			}
		});
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegateFast Callback)
{
	const TTuple<FModioModID, EModioLogoSize> ModIdAndSizePair = MakeTuple(ModId, LogoSize);
	if (FOnGetMediaMulticastDelegateFast* MulticastCallbackPtr = PendingModMediaLogoRequests.Find(ModIdAndSizePair))
	{
		MulticastCallbackPtr->Add(Callback);
		return;
	}
	PendingModMediaLogoRequests.Add(ModIdAndSizePair).Add(Callback);
	Modio::GetModMediaAsync(
		ToModio(ModId), ToModio(LogoSize),
		[WeakThis = TWeakObjectPtr<UModioSubsystem>(this), ModIdAndSizePair](Modio::ErrorCode ec,
																			 Modio::Optional<std::string> Path) {
			if (!WeakThis.IsValid())
			{
				return;
			}
			// Manually calling ToUnreal on the path and assigning to the member of FModioImage
			// because we already have a std::string -> FString overload of ToUnreal
			// TODO: @modio-ue4 Potentially refactor ToUnreal(std::string) as a template
			// function returning type T so we can be explicit about the expected type
			if (Path)
			{
				FModioImageWrapper Out;
				Out.ImagePath = ToUnreal(Path.value());

				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec, Out]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaLogoRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, Out);
							  }
						  }));
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, ([WeakThis, ModIdAndSizePair, ec]() {
							  if (WeakThis.IsValid())
							  {
								  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
								  WeakThis->PendingModMediaLogoRequests.FindAndRemoveChecked(ModIdAndSizePair)
									  .Broadcast(ec, {});
							  }
						  }));
			}
		});
}

void UModioSubsystem::K2_GetModMediaAvatarAsync(FModioModID ModId, EModioAvatarSize AvatarSize,
												FOnGetMediaDelegate Callback)
{
	GetModMediaAsync(
		ModId, AvatarSize,
		FOnGetMediaDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioImageWrapper> Media) {
			Callback.ExecuteIfBound(ec, ToBP<FModioOptionalImage>(Media));
		}));
}

void UModioSubsystem::K2_GetModMediaGalleryImageAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
													  FOnGetMediaDelegate Callback)
{
	GetModMediaAsync(
		ModId, GallerySize, Index,
		FOnGetMediaDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioImageWrapper> Media) {
			Callback.ExecuteIfBound(ec, ToBP<FModioOptionalImage>(Media));
		}));
}

void UModioSubsystem::K2_GetModMediaLogoAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegate Callback)
{
	GetModMediaAsync(
		ModId, LogoSize,
		FOnGetMediaDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioImageWrapper> Media) {
			Callback.ExecuteIfBound(ec, ToBP<FModioOptionalImage>(Media));
		}));
}

void UModioSubsystem::GetModTagOptionsAsync(FOnGetModTagOptionsDelegateFast Callback)
{
	if (CachedModTags)
	{
		TOptional<FModioModTagOptions> cachedModTags = CachedModTags;
		AsyncTask(ENamedThreads::GameThread,
				  ([Callback, cachedModTags]() { Callback.ExecuteIfBound({}, cachedModTags); }));
		return;
	}
	// TODO: @modio-UE4 capturing `this` is bad and we shouldn't do it. Better to store the cached tags as a TSharedPtr
	// and capture that by value, so we are guaranteed lifetime
	Modio::GetModTagOptionsAsync([WeakThis = TWeakObjectPtr<UModioSubsystem>(this),
								  Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModTagOptions> ModTagOptions) {
		if (!WeakThis.IsValid())
		{
			return;
		}
		WeakThis->CachedModTags = ToUnrealOptional<FModioModTagOptions>(ModTagOptions);

		AsyncTask(ENamedThreads::GameThread, ([Callback, ec, ModTagOptions]() {
					  TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("Callback"));
					  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModTagOptions>(ModTagOptions));
				  }));
	});
}

void UModioSubsystem::K2_GetModTagOptionsAsync(FOnGetModTagOptionsDelegate Callback)
{
	GetModTagOptionsAsync(FOnGetModTagOptionsDelegateFast::CreateLambda(
		[Callback](FModioErrorCode ec, TOptional<FModioModTagOptions> ModTagOptions) {
			Callback.ExecuteIfBound(ec, ToBP<FModioOptionalModTagOptions>(ModTagOptions));
		}));
}

void UModioSubsystem::SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
										FOnSubmitNewModDelegateFast Callback)
{
	Modio::SubmitNewModAsync(ToModio(Handle), ToModio(Params),
							 [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModID> NewModID) {
								 AsyncTask(ENamedThreads::GameThread, ([Callback, ec, NewModID]() {
											   Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModID>(NewModID));
										   }));
							 });
}

void UModioSubsystem::RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress,
												FOnErrorOnlyDelegateFast Callback)
{
	Modio::RequestEmailAuthCodeAsync(ToModio(EmailAddress), [Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::K2_RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress,
												   FOnErrorOnlyDelegate Callback)
{
	RequestEmailAuthCodeAsync(EmailAddress, FOnErrorOnlyDelegateFast::CreateLambda(
												[Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
												 FOnErrorOnlyDelegateFast Callback)
{
	Modio::AuthenticateUserEmailAsync(ToModio(AuthenticationCode), [Callback](FModioErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec); }));
	});
}

void UModioSubsystem::K2_AuthenticateUserEmailAsync(const FModioEmailAuthCode& AuthenticationCode,
													FOnErrorOnlyDelegate Callback)
{
	AuthenticateUserEmailAsync(
		AuthenticationCode,
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													EModioAuthenticationProvider Provider,
													FOnErrorOnlyDelegateFast Callback)
{
	Modio::AuthenticateUserExternalAsync(ToModio(User), ToModio(Provider), [Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec); }));
	});
}

void UModioSubsystem::K2_AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													   EModioAuthenticationProvider Provider,
													   FOnErrorOnlyDelegate Callback)
{
	AuthenticateUserExternalAsync(
		User, Provider,
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}
// Deprecated
void UModioSubsystem::GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
										 FOnGetTermsOfUseDelegateFast Callback)
{
	GetTermsOfUseAsync(Locale, Callback);
}

void UModioSubsystem::GetTermsOfUseAsync(EModioLanguage Locale, FOnGetTermsOfUseDelegateFast Callback)
{
	Modio::GetTermsOfUseAsync(ToModio(Locale), [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::Terms> Terms) {
		AsyncTask(ENamedThreads::GameThread,
				  ([Callback, ec, Terms]() { Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioTerms>(Terms)); }));
	});
}
// Deprecated
void UModioSubsystem::K2_GetTermsOfUseAsync_DEPRECATED(EModioAuthenticationProvider Provider, EModioLanguage Locale,
													   FOnGetTermsOfUseDelegate Callback)
{
	K2_GetTermsOfUseAsync(Locale, Callback);
}

void UModioSubsystem::K2_GetTermsOfUseAsync(EModioLanguage Locale, FOnGetTermsOfUseDelegate Callback)
{
	GetTermsOfUseAsync(Locale, FOnGetTermsOfUseDelegateFast::CreateLambda(
								   [Callback](FModioErrorCode ec, TOptional<FModioTerms> Terms) {
									   Callback.ExecuteIfBound(ec, ToBP<FModioOptionalTerms>(Terms));
								   }));
}

void UModioSubsystem::ClearUserDataAsync(FOnErrorOnlyDelegateFast Callback)
{
	Modio::ClearUserDataAsync([Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::K2_ClearUserDataAsync(FOnErrorOnlyDelegate Callback)
{
	ClearUserDataAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::GetUserMediaAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback)
{
	if (FOnGetMediaMulticastDelegateFast* MulticastCallbackPtr = PendingUserMediaRequests.Find(AvatarSize))
	{
		MulticastCallbackPtr->Add(Callback);
		return;
	}
	PendingUserMediaRequests.Add(AvatarSize).Add(Callback);
	Modio::GetUserMediaAsync(ToModio(AvatarSize), [WeakThis = TWeakObjectPtr<UModioSubsystem>(this), AvatarSize](
													  Modio::ErrorCode ec, Modio::Optional<std::string> Media) {
		// Manually calling ToUnreal on the path and assigning to the member of FModioImage
		// because we already have a std::string -> FString overload of ToUnreal
		// TODO: @modio-ue4 Potentially refactor ToUnreal(std::string) as a
		// template function returning type T so we can be explicit about the expected type
		if (!WeakThis.IsValid())
		{
			return;
		}

		if (Media)
		{
			FModioImageWrapper Out;
			Out.ImagePath = ToUnreal(Media.value());

			AsyncTask(ENamedThreads::GameThread, ([WeakThis, AvatarSize, ec, Out]() {
						  if (WeakThis.IsValid())
							  WeakThis->PendingUserMediaRequests.FindAndRemoveChecked(AvatarSize).Broadcast(ec, Out);
					  }));
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, ([WeakThis, AvatarSize, ec]() {
						  if (WeakThis.IsValid())
							  WeakThis->PendingUserMediaRequests.FindAndRemoveChecked(AvatarSize).Broadcast(ec, {});
					  }));
		}
	});
}

void UModioSubsystem::K2_GetUserMediaAvatarAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegate Callback)
{
	GetUserMediaAsync(AvatarSize, FOnGetMediaDelegateFast::CreateLambda(
									  [Callback](FModioErrorCode ec, TOptional<FModioImageWrapper> ModioMedia) {
										  Callback.ExecuteIfBound(ec, ToBP<FModioOptionalImage>(ModioMedia));
									  }));
}

void UModioSubsystem::InvalidateUserSubscriptionCache()
{
	CachedUserSubscriptions.Reset();
}

void UModioSubsystem::InvalidateUserInstallationCache()
{
	CachedUserInstallationWithOutdatedMods.Reset();
	CachedUserInstallationWithoutOutdatedMods.Reset();
}

FModioImageCache& UModioSubsystem::GetImageCache() const
{
	return *ImageCache;
}

TArray<FModioValidationError> UModioSubsystem::GetLastValidationError()
{
	TArray<FModioValidationError> Errors;
	for (const auto& Error : Modio::GetLastValidationError())
	{
		Errors.Add(FModioValidationError {ToUnreal(Error.Field), ToUnreal(Error.Error)});
	}
	return Errors;
}

TMap<FModioModID, FModioModCollectionEntry> UModioSubsystem::QuerySystemInstallations()
{
	return ToUnreal<FModioModID, FModioModCollectionEntry>(Modio::QuerySystemInstallations());
}

void UModioSubsystem::ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegateFast Callback)
{
	Modio::ForceUninstallModAsync(ToModio(ModToRemove), [Callback](FModioErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec); }));
	});
}

void UModioSubsystem::K2_ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegate Callback)
{
	ForceUninstallModAsync(ModToRemove, FOnErrorOnlyDelegateFast::CreateLambda(
											[Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegateFast Callback)
{
	Modio::SubmitModRatingAsync(ToModio(Mod), ToModio(Rating), [Callback](FModioErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec); }));
	});
}

void UModioSubsystem::K2_SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegate Callback)
{
	SubmitModRatingAsync(Mod, Rating, FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) {
							 Callback.ExecuteIfBound(ec);
						 }));
}

void UModioSubsystem::ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegateFast Callback)
{
	Modio::ReportContentAsync(ToModio(Report), [Callback](FModioErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec); }));
	});
}

void UModioSubsystem::K2_ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegate Callback)
{
	ReportContentAsync(Report, FOnErrorOnlyDelegateFast::CreateLambda(
								   [Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

TArray<FModioModDependency> ToUnreal(const std::vector<Modio::ModDependency>& OriginalArray)
{
	TArray<FModioModDependency> Result;

	Result.Reserve(OriginalArray.size());
	for (const auto& It : OriginalArray)
	{
		Result.Emplace(ToUnreal(It));
	}

	return Result;
}

void UModioSubsystem::GetModDependenciesAsync(FModioModID ModID, FOnGetModDependenciesDelegateFast Callback)
{
	Modio::GetModDependenciesAsync(
		ToModio(ModID), [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModDependencyList> Dependencies) {
			if (Dependencies)
			{
				FModioModDependencyList Out;
				Out.InternalList = ToUnreal(Dependencies->GetRawList());
				Out.PagedResult = FModioPagedResult(Dependencies.value());
				AsyncTask(ENamedThreads::GameThread, ([Callback, ec, Out]() { Callback.ExecuteIfBound(ec, Out); }));
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec, {}); }));
			}
		});
}

FModioModCreationHandle UModioSubsystem::GetModCreationHandle()
{
	return ToUnreal(Modio::GetModCreationHandle());
}

void UModioSubsystem::K2_GetModDependenciesAsync(FModioModID ModID, FOnGetModDependenciesDelegate Callback)
{
	GetModDependenciesAsync(ModID, FOnGetModDependenciesDelegateFast::CreateLambda(
									   [Callback](FModioErrorCode ec, TOptional<FModioModDependencyList> Dependencies) {
										   Callback.ExecuteIfBound(
											   ec, FModioOptionalModDependencyList(MoveTempIfPossible(Dependencies)));
									   }));
}

void UModioSubsystem::SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params)
{
	Modio::SubmitNewModFileForMod(ToModio(Mod), ToModio(Params));
}

void UModioSubsystem::SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
											FOnGetModInfoDelegateFast Callback)
{
	Modio::SubmitModChangesAsync(
		ToModio(Mod), ToModio(Params), [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModInfo> ModInfo) {
			AsyncTask(ENamedThreads::GameThread, ([Callback, ec, ModInfo]() {
						  Callback.ExecuteIfBound(ToUnreal(ec), ToUnrealOptional<FModioModInfo>(ModInfo));
					  }));
		});
}

void UModioSubsystem::K2_SubmitNewModFileForMod(FModioModID Mod, FModioCreateModFileParams Params)
{
	SubmitNewModFileForMod(Mod, Params);
}

FModioModCreationHandle UModioSubsystem::K2_GetModCreationHandle()
{
	return GetModCreationHandle();
}

void UModioSubsystem::K2_SubmitNewModAsync(FModioModCreationHandle Handle, FModioCreateModParams Params,
										   FOnSubmitNewModDelegate Callback)
{
	SubmitNewModAsync(
		Handle, Params,
		FOnSubmitNewModDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioModID> NewModID) {
			Callback.ExecuteIfBound(ec, FModioOptionalModID {NewModID});
		}));
}

void UModioSubsystem::K2_ListUserCreatedModsAsync(const FModioFilterParams& Filter,
												  FOnListUserCreatedModsDelegate Callback)
{
	ListUserCreatedModsAsync(Filter, FOnListUserCreatedModsDelegateFast::CreateLambda(
										 [Callback](FModioErrorCode ec, TOptional<FModioModInfoList> ModList) {
											 Callback.ExecuteIfBound(ec, ToBP<FModioOptionalModInfoList>(ModList));
										 }));
}

void UModioSubsystem::ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegateFast Callback)
{
	Modio::ArchiveModAsync(ToModio(Mod), [Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::K2_ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegate Callback)
{
	ArchiveModAsync(
		Mod, FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast Callback)
{
	Modio::VerifyUserAuthenticationAsync([Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::ListUserCreatedModsAsync(FModioFilterParams Filter, FOnListUserCreatedModsDelegateFast Callback)
{
	Modio::ListUserCreatedModsAsync(
		ToModio(Filter), [Callback](FModioErrorCode ec, Modio::Optional<Modio::ModInfoList> Result) {
			AsyncTask(ENamedThreads::GameThread, ([Callback, ec, Result]() {
						  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModInfoList>(Result));
					  }));
		});
}

void UModioSubsystem::K2_VerifyUserAuthenticationAsync(FOnErrorOnlyDelegate Callback)
{
	VerifyUserAuthenticationAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

FModioErrorCode UModioSubsystem::PrioritizeTransferForMod(FModioModID ModToPrioritize)
{
	return ToUnreal(Modio::PrioritizeTransferForMod(ToModio(ModToPrioritize)));
}

void UModioSubsystem::K2_SubmitModChangesAsync(FModioModID Mod, FModioEditModParams Params,
											   FOnGetModInfoDelegate Callback)
{
	SubmitModChangesAsync(
		Mod, Params,
		FOnGetModInfoDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioModInfo> MaybeModInfo) {
			Callback.ExecuteIfBound(ec, FModioOptionalModInfo {MaybeModInfo});
		}));
}

void UModioSubsystem::MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback)
{
	Modio::MuteUserAsync(ToModio(UserID), [Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::K2_MuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback)
{
	MuteUserAsync(UserID, FOnErrorOnlyDelegateFast::CreateLambda(
							  [Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegateFast Callback)
{
	Modio::UnmuteUserAsync(ToModio(UserID), [Callback](Modio::ErrorCode ec) {
		AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ToUnreal(ec)); }));
	});
}

void UModioSubsystem::K2_UnmuteUserAsync(FModioUserID UserID, FOnErrorOnlyDelegate Callback)
{
	UnmuteUserAsync(UserID, FOnErrorOnlyDelegateFast::CreateLambda(
								[Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::GetMutedUsersAsync(FOnMuteUsersDelegateFast Callback)
{
	Modio::GetMutedUsersAsync([Callback](Modio::ErrorCode ec, Modio::Optional<Modio::UserList> Dependencies) {
		if (Dependencies)
		{
			FModioUserList Out;
			Out.InternalList = ToUnreal(Dependencies->GetRawList());
			Out.PagedResult = FModioPagedResult(Dependencies.value());

			AsyncTask(ENamedThreads::GameThread, ([Callback, ec, Out]() { Callback.ExecuteIfBound(ec, Out); }));
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, ([Callback, ec]() { Callback.ExecuteIfBound(ec, {}); }));
		}
	});
}

void UModioSubsystem::K2_GetMutedUsersAsync(FOnMuteUsersDelegate Callback)
{
	GetMutedUsersAsync(
		FOnMuteUsersDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioUserList> Dependencies) {
			Callback.ExecuteIfBound(ec, FModioOptionalUserList(MoveTempIfPossible(Dependencies)));
		}));
}

bool UModioSubsystem::IsUsingBackgroundThread()
{
	return bUseBackgroundThread;
}

EModioLanguage UModioSubsystem::ConvertLanguageCodeToModio(FString LanguageCode)
{
	if (LanguageMap.Contains(LanguageCode))
	{
		return LanguageMap[LanguageCode];
	}
	else
	{
		UE_LOG(LogModio, Log, TEXT("Language code %s does not correspond to a valid EModioLanguage value"),
			   *LanguageCode);
		return EModioLanguage::English;
	}
}

/// File scope implementations

#pragma region Implementation
template<typename DestKey, typename DestValue, typename SourceKey, typename SourceValue>
TMap<DestKey, DestValue> ToUnreal(std::map<SourceKey, SourceValue>&& OriginalMap)
{
	TMap<DestKey, DestValue> Result;

	Result.Reserve(OriginalMap.size());
	for (auto& It : OriginalMap)
	{
		Result.Add(ToUnreal(It.first), ToUnreal(It.second));
	}

	return Result;
}

template<typename Dest, typename Source>
TOptional<Dest> ToUnrealOptional(Source Original)
{
	TOptional<Dest> DestinationOptional = {};
	if (Original)
	{
		DestinationOptional = ToUnreal(Original.value());
	}

	return DestinationOptional;
}

template<typename Dest, typename Source>
Dest ToBP(Source Original)
{
	Dest Result = {MoveTempIfPossible(Original)};
	return Result;
}

#pragma endregion
