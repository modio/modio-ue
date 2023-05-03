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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Slate/SModioRichTextBlock.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioCodeInputWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioCodeInputWidget : public UWidget, public IModioUIStringInputWidget, public IModioUITextValidator
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SModioCodeInput> MyInput;

	virtual FEventReply NativeValidateCodeInputCharacter(FString Character);
	virtual FString NativeGatherInput() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioCodeInputWidget")
	FEventReply ValidateCodeInputCharacter(const FString& Character);
	FEventReply ValidateCodeInputCharacter_Implementation(const FString& Character);

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	int32 NumberOfCharacters = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioCodeInputStyle"), Category = "Widgets")
	FModioUIStyleRef Style = FModioUIStyleRef {"DefaultCodeInputStyle"};

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
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	TSharedPtr<SVerticalBox> MyVerticalBox;
	TSharedPtr<SModioRichTextBlock> MyErrorTextBlock;

	// parameters for how many characters are needed
	// implement UIStringInput interface
	// Future: allow customization of each input widget

	// layout: TileView/ListView
	// parameter for spacing which gets fed to tileview

public:
	TSharedPtr<SModioCodeInput> GetMyInput();
};
