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

class UModioCommonTextBlock;
class UModioCommonButtonBase;
class UModioCommonEditableTextBox;

/**
 * @brief Email Auth Code View that displays the email auth code entry screen
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonEmailAuthCodeView : public UModioCommonEmailAuthCodeViewBase
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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> DigitTextBox1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> DigitTextBox2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> DigitTextBox3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> DigitTextBox4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> DigitTextBox5;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;

protected:
	// TODO: Redesign this
	UFUNCTION() 
	void OnTextBox1Changed(const FText& Text);

	UFUNCTION() 
	void OnTextBox2Changed(const FText& Text);

	UFUNCTION() 
	void OnTextBox3Changed(const FText& Text);

	UFUNCTION() 
	void OnTextBox4Changed(const FText& Text);

	UFUNCTION()
	void OnTextBox5Changed(const FText& Text);

	void HandleInputText(const FText& Text, UModioCommonEditableTextBox* TextBox);

public:
	// TODO: Redesign this
	FString GetInputText() const;
	void SetValidationTextVisibility(ESlateVisibility Visbility);
	bool IsCodeValid(const FString& Code);
	void SetIsBusy(bool Busy);
};
