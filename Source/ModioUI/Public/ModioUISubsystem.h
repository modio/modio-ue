// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/Input/InputDeviceTypes.h"
#include "Core/Input/ModioUIInputProcessor.h"
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Engine/Engine.h"
#include "Framework/Commands/UICommandList.h"
#include "ModioSubsystem.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModTagOptions.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUINotification.h"

#include "ModioUISubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSubscriptionCompleted, FModioErrorCode, FModioModID);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModSubscriptionStatusChanged, FModioModID, bool);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModLogoDownloadCompleted, FModioModID, FModioErrorCode,
									   TOptional<FModioImageWrapper>);

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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetModTagOptionsCompleted, TOptional<FModioModTagOptions>);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayNotificationWidget, TScriptInterface<IModioUINotification>&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayNotificationParams, const FModioNotificationParams&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetails, TScriptInterface<IModioModInfoUIDetails>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetailsForID, const FModioModID&);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnGetModEnabled, FModioModID, Mod);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnModEnabledChanged, FModioModID, Mod, bool, bNewStateIsEnabled);

DECLARE_DYNAMIC_DELEGATE(FOnModBrowserClosed);

/**
 *
 */
UCLASS() class MODIOUI_API UModioUISubsystem : public UEngineSubsystem
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

	FOnGetModEnabled GetModEnabledDelegate;
	FOnModEnabledChanged OnModEnabledChanged;

	bool QueryIsModEnabled(FModioModID ID);

	void RequestModEnabledState(FModioModID ID, bool bNewEnabledState);

	UFUNCTION()
	void SubscriptionHandler(FModioErrorCode ec, FModioModID ID);

	UFUNCTION()
	void UnsubscribeHandler(FModioErrorCode ec, FModioModID ID);

	UFUNCTION()
	void UninstallHandler(FModioErrorCode ec, FModioModID ID);

	TSharedPtr<FUICommandList> CommonUICommands = nullptr;
	TSharedPtr<class FModioUIInputProcessor> Processor;
	FOnModLogoDownloadCompleted OnModLogoDownloadCompleted;
	FOnUserAvatarDownloadCompleted OnUserAvatarDownloadCompleted;
	void LogoDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID);

	void UserAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image);

	FOnModGalleryImageDownloadCompleted OnModGalleryImageDownloadCompleted;
	void GalleryImageDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID,
									 int32 Index);

	FOnModCreatorAvatarDownloadCompleted OnModCreatorAvatarDownloadCompleted;
	void CreatorAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID);

	FOnAuthenticatedUserChanged OnUserChanged;
	void OnAuthenticationComplete(FModioErrorCode ec);

	FOnModInfoRequestCompleted OnModInfoRequestCompleted;
	void ModInfoRequestCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
										TArray<FModioModID> IDs);

	FOnListAllModsRequestCompleted OnListAllModsRequestCompleted;
	void ListAllModsCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
									 FString RequestIdentifier);

	FOnModManagementEventUI OnModManagementEvent;
	void ModManagementEventHandler(FModioModManagementEvent Event);

	void OnGetModTagOptionsComplete(FModioErrorCode ModioErrorCode, TOptional<FModioModTagOptions> ModioModTagInfos);

	FOnModBrowserClosed OnModBrowserClosed;

	UPROPERTY()
	UModioUIStyleSet* LoadedDefaultStyleSet = nullptr;

	EModioUIInputMode LastInputMode;

	UFUNCTION()
	void HandleInputModeChanged(EModioUIInputMode NewDevice);
	FOnInputDeviceChanged OnInputDeviceChanged;

	EModioUIInputMode GetLastInputDevice()
	{
		return LastInputMode;
	}

	FOnDisplayNotificationWidget OnDisplayNotificationWidget;
	FOnDisplayNotificationParams OnDisplayErrorNotification;

	FOnDisplayModDetails OnDisplayModDetails;
	FOnDisplayModDetailsForID OnDisplayModDetailsForID;

	UPROPERTY()
	class UModioMenu* ModBrowserInstance;

	TOptional<FModioModTagOptions> CachedModTags;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void EnableModManagement();
	void DisableModManagement();

	int32 GetActiveTabIndex();

	TSharedPtr<FUICommandList> GetCommandList()
	{
		if (!CommonUICommands)
		{
			CommonUICommands = MakeShared<FUICommandList>();
		}
		return CommonUICommands;
	}

	void ShowUserAuthenticationDialog();
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);
	void ShowLogoutDialog();
	void ShowModReportDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestSubscriptionForModID(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestRemoveSubscriptionForModID(FModioModID ID);

	void RequestRemoveSubscriptionForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void OnRatingSubmissionComplete(FModioErrorCode ModioErrorCode, EModioRating ModioRating);
	void RequestRateUpForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateDownForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void SetActiveTabIndex(int TabIndex);
	void OnLogoutComplete(FModioErrorCode ModioErrorCode);
	void CloseSearchDrawer();
	void LogOut(FOnErrorOnlyDelegateFast DedicatedCallback);
	void ShowReportEmailDialog(UObject* DialogDataSource);
	void ShowUninstallConfirmationDialog(UObject* DialogDataSource);

	// Delegate for the subscription success or fail
	FOnSubscriptionCompleted OnSubscriptionRequestCompleted;

	// Perhaps this should also carry the error code and a TOptional<bool> for the newly changed state?
	FOnModSubscriptionStatusChanged OnSubscriptionStatusChanged;

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestUserAvatar();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestEmailAuthentication(FModioEmailAuthCode Code);

	/// @brief Special native-only overload for when we want direct notification of success or failure but still want to
	/// broadcast UI events
	void RequestEmailAuthentication(FModioEmailAuthCode Code, FOnErrorOnlyDelegateFast DedicatedCallback);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider);
	
	/// @brief Special native-only overload for when we want direct notification of success or failure but still want to
	/// broadcast UI events
	void RequestExternalAuthentication(EModioAuthenticationProvider Provider, FOnErrorOnlyDelegateFast DedicatedCallback);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestGalleryImageDownloadForModID(FModioModID ID, int32 Index,
											 EModioGallerySize ImageSize = EModioGallerySize::Original);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void RequestLogoDownloadForModID(FModioModID ID, EModioLogoSize LogoSize = EModioLogoSize::Thumb320);

	TOptional<FModioModTagOptions> GetTagOptionsList();

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
	class UModioUIStyleSet* GetDefaultStyleSet();

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	UMaterialInterface* GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	TArray<FName> GetAllNamedStyleNames();

	void SetControllerOverrideType(EModioUIInputMode NewOverride);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	UModioMenu* ShowModBrowserUIForPlayer(TSubclassOf<UModioMenu> MenuClass, APlayerController* Controller,
										  FOnModBrowserClosed BrowserClosedDelegate);

	/// @brief Sends a command to the browser UI in the form of a virtual keypress
	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void SendCommandToBrowserUI(FKey CommandKey, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void ShowDetailsForMod(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	bool DisplaySearchResults(FModioFilterParams SearchParameters);

	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayNotification(UPARAM(ref) TScriptInterface<IModioUINotification>& Notification);
	UFUNCTION(BlueprintCallable, Category = "ModioUISubsystem")
	void DisplayErrorNotification(const FModioNotificationParams& Params);

	void ExecuteOnModBrowserClosedDelegate();

	TArray<FModioModID> ModsDownloadedThisSession;
};
