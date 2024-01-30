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
#include "Components/EditableTextBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "Layout/WidgetPath.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "ModioCommonEditableTextBox.generated.h"

class UModioCommonEditableTextBoxStyle;
class SModioCommonEditableTextBox;

class MODIOUI_API SModioCommonEditableTextBox : public SEditableTextBox
{
public:
	//~ Begin SWidget Interface
	virtual bool SupportsKeyboardFocus() const override { return true; }
	//~ End SWidget Interface
};

/**
 * Editable Text Box styled using the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonEditableTextBox : public UEditableTextBox
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Editable Text Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonEditableTextBoxStyle> InStyle);

protected:
	/** The style of the Editable Text Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonEditableTextBoxStyle> ModioStyle;

public:
	/**
	 * Returns true if the text box is focused
	 * @return True if the text box is focused, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Editable Text Box")
	bool IsTextBoxFocused() const;

	/** Called whenever the text is changed programmatically or interactively by the user */
	UPROPERTY(BlueprintAssignable, Category="Widget Event", meta=(DisplayName="OnTextChanged (Multi-Line Text Box)"))
	FOnEditableTextBoxChangedEvent OnEditableTextChanged;
	
public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	/**
	 * Get the hint brush
	 * @return The hint brush
	 */
	const FSlateBrush* GetHintBrush() const;

	/**
	 * Get the hint brush visibility
	 * @return The hint brush visibility
	 */
	EVisibility GetHintBrushVisibility() const;

	// TODO: Handle this
	virtual bool HasValidationError() const { return false; }

	/**
	 * Get the border image (takes into account the error and hover states)
	 * @return The border image
	 */
	const FSlateBrush* GetBorderImage() const;

	//~ Begin SEditableTextBox Interface
	virtual void HandleOnTextChanged(const FText& InText) override;
	//~ End SEditableTextBox Interface

	/** Vertical box that contains the editable text box */
	TSharedPtr<SVerticalBox> MyVerticalBox;
};
