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

#include "Core/ModioModInfoUI.h"
#include "Core/ModioReportInfoUI.h"
#include "CoreMinimal.h"
#include "Types/ModioAuthenticationParams.h"
#include "Types/ModioModInfo.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioLoadingSpinner.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/BaseWidgets/ModioWidgetBase.h"
#include "UI/BaseWidgets/ModioWidgetSwitcher.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Dialogs/ModioDialogInfo.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"

#include "ModioDialogController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogClosed);

/**
 * Enumerator with the possible replies to a dialog box
 **/
UENUM(BlueprintType)
enum class EModioDialogReply : uint8
{
	/** A neutral reply **/
	Neutral,

	/** Confirm an action **/
	Confirm,

	/** Cancel an action **/
	Cancel
};

/**
 * Class definition of a dialog controller, which helps with authentication,
 * subscriptions, and mod report operations
 **/
UCLASS()
class MODIOUI_API UModioDialogController : public UModioWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	FReply OnBackgroundButtonClicked();

	UPROPERTY()
	class UModioDialogBaseInternal* ActualDialog;

	TSharedPtr<class SBackgroundBlur> MyBackgroundBlur;
	TSharedPtr<class SModioButtonBase> MyBackgroundButton;
	TSharedPtr<class SBox> MyBackgroundBox;
	TSharedPtr<class SModioButtonBase> ClickDisableButton;

	/// @brief Controls the animation direction. If negative, we are transitioning from visible ->collapsed; 0, we are
	/// not transitioning at all; positive, we are transitioning from collapsed ->visible
	int32 AnimationDirection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioButtonStyle", DesignerRebuild), Category = "Appearance")
	FModioUIStyleRef InvisibleButtonStyleRef = FModioUIStyleRef {"InvisibleButtonStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "Widgets")
	bool bCurrentlyDisplayingDialog = false;

	UPROPERTY(Transient)
	TArray<FString> DialogInputValues;

	float GetBlurStrength() const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FInterpCurveFloat BlurCurve;

	UFUNCTION()
	FEventReply HandleBackgroundClick(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> AuthenticationChoiceDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> UnsubscribeConfirmationDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> EmailAuthenticationDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> ReportContentDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> ReportEmailDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> UninstallConfirmationDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> TermsOfUseDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> TermsOfUseFailDialog;

	/// @brief Needs to implement error display interface I guess
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> ErrorDisplayDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UModioDialogInfo> LogoutConfirmationDialog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TSubclassOf<UWidget> LoadingOverlay;

	friend class UModioDialogBaseInternal;

	void OnFetchExternalCompleted(FModioErrorCode ModioErrorCode) {}

	void HandleEmailRequestSent(FModioErrorCode ec, UModioDialogInfo* DestinationOnSuccess);
	void HandleAuthCodeRequestSent(FModioErrorCode ec);
	void SendEmailCodeRequest(FString EmailAddress, UModioDialogInfo* DestinationOnSuccess);
	void HandleUnsubscribe(FModioModID ec, bool IsSubscribe);
	void HandleExternalAuthenticationComplete(FModioErrorCode ec);
	void SubmitEmailAuthCode(FString Code);

public:
	FOnDialogClosed OnDialogClosed;

	UPROPERTY(Transient)
	TArray<class UModioDialogInfo*> DialogStack;

	UModioDialogBaseInternal* GetActualDialog();
	bool TrySetFocusToActiveDialog();
	void PopDialog();
	void FinalizeDialog(EModioDialogReply Reply = EModioDialogReply::Neutral);
	void PushDialog(UModioDialogInfo* InitialDialog, UObject* DialogDataSource = nullptr);

	void ShowAuthenticationChoiceDialog();
	void ShowEmailAuthenticationDialog();
	void ShowLogoutDialog();
	// Gets TermsOfUse from the API for the dialog
	void ShowTermsOfUseDialog(TSharedPtr<FModioUIAuthenticationProviderInfo> ProviderInfo);
	// Invoked by ShowTermsOfUseDialog(). Sets Terms text field and displays the dialog.
	void HandleTermsOfUseReceived(FModioErrorCode ec, TOptional<FModioTerms> Terms,
								  TSharedPtr<FModioUIAuthenticationProviderInfo> ProviderInfo);

	// Does this require a destination parameter?
	void BeginExternalAuthentication(EModioAuthenticationProvider Provider);

	void ShowReportContentDialog();
	void ShowUninstallConfirmationDialog(UObject* DialogDataSource);

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void ShowErrorDialog(FModioErrorCode ec, bool bCloseDialogsOnOK = false);

	void ShowLoadingDialog();
	void ShowModUnsubscribeDialog(UObject* DialogDataSource);
	void HandleUnsubscribeError(FModioErrorCode ModioErrorCode);
	void RequestUnsubscribe(const FModioModID& ModId, UModioDialogInfo* DestinationOnSuccess);
	void RequestUninstall(const FModioModID& ModId, UModioDialogInfo* DestinationOnSuccess);
	void HandleUninstallError(FModioErrorCode ec, UModioDialogInfo* DestinationOnSuccess);
	void LogOutCallback(FModioErrorCode ec);
	void LogOut();
	void ShowModReportDialog(UObject* DialogDataSource);
	void ShowReportEmailDialog(UObject* DialogDataSource);
	void HandleReportContent(FModioErrorCode ec, UModioDialogInfo* ModioDialogInfo);
	void ReportContentAsync(const FModioReportParams& ReportData, UModioDialogInfo* Destination);
	// has three widgets : the actual dialog stack, the error display widget and the in-progress widget
	// Error display and in-progress widgets should implement an interface I guess
};
