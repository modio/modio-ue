// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/Interfaces/IModioUIInputHintDisplayWidget.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioRichTextButton.generated.h"

/**
 *
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	UModioRichTextBlock* ButtonContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	UModioInputBindingImage* InputHintImage;

	TOptional<FKey> KeyForInputHint;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText ButtonLabel;

	UFUNCTION()
	TArray<FString> GetStyleNames() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category="Widgets")
	FName DefaultStyleName = FName("default");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TEnumAsByte<ETextJustify::Type> Justification;

	virtual void NativeDisplayHintForInput(FKey VirtualKey) override;

	UFUNCTION()
	ESlateVisibility GetInputHintVisibility(EModioUIInputMode InputMode);

public:
	UFUNCTION()
	const FModioRichTextStyle& GetRichTextStyle() const;

	void SetLabel(FText NewLabel);

	void SetJustification(ETextJustify::Type NewJustification);
};
