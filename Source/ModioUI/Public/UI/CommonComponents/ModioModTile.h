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
#include "Components/MenuAnchor.h"
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioOverlay.h"
#include "UI/BaseWidgets/ModioPopupMenu.h"
#include "UI/BaseWidgets/ModioSizeBox.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioModTileBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"

#include "ModioModTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModTileDetailsClicked, UModioModInfoUI*, ModInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModTileReportClicked, UModioModInfoUI*, ModInfo);

/**
* Base class that displays an image and a label, and performs rating operations
**/
UCLASS()
class MODIOUI_API UModioModTile : public UModioModTileBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	/*virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;*/
	virtual void NativeOnSetExpandedState(bool bExpandedState) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry,
												const FNavigationEvent& InNavigationEvent) override;
	void OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating);
	UFUNCTION()
	void SubmitNegativeRating();
	UFUNCTION()
	void SubmitPositiveRating();
	UFUNCTION()
	void SubmitModReport();

	UFUNCTION()
	void NativeMoreOptionsClicked();
	UFUNCTION()
	void NativeReportClicked();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioPopupMenu* MoreOptionsMenu;

	UFUNCTION()
	FEventReply OnThumbnailMouseDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText MoreOptionsLabel;

	virtual void BuildCommandList(TSharedRef<FUICommandList> CommandList) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioImage* TileActiveFrame;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnModTileDetailsClicked OnDetailsClicked;

	UPROPERTY(BlueprintAssignable)
	FOnModTileReportClicked OnReportClicked;

	/**
	* Update the widget size
	* @param NewSize 2D vector with the new expected size
	**/
	UFUNCTION()
	void SetSizeOverride(FVector2D NewSize);
};
