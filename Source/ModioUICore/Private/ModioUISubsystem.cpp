/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Core/ModioModInfoUI.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Loc/BeginModioLocNamespace.h"
#include "Math/IntPoint.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "ModioUICore.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Interfaces/IModioModBrowser.h"

void UModioUISubsystem::GetPreloadDependencies(TArray<UObject*>& OutDeps)
{
	Super::GetPreloadDependencies(OutDeps);
}

bool UModioUISubsystem::QueryIsModEnabled(FModioModID ID)
{
	// First we check if the user has overridden the default functionality
	if (GetModEnabledDelegate.IsBound())
	{
		return GetModEnabledDelegate.Execute(ID);
	}

	// Mods should be enabled as default
	return true;
}

bool UModioUISubsystem::RequestModEnabledState(FModioModID ID, bool bNewEnabledState)
{
	OnModEnabledChanged.Broadcast(ID, bNewEnabledState);
	return OnModEnabledChanged.IsBound();
}

void UModioUISubsystem::SubscriptionHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	LastSubscriptionErrorCode = ErrorCode;
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Warning, TEXT("Failed to subscribe to mod %s. Received error code %d: %s"),
			   *(ID.ToString()), ErrorCode.GetValue(), *(ErrorCode.GetErrorMessage()));
	}

	OnSubscriptionRequestCompleted.Broadcast(ErrorCode, ID);
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, true);
		RequestModEnabledState(ID, true);
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModInfoAsync(
			ID, FOnGetModInfoDelegateFast::CreateWeakLambda(
					this, [this, ErrorCode](FModioErrorCode InternalErrorCode, TOptional<FModioModInfo> ModInfo) {
						UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
						ModInfoObj->Underlying = ModInfo.Get(FModioModInfo());
						DisplayNotificationParams(
							UModioNotificationParamsLibrary::CreateSubscriptionNotification(ErrorCode ? ErrorCode : InternalErrorCode, ModInfoObj));
					}));

		OnModSubscribeFailed.Broadcast(ID);
	}
}

void UModioUISubsystem::UnsubscribeHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, false);
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->GetModInfoAsync(
				ID, FOnGetModInfoDelegateFast::CreateWeakLambda(
						this, [this, ErrorCode](FModioErrorCode InternalErrorCode, TOptional<FModioModInfo> ModInfo) {
							UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
							ModInfoObj->Underlying = ModInfo.Get(FModioModInfo());
							DisplayNotificationParams(
								UModioNotificationParamsLibrary::CreateUnsubscriptionNotification(ErrorCode ? ErrorCode : InternalErrorCode, ModInfoObj));
						}));
		}
	}
	else
	{
		OnModUnsubscribeFailed.Broadcast(ID);
		DisplayErrorDialog(ErrorCode);
	}
}

void UModioUISubsystem::UninstallHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	// Need to create a synthetic FModioModManagementEvent to let the UI know an uninstallation has occurred.
	// UninstallHandler is used by ForceUninstallAsync, which does not emit a ModManagementEvent as it's an async
	// function with a callback, and is not used by the mod management loop.
	FModioModManagementEvent Event;
	Event.ID = ID;
	Event.Event = EModioModManagementEventType::Uninstalled;
	Event.Status = ErrorCode;
	OnModManagementEvent.Broadcast(Event);

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModInfoAsync(
			ID, FOnGetModInfoDelegateFast::CreateWeakLambda(
					this, [this, ErrorCode](FModioErrorCode InternalErrorCode, TOptional<FModioModInfo> ModInfo) {
						UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
						ModInfoObj->Underlying = ModInfo.Get(FModioModInfo());
						DisplayNotificationParams(
							UModioNotificationParamsLibrary::CreateUninstallNotification(ErrorCode ? ErrorCode : InternalErrorCode, ModInfoObj));
					}));
	};
}

void UModioUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UModioSubsystem::StaticClass());
}

UUserWidget* UModioUISubsystem::GetModBrowserInstance()
{
	if (IsValid(ModBrowserInstance))
	{
		return ModBrowserInstance;
	}
	UE_LOG(ModioUICore, Error,
		   TEXT("ModBrowserInstance is null. Call ShowModBrowserUIForPlayer() to ensure ModBrowserInstance is valid."));
	return nullptr;
}

void UModioUISubsystem::RetryAllAsyncLoaders()
{
	OnRetryAllAsyncLoaders.Broadcast();
}

