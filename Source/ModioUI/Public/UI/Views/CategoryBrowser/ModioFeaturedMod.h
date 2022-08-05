// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/WidgetAnimation.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioPopupMenu.h"
#include "UI/CommonComponents/ModioModTileBase.h"

#include "ModioFeaturedMod.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioFeaturedMod : public UModioModTileBase
{
	GENERATED_BODY()
protected:
	bool bCurrentSelectionState = false;

	virtual void NativeOnSetDataSource() override;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTileClicked() override
	{
		if (bCurrentSelectionState)
		{
			Super::NativeTileClicked();
		}
	}
	UFUNCTION()
	void SubmitNegativeRating();
	UFUNCTION()
	void SubmitPositiveRating();

	void OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating);

	UFUNCTION()
	void SubmitModReport();

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioPopupMenu* MoreOptionsMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioImage* TileActiveFrame;
};
