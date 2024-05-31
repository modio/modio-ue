/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Engine/Engine.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioSubsystem.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModTagOptions.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "Core/ModioFilterParamsUI.h"

#include "ModioUISubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSubscriptionCompleted, FModioErrorCode, FModioModID);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModSubscriptionStatusChanged, FModioModID, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnModSubscribeFailed, FModioModID);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnModUnsubscribeFailed, FModioModID);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModLogoDownloadCompleted, FModioModID, FModioErrorCode,
									  TOptional<FModioImageWrapper>, EModioLogoSize);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModGalleryImageDownloadCompleted, FModioModID, FModioErrorCode, int32,
									  TOptional<FModioImageWrapper>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUserAvatarDownloadCompleted, FModioErrorCode, TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModCreatorAvatarDownloadCompleted, FModioModID, FModioErrorCode,
									   TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModInfoRequestCompleted, FModioModID, FModioErrorCode,
									   TOptional<FModioModInfo>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnListAllModsRequestCompleted, FString, FModioErrorCode,
									   TOptional<FModioModInfoList>);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAuthenticatedUserChanged, TOptional<FModioUser>);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnModManagementEventUI, FModioModManagementEvent);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayNotificationWidget, TScriptInterface<IModioUINotification>&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayNotificationParams, const FModioNotificationParams&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDisplayErrorManualParams, const FText&, const FText&, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetails, TScriptInterface<IModioModInfoUIDetails>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetailsForID, const FModioModID&);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnGetModEnabled, FModioModID, Mod);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModEnabledChanged, FModioModID, Mod, bool, bNewStateIsEnabled);

DECLARE_DYNAMIC_DELEGATE(FOnModBrowserCloseRequested);

DECLARE_MULTICAST_DELEGATE(FOnAuthenticationChangeStarted);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnDisplaySearchResults, FModioFilterParams, Params);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRetryAllAsyncLoaders);

/**
 *
 */
UCLASS() class MODIOUICORE_API UModioUISubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	APlayerController* OwningPlayerController;

protected:
	virtual void GetPreloadDependencies(TArray<UObject*>& OutDeps);

	friend class IModioUIMediaDownloadCompletedReceiver;
	friend class IModioUIModInfoReceiver;
	friend class IModioUIModManagementEventReceiver;
	friend class IModioUIInputDeviceChangedReceiver;
	friend class IModioUIUserChangedReceiver;
	friend class IModioUIUserAvatarDownloadCompletedReceiver;
	friend class IModioUIAuthenticationChangedReceiver;
	friend class IModioUIModEnableWidget;
	friend class IModioUINotificationController;
	friend class IModioUIModDetailsDisplay;

	UFUNCTION()
	void SubscriptionHandler(FModioErrorCode ErrorCode, FModioModID ID);

	UFUNCTION()
	void UnsubscribeHandler(FModioErrorCode ErrorCode, FModioModID ID);

	UFUNCTION()
	void UninstallHandler(FModioErrorCode ErrorCode, FModioModID ID);

	FOnModLogoDownloadCompleted OnModLogoDownloadCompleted;
	FOnUserAvatarDownloadCompleted OnUserAvatarDownloadCompleted;
	void LogoDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID,
							 EModioLogoSize LogoSize);

	void UserAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

	FOnModGalleryImageDownloadCompleted OnModGalleryImageDownloadCompleted;
	void GalleryImageDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID,
									 int32 Index);

	FOnModCreatorAvatarDownloadCompleted OnModCreatorAvatarDownloadCompleted;
	void CreatorAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID);

public:
	FOnAuthenticatedUserChanged OnUserChanged;
	FOnAuthenticationChangeStarted OnAuthenticationChangeStarted;

public:
	void OnAuthenticationComplete(FModioErrorCode ec);

protected:
	FOnModInfoRequestCompleted OnModInfoRequestCompleted;
	void ModInfoRequestCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
										TArray<FModioModID> IDs);

	FOnListAllModsRequestCompleted OnListAllModsRequestCompleted;
	void ListAllModsCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
									 FString RequestIdentifier);

public:
	FOnModManagementEventUI OnModManagementEvent;

protected:
	void ModManagementEventHandler(FModioModManagementEvent Event);

	void OnGetModTagOptionsComplete(FModioErrorCode ModioErrorCode, TOptional<FModioModTagOptions> ModioModTagInfos);

	FOnModBrowserCloseRequested OnModBrowserCloseRequested;

public:
	FOnDisplayNotificationWidget OnDisplayNotificationWidget;
	FOnDisplayNotificationParams OnDisplayNotificationParams;
	FOnDisplayErrorManualParams OnDisplayManualParams;
	FOnDisplayModDetails OnDisplayModDetails;
	FOnDisplayModDetailsForID OnDisplayModDetailsForID;
	FOnDisplaySearchResults OnDisplaySearchResults;
	FOnRetryAllAsyncLoaders OnRetryAllAsyncLoaders;

