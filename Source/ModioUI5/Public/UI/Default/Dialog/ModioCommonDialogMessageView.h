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
#include "UI/Foundation/Base/Dialog/ModioCommonDialogMessageViewBase.h"
#include "ModioCommonDialogMessageView.generated.h"

class UModioCommonTextBlock;
class UModioCommonEditableTextBox;
class UModioCommonButtonBase;

/**
 * @brief Report Email View that allows the user to enter an email address to send a report to
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonDialogMessageView : public UModioCommonDialogMessageViewBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> DescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> ConfirmButton;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnSetDataSource() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	TArray<UModioCommonButtonBase*> GetButtonsToDisplay() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetButtonEnabledState(UModioCommonButtonBase* Button, bool bEnabled);
};
