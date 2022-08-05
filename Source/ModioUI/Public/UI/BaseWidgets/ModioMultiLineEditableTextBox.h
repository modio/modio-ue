

#pragma once

#include "Components/MultiLineEditableTextBox.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "Slate/SModioRichTextBlock.h"
#include "UI/Interfaces/IModioUITextValidator.h"

#include "ModioMultiLineEditableTextBox.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioMultiLineEditableTextBox : public UMultiLineEditableTextBox, public IModioUIStringInputWidget, public IModioUITextValidator
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widgets")
	FModioUIStyleRef ErrorTextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};
	
	TSharedPtr<SVerticalBox> MyVerticalBox;
	TSharedPtr<SModioRichTextBlock> MyErrorTextBlock;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioMultiLineEditableTextBoxStyle"), Category="Widgets")
	FModioUIStyleRef TextBoxStyle = FModioUIStyleRef {"DefaultMultiLineEditableTextBoxStyle"};

};
