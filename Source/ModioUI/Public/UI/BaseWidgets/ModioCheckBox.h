// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CheckBox.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioCheckBox.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioCheckBox : public UCheckBox
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SModioRichTextBlock> MyDefaultLabel;
	void SynchronizeProperties() override;
	const struct FCheckBoxStyle* GetCheckboxStyle() const;
	const struct FModioUIStyleRef* GetLabelStyleRef() const;
	FText GetLabelText() const;
	virtual TSharedRef<SWidget> GetContentWidget();
	FMargin GetCheckboxContentPadding() const;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/// Will be used to generate a text label if not overridden by adding a child widget to this checkbox
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText LabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioCheckBoxStyle"), Category="Widgets")
	FModioUIStyleRef CheckBoxStyle;

	public:
	void SetLabelText(FText InText);
};
