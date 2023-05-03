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

#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "Templates/SharedPointer.h"
#include "UI/BaseWidgets/Slate/SModioListView.h"
#include "UI/Commands/ModioUIMenuEntry.h"
#include "UI/Interfaces/IModioUIPopupMenuContentWidget.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioPopupMenuStyle.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "ModioDefaultPopupMenuContent.generated.h"

/**
* Modio UI element to wrap popup menus, which stores multiple entries
* and a style for text or its border
**/
UCLASS()
class MODIOUI_API UModioDefaultPopupMenuContent : public UUserWidget,
												  public IModioUIPopupMenuContentWidget
{
	GENERATED_BODY()
public:
	void SelectListIndex(int index);
	void SelectCurrentIndex();

protected:
	FVector2D DesiredSizeFromParentMenu = FVector2D(32, 32);

	FOptionalSize GetDesiredItemWidth() const;
	FOptionalSize GetDesiredMenuHeight() const;
	float GetDesiredItemHeight() const;
	TSharedPtr<SBox> MyBox;
	TSharedPtr<SModioListView<TSharedPtr<FModioUIMenuEntry>>> MyList;
	TSharedPtr<class SRetainerWidget> MyBorder;
	TArray<TSharedPtr<FModioUIMenuEntry>> CurrentMenuEntries;
	FModioUIMenuCommandList CurrentCommandList;
	virtual void SynchronizeProperties() override;
	virtual void UpdateBoundValues();
	virtual void OnEntrySelected(TSharedPtr<FModioUIMenuEntry> SelectedEntry, ESelectInfo::Type SelectionType);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultButtonRichTextStyleWhite"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FModioUIStyleRef BorderStyle = FModioUIStyleRef {"DefaultPopupBorderStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FModioUIMaterialRef RetainerMaterial = FModioUIMaterialRef {"RoundedRectangle"};

	FModioUIStyleRef MenuItemStyle;

	FSlateSound ItemHoverSound;
	FSlateSound ItemPressedSound;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	int32 PreviewItemCount = 5;
#endif

	void PlayHoveredSound(const FGeometry& InGeometry, const FPointerEvent& InEvent)
	{
		FSlateApplication::Get().PlaySound(ItemHoverSound);
	}

	TSharedRef<ITableRow> GenerateMenuEntryWidget(TSharedPtr<FModioUIMenuEntry> Item,
												  const TSharedRef<STableViewBase>& OwnerTable);

	void NativeSetMenuEntries(const struct FModioUIMenuCommandList& Entries) override;
	void NativeSetDesiredSize(const FVector2D& DesiredSize) override;
	void NativeSetStyle(const FModioUIStyleRef StyleRef) override 
	{
		MenuItemStyle = StyleRef;

		const FModioPopupMenuStyle* PopupMenuStyle = MenuItemStyle.FindStyle<FModioPopupMenuStyle>();

		if (PopupMenuStyle) {
			ItemHoverSound = PopupMenuStyle->HoveredSound;
			ItemPressedSound = PopupMenuStyle->PressedSound;
		}
	}

	void ReleaseSlateResources(bool bReleaseChildren);
	TSharedRef<SWidget> RebuildWidget() override;
};
