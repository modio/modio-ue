/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "ModioCommonMultiLineEditableTextBoxStyle.generated.h"

/**
 * The style of the Multi Line Editable Text Box within the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonMultiLineEditableTextBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	int32 MaxTextLength = -1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FText HintText;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush NormalBackgroundBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush HoveredBackgroundBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush FocusedBackgroundBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush ReadOnlyBackgroundBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FMargin Padding;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FTextBlockStyle TextStyle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateColor ForegroundColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateColor BackgroundColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateColor ReadOnlyForegroundColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateColor FocusedForegroundColor;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush NormalBorderBrush;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush HoveredBorderBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	FSlateBrush ErrorBorderBrush;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bShowHintIcon = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = bShowHintIcon), Category = "Properties")
	FSlateBrush HintIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = bShowHintIcon), Category = "Properties")
	FMargin HintIconPadding;

	/** Whether to apply the focused style when the text box is read only */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Properties")
	bool bApplyFocusedStyleInReadOnlyMode = false;
};
