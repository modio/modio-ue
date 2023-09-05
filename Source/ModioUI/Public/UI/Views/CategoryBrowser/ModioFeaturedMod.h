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

#include "Animation/WidgetAnimation.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioPopupMenu.h"
#include "UI/CommonComponents/ModioModTileBase.h"

#include "ModioFeaturedMod.generated.h"

/**
* Class that bridges mod features with actions, like submit ratings and reports
**/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModTilePressed, UWidget*, WidgetReference);

UCLASS()
class MODIOUI_API UModioFeaturedMod : public UModioModTileBase
{
	GENERATED_BODY()
public:
	FOnModTilePressed OnModTilePressed;

protected:
	float truncateDivider = 1.0f;
	bool bCurrentSelectionState = false;

	virtual void NativeOnSetDataSource() override;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTileClicked() override
	{
		if (bCurrentSelectionState)
		{
			if (MoreOptionsMenu->GetIsMenuOpen())
			{
				MoreOptionsMenu->SelectCurrentMenuItem();
				return;
			}
			Super::NativeTileClicked();
		}
	}

	UFUNCTION()
	void NativeMoreOptionsClicked();

	UFUNCTION()
	void SubmitNegativeRating();
	UFUNCTION()
	void SubmitPositiveRating();

	void OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating);

	virtual void BuildCommandList(TSharedRef<FUICommandList> CommandList) override;

	UFUNCTION()
	void SubmitModReport();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioPopupMenu* MoreOptionsMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioImage* TileActiveFrame;
};
