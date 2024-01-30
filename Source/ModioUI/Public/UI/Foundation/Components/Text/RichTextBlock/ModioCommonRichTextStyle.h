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
#include "UI/Foundation/Components/Text/TextBlock/ModioCommonTextBlockStyle.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonRichTextStyle.generated.h"

class UModioCommonImage;
class UModioCommonImageStyle;

UENUM(BlueprintType)
enum class EModioCommonRichTextStyleTextImageOrder : uint8
{
	ImageFirst,
	TextFirst
};

/**
 * The style of the button within the Mod.io Common UI styling system
 * This also allows for an image to be attached to the text either before or after the text
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonRichTextStyle : public UCommonTextStyle
{
	GENERATED_BODY()

public:
	UModioCommonRichTextStyle()
	{
		ImageSize.Value = 0.0f;
		ImageSize.SizeRule = ESlateSizeRule::Automatic;
	}
	
	/** The brush to use as the image to attach to the text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rich Text Style")
	FSlateBrush ImageBrush;

	/** The order of text and image */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rich Text Style")
	EModioCommonRichTextStyleTextImageOrder TextImageOrder;

	/** The padding around the image */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rich Text Style")
	FMargin ImagePadding;

	/** The padding around the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	FMargin TextPadding;

	/** The desired size of the image */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	FSlateChildSize ImageSize;

	/** The desired size of the text within the style */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	FSlateChildSize TextSize;

	/** The horizontal alignment of the image */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	TEnumAsByte<EHorizontalAlignment> ImageHorizontalAlignment;

	/** The vertical alignment of the image */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	TEnumAsByte<EVerticalAlignment> ImageVerticalAlignment;

	/** The horizontal alignment of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	TEnumAsByte<EHorizontalAlignment> TextHorizontalAlignment;

	/** The vertical alignment of the text */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rich Text Style")
	TEnumAsByte<EVerticalAlignment> TextVerticalAlignment;
};