void UModioUISubsystem::EnableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
	{
		DependencySubsystem->EnableModManagement(
			FOnModManagementDelegateFast::CreateUObject(this, &UModioUISubsystem::ModManagementEventHandler));
	}
}

void UModioUISubsystem::DisableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
	{
		DependencySubsystem->DisableModManagement();
	}
}

void UModioUISubsystem::ShowModReportDialog(UObject* DialogDataSource)
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowReportMod(ModBrowserInstance, DialogDataSource);
	}
}

void UModioUISubsystem::ShowDialog(UObject* DialogDataSource)
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowDialog(ModBrowserInstance, DialogDataSource);
	}
}

void UModioUISubsystem::RequestExternalAuthentication(EModioAuthenticationProvider Provider)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_RequestExternalAuthentication(ModBrowserInstance, Provider);
	}
}

void UModioUISubsystem::RequestExternalAuthentication(EModioAuthenticationProvider Provider,
													  FOnErrorOnlyDelegateFast DedicatedCallback)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		Cast<IModioModBrowserInterface>(ModBrowserInstance)
			->RequestExternalAuthenticationNative(Provider, DedicatedCallback);
	}
}

void UModioUISubsystem::RequestSubscriptionForModID(FModioModID ID, bool IncludeDependencies)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuth();
				return;
			}
		}
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModAsync(
			ID, IncludeDependencies, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::SubscriptionHandler, ID));
	}
}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															this, &UModioUISubsystem::UnsubscribeHandler, ID),
														DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ForceUninstallModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															this, &UModioUISubsystem::UninstallHandler, ID),
														DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::ShowModUnsubscribeDialog(UObject* DialogDataSource)
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowModUnsubscribeDialog(ModBrowserInstance, DialogDataSource);
	}
}

void UModioUISubsystem::ShowLogoutDialog()
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_LogOut(ModBrowserInstance);
	}
}

void UModioUISubsystem::RequestRateUpForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuth();
				return;
			}
		}
	}

	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Positive,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Positive),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateDownForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuth();
				return;
			}
		}
	}

	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Negative,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Negative),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating Rating) {}

void UModioUISubsystem::OnExternalUpdatesFetched(FModioErrorCode ErrorCode) {}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::UnsubscribeHandler, ID));
	}
}

