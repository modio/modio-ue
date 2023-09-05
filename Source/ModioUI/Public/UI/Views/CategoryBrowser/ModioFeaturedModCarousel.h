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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "ModioFeaturedModCarousel.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioFeaturedModCarousel : public UModioUserWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CarouselCanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod Carousel Configuration")
	TSubclassOf<class UModioFeaturedMod> ModTileTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod Carousel Configuration")
	float WidgetMargin = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod Carousel Configuration")
	float ScrollTime = 0.25f;

	// Adjust the input delay here.
	// Note that if this is set higher than scroll time the keyboard/controller inputs will not work
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod Carousel Configuration")
	float ScrollInputDelay = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bCalculateWidgetAmountBySize"),
			  Category = "Mod Carousel Configuration")
	FVector2D GeneratedWidgetSize = FVector2D(960.0f, 720.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod Carousel Configuration")
	UCurveFloat* AnimationCurve;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ScrollToTarget();
	void NavigateToIndex(int IndexToNavigate);
	void GenerateWidgets(TArray<UObject*> InWidgetData);
	void RePositionWidgets();

	FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SetFocusToCenterWidget();

	UFUNCTION()
	void NavigateToWidget(UWidget* InWidget);

private:
	int DisplayedAmount = 0;
	int CenterWidgetIndex = 0;
	int ScrollTargetIndex = 0;

	float AnimationTimer = 0.0f;
	float ScrollDistance = 0.0f;
	bool bIsAnimating = false;

	TArray<class UModioFeaturedMod*> GeneratedWidgets;
	TArray<float> WidgetPositions;

	UPROPERTY()
	TArray<UObject*> CachedWidgetData;
};
