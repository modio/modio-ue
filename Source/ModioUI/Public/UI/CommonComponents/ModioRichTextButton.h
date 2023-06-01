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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/Interfaces/IModioUIInputHintDisplayWidget.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioRichTextButton.generated.h"

/**
* Class definition for a rich text button that contains an image, 
* a function key, and a button label
**/
UCLASS()
class MODIOUI_API UModioRichTextButton : public UModioButton, public IModioUIInputHintDisplayWidget
{
	GENERATED_BODY()

protected:
	UModioRichTextButton(const FObjectInitializer& Initializer) : UModioButton(Initializer)
	{
		IsFocusable = true;
	}

	TSharedPtr<SGridPanel> MyContentGrid;
	SGridPanel::FSlot* MyHintImageSlot;
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	UModioRichTextBlock* ButtonContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	UModioInputBindingImage* InputHintImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText ButtonLabel;

	UFUNCTION()
	TArray<FString> GetStyleNames() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category = "Widgets")
	FName DefaultStyleName = FName("default");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TEnumAsByte<ETextJustify::Type> Justification;

	virtual void NativeDisplayHintForInput(FKey VirtualKey) override;

	UFUNCTION()
	ESlateVisibility GetInputHintVisibility(EModioUIInputMode InputMode);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	FKey KeyForInputHint;
	/**
	* Retrieve the rich text style in use for this instance
	* @return The text style used
	**/
	UFUNCTION()
	const FModioRichTextStyle& GetRichTextStyle() const;

	/**
	* Change the label displayed in this button
	* @param NewLabel Update text to display
	**/
	void SetLabel(FText NewLabel);

	/**
	 * Get the label displayed in this button
	 **/
	FString GetLabel();

	/**
	* Change the text orientation in this button
	* @param NewJustification Update text orientation
	**/
	void SetJustification(ETextJustify::Type NewJustification);
};
