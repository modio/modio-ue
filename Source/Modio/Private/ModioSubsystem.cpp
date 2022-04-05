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
#include "Engine/Engine.h"
#include "Internal/Convert/AuthParams.h"
#include "Internal/Convert/CreateModFileParams.h"
#include "Internal/Convert/CreateModParams.h"
#include "Internal/Convert/ErrorCode.h"
#include "Internal/Convert/FilterParams.h"
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
#include "Internal/Convert/User.h"
#include "Internal/ModioConvert.h"
#include "ModioSettings.h"
#include <map>

template<typename DestKey, typename DestValue, typename SourceKey, typename SourceValue>
TMap<DestKey, DestValue> ToUnreal(std::map<SourceKey, SourceValue>&& OriginalMap);

template<typename Dest, typename Source>
TOptional<Dest> ToUnrealOptional(Source Original);

template<typename Dest, typename Source>
Dest ToBP(Source Original);

void UModioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UModioSettings* Settings = GetDefault<UModioSettings>())
	{
		SetLogLevel(Settings->LogLevel);
	}
	ImageCache = MakeUnique<FModioImageCache>();
}

void UModioSubsystem::Deinitialize()
{
	ImageCache.Release();

	Super::Deinitialize();
}

bool UModioSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// @todo: Add hooks here where the user can decide where the subsystem is valid
	return true;
}

void UModioSubsystem::InitializeAsync(const FModioInitializeOptions& Options, FOnErrorOnlyDelegateFast OnInitComplete)
{
	Modio::InitializeAsync(ToModio(Options), [this, OnInitComplete](Modio::ErrorCode ec) {
		InvalidateUserSubscriptionCache();

		OnInitComplete.ExecuteIfBound(ToUnreal(ec));
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
								Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModInfoList>(Result));
							});
}

void UModioSubsystem::SubscribeToModAsync(FModioModID ModToSubscribeTo, FOnErrorOnlyDelegateFast OnSubscribeComplete)
{
	Modio::SubscribeToModAsync(ToModio(ModToSubscribeTo), [this, OnSubscribeComplete](Modio::ErrorCode ec) {
		InvalidateUserSubscriptionCache();

		OnSubscribeComplete.ExecuteIfBound(ToUnreal(ec));
	});
}

void UModioSubsystem::UnsubscribeFromModAsync(FModioModID ModToUnsubscribeFrom,
											  FOnErrorOnlyDelegateFast OnUnsubscribeComplete)
{
	Modio::UnsubscribeFromModAsync(ToModio(ModToUnsubscribeFrom), [this, OnUnsubscribeComplete](Modio::ErrorCode ec) {
		InvalidateUserSubscriptionCache();

		OnUnsubscribeComplete.ExecuteIfBound(ToUnreal(ec));
	});
}

void UModioSubsystem::FetchExternalUpdatesAsync(FOnErrorOnlyDelegateFast OnFetchDone)
{
	Modio::FetchExternalUpdatesAsync([this, OnFetchDone](Modio::ErrorCode ec) {
		InvalidateUserSubscriptionCache();
		OnFetchDone.ExecuteIfBound(ToUnreal(ec));
	});
}

void UModioSubsystem::EnableModManagement(FOnModManagementDelegateFast Callback)
{
	Modio::EnableModManagement([this, Callback](Modio::ModManagementEvent Event) {
		// @todo: For some smarter caching, look at the event and see if we should invalidate the cache
		InvalidateUserInstallationCache();
		Callback.ExecuteIfBound(ToUnreal(Event));
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

void UModioSubsystem::K2_EnableModManagement(FOnModManagementDelegate Callback)
{
	EnableModManagement(FOnModManagementDelegateFast::CreateLambda(
		[Callback](FModioModManagementEvent Event) { Callback.ExecuteIfBound(Event); }));
}

void UModioSubsystem::DisableModManagement()
{
	Modio::DisableModManagement();
}

void UModioSubsystem::ShutdownAsync(FOnErrorOnlyDelegateFast OnShutdownComplete)
{
	Modio::ShutdownAsync([this, OnShutdownComplete](Modio::ErrorCode ec) {
		InvalidateUserSubscriptionCache();

		OnShutdownComplete.ExecuteIfBound(ToUnreal(ec));
	});
}

void UModioSubsystem::K2_ShutdownAsync(FOnErrorOnlyDelegate OnShutdownComplete)
{
	ShutdownAsync(FOnErrorOnlyDelegateFast::CreateLambda(
		[OnShutdownComplete](FModioErrorCode ec) { OnShutdownComplete.ExecuteIfBound(ec); }));
}

void UModioSubsystem::RunPendingHandlers()
{
	InvalidateUserSubscriptionCache();

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
	if (!CachedUserSubscriptions)
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
		Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModInfo>(ModInfo));
	});
}

