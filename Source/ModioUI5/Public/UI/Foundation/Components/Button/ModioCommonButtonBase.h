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
#include "CommonButtonBase.h"
#include "ModioCommonButtonStyle.h"
#include "ModioCommonButtonBase.generated.h"

class UModioCommonTextBlock;
class UModioCommonImageStyle;
class UModioCommonImage;

/**
 * @brief Button that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Mod.io Common UI")
	FText OverrideActionDisplayName;

	/**
	 * Whether the button should trigger a click when the user presses enter or gamepad accept (Gamepad_FaceButton_Bottom) after the button has been focused
	 */
	UPROPERTY(EditAnywhere, Category = "Mod.io Common UI")
	bool bShouldTriggerButtonClickOnEnterOrGamepadAccept = true;

protected:
	UPROPERTY(EditAnywhere, Category = "Mod.io Common UI")
	FText ButtonText;

	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonTextBlock> TextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonImage> Icon;

public:
	UFUNCTION(BlueprintCallable, Category = "ModioButton")
	void SetLabel(FText NewLabel);

	/**
	 * @brief Set the style of the Button within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonButtonStyle> InStyle);

	bool IsButtonFocused() const;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = true))
	FCommonButtonBaseClicked OnButtonBaseFocusReceived;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess = true))
	FCommonButtonBaseClicked OnButtonBaseFocusLost;

	DECLARE_EVENT(UCommonButtonBase, FCommonButtonEvent);
	FCommonButtonEvent& OnFocusLost() const { return OnFocusLostEvent; }
private:
	mutable FCommonButtonEvent OnFocusLostEvent;
protected:
	// UCommonButtonBase interface
	virtual void BindTriggeringInputActionToClick() override;
	// UCommonButtonBase interface

	// UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
public:
	virtual void SynchronizeProperties() override;
	// UUserWidget interface

protected:
	virtual void RefreshIconStyle();

private:
	/**
	 * These properties are used to properly handle keyboard focusing which is not supported by default in Common UI
	 */

	/** The default style class for the button */
	UPROPERTY()
	TSubclassOf<UModioCommonButtonStyle> ModioDefaultStyleClass;

	/** The style class for the focused state of the button */
	UPROPERTY(Transient)
	UClass* ModioFocusedStyleClass;
};
