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

#include "Components/CheckBox.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioCheckBoxStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioCheckBox.generated.h"

/**
* Modio UI element that provides a check box functionality, in other words
* the capacity to receive boolean input along a text label
**/
UCLASS()
class MODIOUI_API UModioCheckBox : public UCheckBox
{
	GENERATED_BODY()
protected:
	TSharedPtr<class SModioRichTextBlock> MyDefaultLabel;
	void SynchronizeProperties() override;
	const struct FCheckBoxStyle* GetCheckboxStyle() const;
	const struct FModioUIStyleRef* GetLabelStyleRef() const;

	virtual TSharedRef<SWidget> GetContentWidget();
	FMargin GetCheckboxContentPadding() const;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/// Will be used to generate a text label if not overridden by adding a child widget to this checkbox
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FText LabelText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioCheckBoxStyle"), Category = "Widgets")
	FModioUIStyleRef CheckBoxStyle;

public:
	/**
	* Change the label for this Check Box
	* @param InText The new string to display
	**/
	FText GetLabelText() const;
	void SetLabelText(FText InText);
	void SetStyle(const FModioCheckBoxStyle* InStyle);
};
