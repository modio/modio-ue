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

#include "Components/EditableTextBox.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "ModioRichTextBlock.h"
#include "UI/Interfaces/IModioUIErrorDisplayWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioEditableTextBox.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FOnValidateTextPostChange, const FText&, ChangedText, FText&,
										  ErrorText);
DECLARE_MULTICAST_DELEGATE(FOnSubmit);
DECLARE_MULTICAST_DELEGATE(FOnNavigateDown);


/**
 * Enumerator with the validation types to perform on input
 **/
UENUM(BlueprintType)
enum class EModioInputValidationType : uint8
{
	/** Validate email **/
	Email,

	/** Validate content has some lenght **/
	NotEmpty,

	/** Run a custom validation **/
	Custom
};

/**
 * Modio UI element that provides an editable text functionality combining input and text validation
 **/
UCLASS()
class MODIOUI_API UModioEditableTextBox : public UEditableTextBox,
										  public IModioUIStringInputWidget,
										  public IModioUITextValidator,
										  public IModioInputMappingAccessor
{
	GENERATED_BODY()
public:
	void StartInput();
	TSharedPtr<SEditableTextBox> GetMyEditableTextBlock();
	FOnSubmit OnSubmit;
	FOnNavigateDown OnNavigateDown;

protected:
	// Cached style set for error validation
	TSharedPtr<FSlateStyleSet> ErrorStyleSet;
	UModioEditableTextBox(const FObjectInitializer& Initializer);
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeSetHintText(FText InHintText) override;
	virtual FString NativeGatherInput() override;
	const FSlateBrush* GetHintGlyph() const;
	bool SupportsKeyboardFocus() const{return true;};
	bool HasValidationError() const;
	const FSlateBrush* GetBorderImage() const;
	EVisibility GetHintGlyphVisibility() const;
	virtual void HandleOnTextChanged(const FText& InText) override;
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioEditableTextBoxStyle"), Category = "Widgets")
	FModioUIStyleRef TextBoxStyle = FModioUIStyleRef {"DefaultEditableTextBoxStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	TSharedPtr<SVerticalBox> MyVerticalBox;
	TSharedPtr<SModioRichTextBlock> MyErrorTextBlock;

	FReply OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
};
