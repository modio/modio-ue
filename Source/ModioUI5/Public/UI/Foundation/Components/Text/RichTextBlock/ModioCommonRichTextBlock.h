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
#include "CommonRichTextBlock.h"
#include "Templates/SubclassOf.h"
#include "Templates/SharedPointer.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "ModioCommonRichTextBlock.generated.h"

class UModioCommonImage;
class UModioCommonRichTextStyle;
enum class EModioCommonRichTextStyleTextImageOrder : uint8;

/**
 * @brief Rich Text Block that displays text and optionally an image near the text
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonRichTextBlock : public UCommonRichTextBlock
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Rich Text Block within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonRichTextStyle> InStyle);

protected:
	/** @brief The style of the Rich Text Block within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonRichTextStyle> ModioStyle;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedRef<SWidget> RefreshContent();

protected:
	TSharedPtr<SHorizontalBox> TextImagePanel;

	TSharedPtr<SWidget> TextWidget;

	TSharedPtr<SImage> ImageWidget;
	TOptional<FSlateBrush> ImageBrush;

	SHorizontalBox::FSlot* ImageSlot;
	SHorizontalBox::FSlot* TextSlot;

	TOptional<EModioCommonRichTextStyleTextImageOrder> TextImageOrder;
};
