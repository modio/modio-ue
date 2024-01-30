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
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthCodeViewBase.h"
#include "ModioCommonEmailAuthCodeView.generated.h"

class UEditableTextBox;
class UModioCommonTextBlock;
class UModioCommonButtonBase;
class UModioCommonCodeInputTextBox;

/**
 * Email Auth Code View that displays the email auth code entry screen
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonEmailAuthCodeView : public UModioCommonEmailAuthCodeViewBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> DescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> ValidationTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> SubmitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

	/** Code input text box. Use CustomCodeInputTextBox if you want to use your own editable text box */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonCodeInputTextBox> CodeInputTextBox;

	/** Custom code input text box. Use CodeInputTextBox if you want to use the default code input text box with the handy styling */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UEditableTextBox> CustomCodeInputTextBox;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnSubmitClicked();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCustomSubmitClicked(const FText& Text, ETextCommit::Type CommitMethod);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	FString GetInputText() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetValidationTextVisibility(ESlateVisibility Visbility);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool IsCodeValid(const FString& Code);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetIsBusy(bool bBusy);
};
