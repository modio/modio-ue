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
#include "Components/MultiLineEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "Layout/WidgetPath.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "ModioCommonMultiLineEditableTextBox.generated.h"

class UModioCommonMultiLineEditableTextBoxStyle;
class SModioCommonMultiLineEditableTextBox;

/**
 * Multi Line Editable Text Box which supports keyboard and gamepad navigation
 */
class MODIOUI_API SModioCommonMultiLineEditableTextBox : public SMultiLineEditableTextBox
{
public:
	virtual void SetApplyFocusedStyleInReadOnlyMode(bool bInApplyFocusedStyleInReadOnlyMode) { bApplyFocusedStyleInReadOnlyMode = bInApplyFocusedStyleInReadOnlyMode; }

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply HandleNavigation(const FGeometry& MyGeometry, EUINavigation Navigation);
	virtual void OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

protected:
	/** Whether we should ignore the next OnKeyDown event */
	bool bIgnoreOnKeyDown = false;

	/** Whether the text box is currently focused */
	bool bIsTextBoxFocused = false;

	/** Editable text box style. Used for intermediary storage */
	FEditableTextBoxStyle EditableTextBoxStyle;

	/** Whether to apply the focused style when the text box is read only */
	bool bApplyFocusedStyleInReadOnlyMode = false;
};

/**
 * Multi Line Editable Text Box styled using the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonMultiLineEditableTextBox : public UMultiLineEditableTextBox
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Multi Line Editable Text Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonMultiLineEditableTextBoxStyle> InStyle);

protected:
	/** The style of the Multi Line Editable Text Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonMultiLineEditableTextBoxStyle> ModioStyle;

public:
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	bool IsTextBoxFocused() const;

	/** Called whenever the text is changed programmatically or interactively by the user */
	UPROPERTY(BlueprintAssignable, Category="Widget Event", meta=(DisplayName="OnTextChanged (Multi-Line Text Box)"))
	FOnMultiLineEditableTextBoxChangedEvent OnMultiLineEditableTextChanged;

protected:
	/** Workaround as we lose focus when the auto completion closes. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool IsCaretMovedWhenGainFocus;

	/** Whether to select all text when the user clicks to give focus on the widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextWhenFocused;

	/** Whether to allow the user to back out of changes when they press the escape key */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool RevertTextOnEscape;

	/** Whether to clear keyboard focus when pressing enter to commit changes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool ClearKeyboardFocusOnCommit;

	/** Whether to select all text when pressing enter to commit changes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextOnCommit;

	/** Whether to show scroll bars */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	bool AlwaysShowScrollbars;

	/** Whether to show scroll bars */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	bool AllowMultiLine;

	/** The type of event that will trigger the display of the virtual keyboard */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	EVirtualKeyboardTrigger VirtualKeyboardTrigger;

	/** Sets what should happen when text is clipped because the block does not have enough space */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
	ETextOverflowPolicy OverflowPolicy;
	
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	const FSlateBrush* GetHintBrush() const;
	EVisibility GetHintBrushVisibility() const;

	virtual bool HasValidationError() const { return false; }
	
	const FSlateBrush* GetBorderImage() const;
	virtual void HandleOnCommonTextChanged(const FText& InText);

	TSharedPtr<SVerticalBox> MyVerticalBox;
};
