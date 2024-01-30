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
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthViewBase.h"
#include "ModioCommonEmailAuthView.generated.h"

class UModioCommonTextBlock;
class UModioCommonEditableTextBox;
class UModioCommonButtonBase;

/**
 * Email Auth View that displays a form for the user to enter their email address to begin the email auth process
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonEmailAuthView : public UModioCommonEmailAuthViewBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> DescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> ValidationTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonEditableTextBox> EmailTextBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> SubmitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetValidationTextVisibility(ESlateVisibility Visbility);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool IsEmailValid(const FString& Email);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnEditableTextBoxTextChanged(const FText& InText);
};
