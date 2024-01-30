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
 * Dialog View that allows the user to show a message to the user
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDialogMessageView : public UModioCommonDialogMessageViewBase
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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> OkButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> ModDetailsButton;

	//~ Begin UUserWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

	/**
	 * Get all buttons that are used by this dialog (even if they are disabled)
	 * @return A map of button type to button widget
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	TMap<EModioCommonDialogButtonType, UModioCommonButtonBase*> GetButtons() const;

	/**
	 * Sets the enabled state of a button
	 * @param Button The button to set the enabled state of
	 * @param bEnabled Whether the button should be enabled
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetButtonEnabledState(UModioCommonButtonBase* Button, bool bEnabled);
};
