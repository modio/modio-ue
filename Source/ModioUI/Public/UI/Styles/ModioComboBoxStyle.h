#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateWidgetStyle.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioComboBoxStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioComboBoxStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	FModioComboBoxStyle() : Super()
	{
		// Populate the combo box style from defaults
		ComboBoxStyle = FCoreStyle::Get().GetWidgetStyle<FComboBoxStyle>("ComboBox");
		// Set the button style to use flat colours
		ComboBoxStyle.ComboButtonStyle.ButtonStyle.SetNormal(FSlateColorBrush(FLinearColor(0.25, 0.25, 0.25)))
			.SetHovered(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)))
			.SetPressed(FSlateColorBrush(FLinearColor(0.5, 0.5, 0.5)));
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FComboBoxStyle ComboBoxStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FModioWidgetBorderStyle MenuBorderStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FModioWidgetBorderStyle ButtonBorderStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FTableRowStyle DropdownItemStyle;
};