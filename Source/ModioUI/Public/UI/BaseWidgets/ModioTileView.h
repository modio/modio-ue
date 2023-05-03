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

#include "Components/Overlay.h"
#include "Components/TileView.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/Slate/SModioTileView.h"

#include "ModioTileView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTileView : public UTileView
{
	GENERATED_BODY()

protected:
	TSharedPtr<class SModioTileView<UObject*>> MyDerivedTileView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool AllowPartialItems = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool CenterPanelItems;

	virtual void OnItemHoveredChanged(UObject* Item, bool bNewSelectionState);
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	TSharedRef<STableViewBase> RebuildListWidget() override;

	virtual void OnSelectionChangedInternal(NullableItemType FirstSelectedItem) override;

	UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bZoomSelectedEntryWidget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	ESlateVisibility ScrollbarVisibility;

public:
	void SetConsumeMouseWheel(EConsumeMouseWheel NewValue);
	float GetScrollOffset();

	UFUNCTION(BlueprintCallable, Category = "mod.io|TileView")
	void SetAllowPartialItems(bool bAllowPartialItems);
	UFUNCTION(BlueprintCallable, Category = "mod.io|TileView")
	void SetCenterPanelItems(bool bCenterPanelItems);
};
