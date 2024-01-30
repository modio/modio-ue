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
 * Rich Text Block that displays text and optionally an image near the text
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonRichTextBlock : public UCommonRichTextBlock
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Rich Text Block within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonRichTextStyle> InStyle);

protected:
	/** The style of the Rich Text Block within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonRichTextStyle> ModioStyle;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	/**
	 * Refreshes the content of the Rich Text Block
	 * @return The content of the Rich Text Block
	 */
	TSharedRef<SWidget> RefreshContent();

protected:
	/** The panel that contains the text and image */
	TSharedPtr<SHorizontalBox> TextImagePanel;

	/** The text widget */
	TSharedPtr<SWidget> TextWidget;

	/** The image widget */
	TSharedPtr<SImage> ImageWidget;

	/** The image brush */
	TOptional<FSlateBrush> ImageBrush;

	/** The image slot */
	SHorizontalBox::FSlot* ImageSlot;

	/** The text slot */
	SHorizontalBox::FSlot* TextSlot;

	/** The text image order */
	TOptional<EModioCommonRichTextStyleTextImageOrder> TextImageOrder;
};