void UModioSubsystem::K2_GetModInfoAsync(FModioModID ModId, FOnGetModInfoDelegate Callback)
{
	GetModInfoAsync(ModId, FOnGetModInfoDelegateFast::CreateLambda(
							   [Callback](FModioErrorCode ec, TOptional<FModioModInfo> ModInfo) {
								   Callback.ExecuteIfBound(ec, ToBP<FModioOptionalModInfo>(ModInfo));
							   }));
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback)
{
	Modio::GetModMediaAsync(ToModio(ModId), ToModio(AvatarSize),
							[Callback](Modio::ErrorCode ec, Modio::Optional<Modio::filesystem::path> Path) {
								// Manually calling ToUnreal on the path and assigning to the member of FModioImage
								// because we already have a Modio::filesystem::path -> FString overload of ToUnreal
								// TODO: @modio-ue4 Potentially refactor ToUnreal(Modio::filesystem::path) as a template
								// function returning type T so we can be explicit about the expected type
								if (Path)
								{
									FModioImageWrapper Out;
									Out.ImagePath = ToUnreal(Path.value());
									Callback.ExecuteIfBound(ec, Out);
								}
								else
								{
									Callback.ExecuteIfBound(ec, {});
								}
							});
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioGallerySize GallerySize, int32 Index,
									   FOnGetMediaDelegateFast Callback)
{
	Modio::GetModMediaAsync(ToModio(ModId), ToModio(GallerySize), Index,
							[Callback](Modio::ErrorCode ec, Modio::Optional<Modio::filesystem::path> Path) {
								// Manually calling ToUnreal on the path and assigning to the member of FModioImage
								// because we already have a Modio::filesystem::path -> FString overload of ToUnreal
								// TODO: @modio-ue4 Potentially refactor ToUnreal(Modio::filesystem::path) as a template
								// function returning type T so we can be explicit about the expected type
								if (Path)
								{
									FModioImageWrapper Out;
									Out.ImagePath = ToUnreal(Path.value());
									Callback.ExecuteIfBound(ec, Out);
								}
								else
								{
									Callback.ExecuteIfBound(ec, {});
								}
							});
}

void UModioSubsystem::GetModMediaAsync(FModioModID ModId, EModioLogoSize LogoSize, FOnGetMediaDelegateFast Callback)
{
	Modio::GetModMediaAsync(ToModio(ModId), ToModio(LogoSize),
							[Callback](Modio::ErrorCode ec, Modio::Optional<Modio::filesystem::path> Path) {
								// Manually calling ToUnreal on the path and assigning to the member of FModioImage
								// because we already have a Modio::filesystem::path -> FString overload of ToUnreal
								// TODO: @modio-ue4 Potentially refactor ToUnreal(Modio::filesystem::path) as a template
								// function returning type T so we can be explicit about the expected type
								if (Path)
								{
									FModioImageWrapper Out;
									Out.ImagePath = ToUnreal(Path.value());
									Callback.ExecuteIfBound(ec, Out);
								}
								else
								{
									Callback.ExecuteIfBound(ec, {});
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
		Callback.ExecuteIfBound({}, CachedModTags);
		return;
	}
	// TODO: @modio-UE4 capturing `this` is bad and we shouldn't do it. Better to store the cached tags as a TSharedPtr
	// and capture that by value, so we are guaranteed lifetime
	Modio::GetModTagOptionsAsync(
		[this, Callback](Modio::ErrorCode ec, Modio::Optional<Modio::ModTagOptions> ModTagOptions) {
			CachedModTags = ToUnrealOptional<FModioModTagOptions>(ModTagOptions);
			Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModTagOptions>(ModTagOptions));
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
								 Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioModID>(NewModID));
							 });
}