void UModioUISubsystem::RequestLogoDownloadForModID(FModioModID ID,
													EModioLogoSize LogoSize /*= EModioLogoSize::Thumb320*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, LogoSize,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::LogoDownloadHandler, ID, LogoSize));
	}
}

float UModioUISubsystem::GetCurrentDPIScaleValue()
{
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
			->GetDPIScaleBasedOnSize(FIntPoint(FGenericPlatformMath::FloorToInt(ViewportSize.X),
											   FGenericPlatformMath::FloorToInt(ViewportSize.Y)));
	}
	else
	{
		return 1.0f;
	}
}

void UModioUISubsystem::RequestModInfoForModIDs(TArray<FModioModID> IDs)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Bind the list of IDs as an additional parameter so that if we get an error we can report the error for the
		// specific IDs that were queried
		Subsystem->ListAllModsAsync(
			FModioFilterParams().MatchingIDs(IDs),
			FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ModInfoRequestCompletedHandler, IDs));
	}
}

void UModioUISubsystem::RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListAllModsAsync(
			Params, FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ListAllModsCompletedHandler,
															  RequestIdentifier));
	}
}

FText UModioUISubsystem::FormatText(FText Input)
{
	// Currently returns the text unmodified, this is a hook for replacing UI icons etc later.
	return Input;
}

UUserWidget* UModioUISubsystem::ShowModBrowserUIForPlayer(TSubclassOf<UUserWidget> MenuClass,
														  APlayerController* Controller,
														  FOnModBrowserCloseRequested OnModBrowserCloseRequestedDelegate)
{
	if (!MenuClass)
	{
		return nullptr;
	}

	if (IsValid(ModBrowserInstance))
	{
		ModBrowserInstance->SetVisibility(ESlateVisibility::Visible);
		return ModBrowserInstance;
	}

	ModBrowserInstance = CreateWidget<UUserWidget>(Controller, MenuClass);
	ModBrowserInstance->AddToViewport();
	OnModBrowserCloseRequested = OnModBrowserCloseRequestedDelegate;
	OwningPlayerController = Controller;
	UModioSubsystem* subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (subsystem->QueryUserProfile().IsSet())
	{
		subsystem->FetchExternalUpdatesAsync(
			FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnExternalUpdatesFetched));
	}
	return ModBrowserInstance;
}

void UModioUISubsystem::HideModBrowserUI()
{
	if (IsValid(ModBrowserInstance))
	{
		ModBrowserInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UModioUISubsystem::CloseModBrowserUI()
{
	if (ModBrowserInstance)
	{
		ModBrowserInstance->RemoveFromParent();
		ModBrowserInstance->ConditionalBeginDestroy();
		ModBrowserInstance = nullptr;
		// Fix for GC error after closing the browser, materials cannot really be released due to how some of the styles
		// are implemented
		// LoadedDefaultStyleSet->ClearMaterialCache();
	}
}

bool UModioUISubsystem::GetIsCollectionModDisableUIEnabled()
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		return IModioModBrowserInterface::Execute_GetIsCollectionModDisableUIEnabled(ModBrowserInstance);
	}
	return false;
}

void UModioUISubsystem::ExecuteOnModBrowserCloseRequestedDelegate()
{
	// Host application must bind BrowserClosedDelegate before creating ModBrowserUI
	OnModBrowserCloseRequested.ExecuteIfBound();
}

void UModioUISubsystem::GetGameInfoAsync(FOnGetGameInfoDelegateFast Callback)
{
	if(CachedGameInfo.IsSet())
	{
		Callback.Execute(FModioErrorCode(), CachedGameInfo);
	}
	else if(UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		ModioSubsystem->GetGameInfoAsync(UModioSDKLibrary::GetProjectGameId(), FOnGetGameInfoDelegateFast::CreateWeakLambda(this, [this, Callback](const FModioErrorCode& ErrorCode, const TOptional<FModioGameInfo>& GameInfo) {
			if(!ErrorCode && GameInfo.IsSet())
			{
				CachedGameInfo = GameInfo;
				Callback.Execute(ErrorCode, CachedGameInfo);
			}
		 }));
	}
}

void UModioUISubsystem::ShowUserAuth()
{
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowUserAuth(ModBrowserInstance);
	}
}

void UModioUISubsystem::ShowDetailsForMod(FModioModID ID)
{
	OnDisplayModDetailsForID.Broadcast(ID);

	// For now we need to do this so that we get the widget switcher to show the menu page. Eventually should be
	// decoupled further I think
	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowDetailsForMod(ModBrowserInstance, ID);
	}
}

bool UModioUISubsystem::ShowSearchResults(const FModioModCategoryParams& SearchParameters, bool bIsDefaultFilter)
{
	if (OnDisplaySearchResults.IsBound())
	{
		return OnDisplaySearchResults.Execute(SearchParameters.ToFilterParams());
	}

	if (ModBrowserInstance && ModBrowserInstance->Implements<UModioModBrowserInterface>())
	{
		IModioModBrowserInterface::Execute_ShowSearchResults(ModBrowserInstance, SearchParameters, bIsDefaultFilter);
	}
	return true;
}

void UModioUISubsystem::DisplayNotification(TScriptInterface<IModioUINotification>& Notification)
{
	OnDisplayNotificationWidget.Broadcast(Notification);
}

void UModioUISubsystem::DisplayNotificationParams(const FModioNotificationParams& Params)
{
	OnDisplayNotificationParams.Broadcast(Params);
}

void UModioUISubsystem::DisplayNotificationManual(FText Title, FText Message, bool bIsError)
{
	OnDisplayManualParams.Broadcast(Title, Message, bIsError);
}

void UModioUISubsystem::DisplayErrorDialog(FModioErrorCode ErrorCode)
{
	DisplayNotificationManual(LOCTEXT("Error", "Error"),
							  FText::Format(LOCTEXT("ErrorCode", "{0}: {1}"),
											FText::FromString(ErrorCode.GetErrorMessage()), FText::AsNumber(ErrorCode)),
							  true);
}

void UModioUISubsystem::LogoDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID,
											EModioLogoSize LogoSize)
{
	OnModLogoDownloadCompleted.Broadcast(ID, ec, Image, LogoSize);
}

void UModioUISubsystem::UserAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image)
{
	OnUserAvatarDownloadCompleted.Broadcast(ec, Image);
}

void UModioUISubsystem::RequestUserAvatar()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetUserMediaAsync(
			EModioAvatarSize::Thumb100,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::UserAvatarDownloadHandler));
	}
}

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnAuthenticationComplete));
	}
}

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															  this, &UModioUISubsystem::OnAuthenticationComplete),
														  DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestGalleryImageDownloadForModID(
	FModioModID ID, int32 Index, EModioGallerySize ImageSize /*= EModioGallerySize::Original*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, ImageSize, Index,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::GalleryImageDownloadHandler, ID, Index));
	}
}

void UModioUISubsystem::GalleryImageDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image,
													FModioModID ID, int32 Index)
{
	OnModGalleryImageDownloadCompleted.Broadcast(ID, ec, Index, Image);
}

void UModioUISubsystem::CreatorAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image,
													 FModioModID ID)
{
	OnModCreatorAvatarDownloadCompleted.Broadcast(ID, ec, Image);
}

void UModioUISubsystem::OnAuthenticationComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TOptional<FModioUser> NewUser = Subsystem->QueryUserProfile();
			if (NewUser.IsSet())
			{
				EnableModManagement();
			}
			OnUserChanged.Broadcast(NewUser);
		}
	}
}

void UModioUISubsystem::ModInfoRequestCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
													   TArray<FModioModID> IDs)
{
	if (ec)
	{
		// Got an error, notify anybody that the specified IDs got errors when requesting mod info
		for (FModioModID& ID : IDs)
		{
			OnModInfoRequestCompleted.Broadcast(ID, ec, {});
		}
	}
	else
	{
		for (FModioModInfo Info : ModInfos.GetValue().GetRawList())
		{
			OnModInfoRequestCompleted.Broadcast(Info.ModId, {}, Info);
		}
	}
}

void UModioUISubsystem::ListAllModsCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
													FString RequestIdentifier)
{
	OnListAllModsRequestCompleted.Broadcast(RequestIdentifier, ec, ModInfos);
}

void UModioUISubsystem::LogOut(FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		OnAuthenticationChangeStarted.Broadcast();

		Subsystem->ClearUserDataAsync(FOnErrorOnlyDelegateFast::CreateLambda(
			[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnLogoutComplete),
			 DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::OnLogoutComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		OnUserChanged.Broadcast({});
	}
}

TOptional<FModioModTagOptions> UModioUISubsystem::GetTagOptionsList()
{
	return CachedModTags;
}

void UModioUISubsystem::GetTagOptionsListAsync()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModTagOptionsAsync(
			FOnGetModTagOptionsDelegateFast::CreateUObject(this, &UModioUISubsystem::OnGetModTagOptionsComplete));
	}
}

void UModioUISubsystem::ModManagementEventHandler(FModioModManagementEvent Event)
{
	// If we've installed a mod, add it to the session downloads for the download queue panel
	if (Event.Event == EModioModManagementEventType::Installed)
	{
		ModsDownloadedThisSession.Add(Event.ID);
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->GetModInfoAsync(
				Event.ID,
				FOnGetModInfoDelegateFast::CreateWeakLambda(
					this, [this, Event](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo) {
						UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
						ModInfoObj->Underlying = ModInfo.Get(FModioModInfo());
						DisplayNotificationParams(
							UModioNotificationParamsLibrary::CreateInstallationNotification(Event.Status, ModInfoObj));
					}));
		}
	}

	// If we're uninstalled a mod, remove it from the session details
	if (Event.Event == EModioModManagementEventType::Uninstalled)
	{
		if (ModsDownloadedThisSession.Contains(Event.ID))
		{
			ModsDownloadedThisSession.Remove(Event.ID);
		}
	}

	OnModManagementEvent.Broadcast(Event);
}

void UModioUISubsystem::OnGetModTagOptionsComplete(FModioErrorCode ec, TOptional<FModioModTagOptions> ModTags)
{
	if (!ec)
	{
		CachedModTags = ModTags;
	}
}

bool UModioUISubsystem::IsUserAuthenticated()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		TOptional<FModioUser> NewUser = Subsystem->QueryUserProfile();
		return NewUser.IsSet(); 
	}
	return false;
}

void UModioUISubsystem::PrioritizeTransferForMod(FModioModID ModToPrioritize)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		FModioErrorCode ErrorCode = Subsystem->PrioritizeTransferForMod(ModToPrioritize);
		Subsystem->GetModInfoAsync(
			ModToPrioritize,
			FOnGetModInfoDelegateFast::CreateWeakLambda(
				this, [this](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo) {
					UModioModInfoUI* ModInfoObj = NewObject<UModioModInfoUI>();
					ModInfoObj->Underlying = ModInfo.Get(FModioModInfo());
					DisplayNotificationParams(
						UModioNotificationParamsLibrary::CreatePrioritizeTransferForModNotification(ErrorCode, ModInfoObj));
				}));
	}
}

#include "Loc/EndModioLocNamespace.h"
