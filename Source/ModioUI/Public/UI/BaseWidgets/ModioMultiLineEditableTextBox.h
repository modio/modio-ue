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

#include "ModioMultiLineEditableTextBox.generated.h"

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
protected:
	virtual FString NativeGatherInput() override;

	virtual void NativeSetHintText(FText InHintText) override;

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
};