protected:
	TOptional<FModioModTagOptions> CachedModTags;

	FVector2D CachedMouseCursorLocation;

	UPROPERTY()
	class UUserWidget* ModBrowserInstance;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UUserWidget* GetModBrowserInstance();

	void RetryAllAsyncLoaders();

	void EnableModManagement();
	void DisableModManagement();

	void ShowModReportDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void ShowDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider);

	/// @brief Special native-only overload for when we want direct notification of success or failure but still want to
	/// broadcast UI events
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider,
									   FOnErrorOnlyDelegateFast DedicatedCallback);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestSubscriptionForModID(FModioModID ID, bool IncludeDependencies = false);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestRemoveSubscriptionForModID(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	bool QueryIsModEnabled(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	bool RequestModEnabledState(FModioModID ID, bool bNewEnabledState);

	void RequestRemoveSubscriptionForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating);
	void OnExternalUpdatesFetched(FModioErrorCode ErrorCode);
	void RequestRateUpForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateDownForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);

	void ShowModUnsubscribeDialog(UObject* DialogDataSource);

	void ShowLogoutDialog();
	void OnLogoutComplete(FModioErrorCode ModioErrorCode);
	void LogOut(FOnErrorOnlyDelegateFast DedicatedCallback);

	// Delegate for the subscription success or fail
	FOnSubscriptionCompleted OnSubscriptionRequestCompleted;

	// Perhaps this should also carry the error code and a TOptional<bool> for the newly changed state?
	FOnModSubscriptionStatusChanged OnSubscriptionStatusChanged;

	FOnModSubscribeFailed OnModSubscribeFailed;
	FOnModUnsubscribeFailed OnModUnsubscribeFailed;

	UPROPERTY(BlueprintAssignable, Category = "ModioUISubsystem")
	FOnModEnabledChanged OnModEnabledChanged;

	// RetVal delegates can be accessed only by setting them BlueprintReadOnly
	// Overriding or binding this delegate will disable the default enabled mod lookup
	UPROPERTY(BlueprintReadWrite, Category = "ModioUISubsystem")
	FOnGetModEnabled GetModEnabledDelegate;

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestUserAvatar();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestEmailAuthentication(FModioEmailAuthCode Code);

	/// @brief Special native-only overload for when we want direct notification of success or failure but still want to
	/// broadcast UI events
	void RequestEmailAuthentication(FModioEmailAuthCode Code, FOnErrorOnlyDelegateFast DedicatedCallback);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestGalleryImageDownloadForModID(FModioModID ID, int32 Index,
											 EModioGallerySize ImageSize = EModioGallerySize::Original);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestLogoDownloadForModID(FModioModID ID, EModioLogoSize LogoSize = EModioLogoSize::Thumb320);

	TOptional<FModioModTagOptions> GetTagOptionsList();

	void GetTagOptionsListAsync();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	float GetCurrentDPIScaleValue();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestModInfoForModIDs(TArray<FModioModID> IDs);

	// Uses RequestIdentifier so requesters can tell if a set of results or an error belongs to them
	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	FText FormatText(FText Input);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	UUserWidget* ShowModBrowserUIForPlayer(TSubclassOf<UUserWidget> MenuClass, APlayerController* Controller,
										   FOnModBrowserCloseRequested OnModBrowserCloseRequestedDelegate);

	/// Sets the browser visibility to collapsed, does not free resources and is generally more stable
	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void HideModBrowserUI();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void CloseModBrowserUI();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void ShowDetailsForMod(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	bool ShowSearchResults(const FModioModCategoryParams& SearchParameters, bool bIsDefaultFilter);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayNotification(UPARAM(ref) TScriptInterface<IModioUINotification>& Notification);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayNotificationParams(const FModioNotificationParams& Params);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayNotificationManual(FText Title, FText Message, bool bIsError);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayErrorDialog(FModioErrorCode ErrorCode);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	bool GetIsCollectionModDisableUIEnabled();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void ExecuteOnModBrowserCloseRequestedDelegate();

	// If CachedGameInfo is set, returns it through the callback
	// else retrieve it from the SDK first via UModioSubsystem and caches it
	void GetGameInfoAsync(FOnGetGameInfoDelegateFast Callback);

	void ShowUserAuth();

	bool IsUserAuthenticated();

	/**
	 * Equivalent to UModioSubsystem::PrioritizeTransferForMod but with UI notifications
	 * @param ModToPrioritize The ID for the mod to prioritize
	 */
	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void PrioritizeTransferForMod(FModioModID ModToPrioritize);

	TArray<FModioModID> ModsDownloadedThisSession;

	FModioErrorCode LastSubscriptionErrorCode;

	TOptional<FModioGameInfo> CachedGameInfo;
};
