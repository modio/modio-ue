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

#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Border.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioDownloadQueueEntry.generated.h"

/**
* Modio UI element that represents a single download entry to display
* within an UModioDownloadQueueDrawer
**/
UCLASS()
class MODIOUI_API UModioDownloadQueueEntry : public UModioUserWidgetBase,
											 public IUserObjectListEntry,
											 public IModioUIMediaDownloadCompletedReceiver
{
	GENERATED_BODY()
protected:
	virtual void NativeOnSetDataSource() override;
	UFUNCTION()
	virtual void OnUnsubClicked();
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec, TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
	FEventReply OnEntryPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	const FModioRichTextStyle& GetRichTextStyle();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioImage* ModThumbnail;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModNameLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModSizeLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* UnsubscribeButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModStatusLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioDownloadQueueEntryStyle"),
			  Category = "Widgets")
	FModioUIStyleRef EntryStyle;

	FSlateSound PressedSound;

	FSlateSound HoveredSound;

	float truncateDivider = 1.0f;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UBorder* EntryBorder;
};