void UModioSubsystem::RequestEmailAuthCodeAsync(const FModioEmailAddress& EmailAddress,
												FOnErrorOnlyDelegateFast Callback)
{
	Modio::RequestEmailAuthCodeAsync(ToModio(EmailAddress),
									 [Callback](Modio::ErrorCode ec) { Callback.ExecuteIfBound(ToUnreal(ec)); });
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
	Modio::AuthenticateUserEmailAsync(ToModio(AuthenticationCode),
									  [Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); });
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
	Modio::AuthenticateUserExternalAsync(ToModio(User), ToModio(Provider),
										 [Callback](Modio::ErrorCode ec) { Callback.ExecuteIfBound(ec); });
}

void UModioSubsystem::K2_AuthenticateUserExternalAsync(const FModioAuthenticationParams& User,
													   EModioAuthenticationProvider Provider,
													   FOnErrorOnlyDelegate Callback)
{
	AuthenticateUserExternalAsync(
		User, Provider,
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
										 FOnGetTermsOfUseDelegateFast Callback)
{
	Modio::GetTermsOfUseAsync(ToModio(Provider), ToModio(Locale),
							  [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::Terms> Terms) {
								  Callback.ExecuteIfBound(ec, ToUnrealOptional<FModioTerms>(Terms));
							  });
}

void UModioSubsystem::K2_GetTermsOfUseAsync(EModioAuthenticationProvider Provider, EModioLanguage Locale,
											FOnGetTermsOfUseDelegate Callback)
{
	GetTermsOfUseAsync(
		Provider, Locale,
		FOnGetTermsOfUseDelegateFast::CreateLambda([Callback](FModioErrorCode ec, TOptional<FModioTerms> Terms) {
			Callback.ExecuteIfBound(ec, ToBP<FModioOptionalTerms>(Terms));
		}));
}

void UModioSubsystem::ClearUserDataAsync(FOnErrorOnlyDelegateFast Callback)
{
	Modio::ClearUserDataAsync([Callback](Modio::ErrorCode ec) { Callback.ExecuteIfBound(ToUnreal(ec)); });
}

void UModioSubsystem::K2_ClearUserDataAsync(FOnErrorOnlyDelegate Callback)
{
	ClearUserDataAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
}

void UModioSubsystem::GetUserMediaAsync(EModioAvatarSize AvatarSize, FOnGetMediaDelegateFast Callback)
{
	Modio::GetUserMediaAsync(ToModio(AvatarSize),
							 [Callback](Modio::ErrorCode ec, Modio::Optional<Modio::filesystem::path> Media) {
								 // Manually calling ToUnreal on the path and assigning to the member of FModioImage
								 // because we already have a Modio::filesystem::path -> FString overload of ToUnreal
								 // TODO: @modio-ue4 Potentially refactor ToUnreal(Modio::filesystem::path) as a
								 // template function returning type T so we can be explicit about the expected type
								 if (Media)
								 {
									 FModioImageWrapper Out;
									 Out.ImagePath = ToUnreal(Media.value());
									 Callback.ExecuteIfBound(ec, Out);
								 }
								 else
								 {
									 Callback.ExecuteIfBound(ec, {});
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

void UModioSubsystem::ForceUninstallModAsync(FModioModID ModToRemove, FOnErrorOnlyDelegate Callback)
{
	Modio::ForceUninstallModAsync(ToModio(ModToRemove),
								  [Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); });
}

void UModioSubsystem::SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegateFast Callback)
{
	Modio::SubmitModRatingAsync(ToModio(Mod), ToModio(Rating),
								[Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); });
}

void UModioSubsystem::K2_SubmitModRatingAsync(FModioModID Mod, EModioRating Rating, FOnErrorOnlyDelegate Callback)
{
	SubmitModRatingAsync(Mod, Rating, FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) {
							 Callback.ExecuteIfBound(ec);
						 }));
}

void UModioSubsystem::ReportContentAsync(FModioReportParams Report, FOnErrorOnlyDelegateFast Callback)
{
	Modio::ReportContentAsync(ToModio(Report), [Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); });
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
				Callback.ExecuteIfBound(ec, Out);
			}
			else
			{
				Callback.ExecuteIfBound(ec, {});
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

void UModioSubsystem::ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegateFast Callback)
{
	Modio::ArchiveModAsync(ToModio(Mod), [Callback](Modio::ErrorCode ec) { Callback.ExecuteIfBound(ToUnreal(ec)); });
}

void UModioSubsystem::K2_ArchiveModAsync(FModioModID Mod, FOnErrorOnlyDelegate Callback)
{
	ArchiveModAsync(
		Mod, FOnErrorOnlyDelegateFast::CreateLambda([Callback](FModioErrorCode ec) { Callback.ExecuteIfBound(ec); }));
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
