/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "ModioSubsystem.h"
#include "UI/Foundation/Base/Auth/ModioCommonAuthViewBase.h"
#include "ModioCommonAuthView.generated.h"

class UModioCommonWidgetSwitcher;
class UModioCommonTermsOfUseViewBase;
class UModioCommonEmailAuthViewBase;
class UModioCommonEmailAuthLoadingViewBase;
class UModioCommonEmailAuthCodeViewBase;

/**
 * Auth View that displays the terms of use, email auth, email auth loading and email auth code views, with automatic switching between them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonAuthView : public UModioCommonAuthViewBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(UModioCommonAuthViewEmailAuthSucceeded);
	UModioCommonAuthViewEmailAuthSucceeded OnEmailAuthSucceeded;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetShowTermsOfView(bool bShow);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonWidgetSwitcher> AuthSwitcher;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnTermsOfUseViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnTermsOfUseViewSubmitClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnTermsOfUseViewLinkClicked(const FString& URL);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthViewBackClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthViewSubmitClicked(const FString& EmailAddress);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthLoadingViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthCodeViewBackClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthCodeViewCancelClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEmailAuthCodeViewSubmitClicked(const FString& AuthCode);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleEmailAuthCodeSent(FModioErrorCode ErrorCode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleEmailAuthenticated(FModioErrorCode ErrorCode);

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Auth View|Widgets")
	TObjectPtr<UModioCommonTermsOfUseViewBase> TermsOfUseView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Auth View|Widgets")
	TObjectPtr<UModioCommonEmailAuthViewBase> EmailAuthView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Auth View|Widgets")
	TObjectPtr<UModioCommonEmailAuthLoadingViewBase> EmailAuthLoadingView;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Auth View|Widgets")
	TObjectPtr<UModioCommonEmailAuthCodeViewBase> EmailAuthCodeView;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Auth View")
	bool bAuthCancelled = false;
};