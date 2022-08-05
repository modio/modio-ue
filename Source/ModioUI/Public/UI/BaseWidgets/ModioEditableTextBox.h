// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/EditableTextBox.h"
#include "CoreMinimal.h"
#include "ModioRichTextBlock.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/Interfaces/IModioUIErrorDisplayWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioEditableTextBox.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FOnValidateTextPostChange, const FText&, ChangedText, FText&,
										  ErrorText);

UENUM(BlueprintType)
enum class EModioInputValidationType : uint8
{
	Email,
	NotEmpty,
	Custom
};

/**
 *
 */
UCLASS()
class MODIOUI_API UModioEditableTextBox : public UEditableTextBox, public IModioUIStringInputWidget, public IModioUITextValidator
{
	GENERATED_BODY()

protected:
	UModioEditableTextBox(const FObjectInitializer& Initializer);
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void NativeSetHintText(FText InHintText) override;
	virtual FString NativeGatherInput() override;
	const FSlateBrush* GetHintGlyph() const;
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
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioEditableTextBoxStyle"), Category="Widgets")
	FModioUIStyleRef TextBoxStyle = FModioUIStyleRef {"DefaultEditableTextBoxStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	TSharedPtr<SVerticalBox> MyVerticalBox;
	TSharedPtr<SModioRichTextBlock> MyErrorTextBlock;

};
