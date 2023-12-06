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

#include "Components/MultiLineEditableTextBox.h"
#include "CoreMinimal.h"
#include "Slate/SModioRichTextBlock.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#include "ModioMultiLineEditableTextBox.generated.h"

/**
 * Modio Multi Line Editable Text Box. Supports keyboard/gamepad navigation
 */
class MODIOUI_API SModioMultiLineEditableTextBox : public SMultiLineEditableTextBox
{
public:
	virtual void SetApplyFocusedStyleInReadOnlyMode(bool bInApplyFocusedStyleInReadOnlyMode) { bApplyFocusedStyleInReadOnlyMode = bInApplyFocusedStyleInReadOnlyMode; }

	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply HandleNavigation(const FGeometry& MyGeometry, EUINavigation Navigation);
	virtual void HandleOnTextCursorMoved(const FTextLocation& NewCursorPosition);

	virtual void OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

protected:
	/** Whether we should ignore the next OnKeyDown event */
	bool bIgnoreOnKeyDown = false;

	FTextLocation LastCursorPosition;

	/** Whether the text box is currently focused */
	bool bIsTextBoxFocused = false;

	/** Editable text box style. Used for intermediary storage */
	FEditableTextBoxStyle EditableTextBoxStyle;

	/** Whether to apply the focused style when the text box is read only */
	bool bApplyFocusedStyleInReadOnlyMode = false;
};

/**
 * This class facilitates an editable text box with multiline support as well an
 * array of validation rules for that text.
 **/
UCLASS()
class MODIOUI_API UModioMultiLineEditableTextBox : public UMultiLineEditableTextBox,
												   public IModioUIStringInputWidget,
												   public IModioUITextValidator
{
	GENERATED_BODY()

	void HandleOnTextCursorMoved(const FTextLocation& NewCursorPosition);

public:
	TSharedPtr<SMultiLineEditableTextBox> GetEditableTextBox();

protected:
	// Cached style set for input validation
	TSharedPtr<FSlateStyleSet> ErrorStyleSet;
	virtual FString NativeGatherInput() override;

	virtual void NativeSetHintText(FText InHintText) override;
	virtual void NativeSetInput(const FString& Input) override;

	virtual void SynchronizeProperties() override;

	bool HasValidationError() const;

	void HandleOnTextChangedDelegate(const FText& InText);

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeGetTextValidationRules(TArray<FModioTextValidationRule>& Rules)
	{
		Rules = ValidationRules;
	}

	virtual void NativeSetValidationError(FText ErrorText) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data Validation")
	bool bValidateInput = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data Validation",
			  meta = (EditCondition = "bValidateInput", EditConditionHides))
	TArray<FModioTextValidationRule> ValidationRules;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data Validation",
			  meta = (EditCondition = "bValidateInput", EditConditionHides))
	bool bDisplayValidationErrors = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef ErrorTextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	TSharedPtr<SVerticalBox> MyVerticalBox;
	TSharedPtr<SModioRichTextBlock> MyErrorTextBlock;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioMultiLineEditableTextBoxStyle"),
			  Category = "Widgets")
	FModioUIStyleRef TextBoxStyle = FModioUIStyleRef {"DefaultMultiLineEditableTextBoxStyle"};

	/** Whether to apply the focused style when the text box is read only */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	bool bApplyFocusedStyleInReadOnlyMode = false;
};
