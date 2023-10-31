// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Views/CategoryBrowser/ModioFeaturedModCarousel.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedMod.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "Core/ModioModInfoUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/ModioModInfoUI.h"
#include "Curves/CurveFloat.h"

void UModioFeaturedModCarousel::NativePreConstruct() 
{
	Super::NativePreConstruct();
}

void UModioFeaturedModCarousel::NativeConstruct() 
{
	Super::NativeConstruct();
}

void UModioFeaturedModCarousel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) 
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (AnimationTimer < ScrollTime)
	{
		AnimationTimer += InDeltaTime;
		ScrollToTarget();
	}

	if (AnimationTimer >= ScrollTime && bIsAnimating) 
	{
		bIsAnimating = false;
		CenterWidgetIndex = ScrollTargetIndex;
		RePositionWidgets();
	}
}

void UModioFeaturedModCarousel::ScrollToTarget()
{
	int index = 0;
	for (auto& widget : GeneratedWidgets)
	{
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot);
		if (!slot || !WidgetPositions.IsValidIndex(index))
		{
			continue;
		}
		float position = WidgetPositions[index];
		index++;
		FVector2D targetPosition = FVector2D(
			FMath::Lerp(position, position - ScrollDistance, AnimationCurve->GetFloatValue(AnimationTimer)), 0.0f);
		slot->SetPosition(targetPosition);
		
	}
}

void UModioFeaturedModCarousel::NavigateToWidget(UWidget* InWidget) 
{
	int index = 0;
	UModioFeaturedMod* widget = Cast<UModioFeaturedMod>(InWidget);
	if (widget)
	{
		GeneratedWidgets.Find(widget, index);
		NavigateToIndex(index);
	}
}

void UModioFeaturedModCarousel::NavigateToIndex(int IndexToNavigate)
{
	AnimationTimer = 0.0f;

	ScrollTargetIndex = FMath::Clamp(IndexToNavigate, 0, GeneratedWidgets.Num()-1);

	UCanvasPanelSlot* targetSlot = Cast<UCanvasPanelSlot>(GeneratedWidgets[ScrollTargetIndex]->Slot);
	UCanvasPanelSlot* centerSlot = Cast<UCanvasPanelSlot>(GeneratedWidgets[CenterWidgetIndex]->Slot);

	ScrollDistance = targetSlot->GetPosition().X;

	WidgetPositions.Empty();
	for (auto& widget : GeneratedWidgets)
	{
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot);
		WidgetPositions.Add(slot->GetPosition().X);
	}
	bIsAnimating = true;
}

void UModioFeaturedModCarousel::GenerateWidgets(TArray<UObject*> InWidgetData) 
{

	GeneratedWidgets.Empty();
	for (auto& child : CarouselCanvasPanel->GetAllChildren())
	{
		CarouselCanvasPanel->RemoveChild(child);
	}

	// We want to generate extra widgets to both sides to achieve seamless scrolling 
	float panelWidth = CarouselCanvasPanel->GetCachedGeometry().GetLocalSize().X;
	DisplayedAmount = 1 + (FMath::FloorToInt(panelWidth / GeneratedWidgetSize.X) * 2);

	int amountToGenerate = InWidgetData.Num() >= DisplayedAmount ? InWidgetData.Num() : DisplayedAmount;
	
	for (int i = 0; i < amountToGenerate; i++)
	{
		UModioFeaturedMod* widget = CreateWidget<UModioFeaturedMod>(this, ModTileTemplate);
		GeneratedWidgets.Add(widget);
		CarouselCanvasPanel->AddChildToCanvas(widget);
	}

	int dataIndex = 0;
	for (auto& widget : GeneratedWidgets)
	{
		if (!IsDesignTime())
		{
			widget->OnModTilePressed.AddUniqueDynamic(this, &UModioFeaturedModCarousel::NavigateToWidget);
		}

		if (InWidgetData.IsValidIndex(dataIndex))
		{
			CachedWidgetData = InWidgetData;
			widget->SetDataSource(InWidgetData[dataIndex]);
		}

		dataIndex = dataIndex + 1 >= InWidgetData.Num() ? 0 : dataIndex + 1;
	}
	CenterWidgetIndex = 0;
}

void UModioFeaturedModCarousel::RePositionWidgets() 
{
	float panelWidth = CarouselCanvasPanel->GetCachedGeometry().GetLocalSize().X;
	float desiredWidth = GeneratedWidgetSize.X;
	int widgetsPerSide = FMath::FloorToInt(panelWidth / GeneratedWidgetSize.X);

	if (GeneratedWidgets.Num() < widgetsPerSide * 2 + 1)
	{
		GenerateWidgets(CachedWidgetData);
		RePositionWidgets();
		return;
	}

	for (auto& widget : GeneratedWidgets)
	{
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot);
		slot->SetAlignment(FVector2D(0.5f, 0.5f));
		slot->SetSize(GeneratedWidgetSize);
		slot->SetAnchors(FAnchors(0.5f, 0.5f));
		slot->SetPosition(FVector2D(0.0f, 0.0f));
		widget->SetVisibility(ESlateVisibility::Collapsed);
		widget->UpdateItemSelection(*widget, false);
	}

	// Center widget;
	GeneratedWidgets[CenterWidgetIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	GeneratedWidgets[CenterWidgetIndex]->UpdateItemSelection(*GeneratedWidgets[CenterWidgetIndex], true);
	GeneratedWidgets[CenterWidgetIndex]->SetKeyboardFocus();
	UCanvasPanelSlot* centerSlot = Cast<UCanvasPanelSlot>(GeneratedWidgets[CenterWidgetIndex]->Slot);
	centerSlot->SetPosition(FVector2D::ZeroVector);

	if (GeneratedWidgets.Num() <= 1)
	{
		return;
	}

	//left widgets
	float currentPosition = -desiredWidth-WidgetMargin;
	for (int i = 0; i < widgetsPerSide; i++) 
	{
		int index = CenterWidgetIndex - 1 - i;
		index = index < 0 ? GeneratedWidgets.Num() + index : index;
		if (!GeneratedWidgets.IsValidIndex(index))
		{
			continue;
		}
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(GeneratedWidgets[index]->Slot);
		slot->SetPosition(FVector2D(currentPosition, 0.0f));
		currentPosition -= desiredWidth + WidgetMargin;
		GeneratedWidgets[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	//right widgets
	currentPosition = desiredWidth + WidgetMargin;
	for (int i = 0; i < widgetsPerSide; i++)
	{
		int index = CenterWidgetIndex + 1 + i;
		index = index < 0 ? GeneratedWidgets.Num() + index : index;
		index = index >= GeneratedWidgets.Num() ? index - GeneratedWidgets.Num() : index;
		if (!GeneratedWidgets.IsValidIndex(index))
		{
			continue;
		}
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(GeneratedWidgets[index]->Slot);
		slot->SetPosition(FVector2D(currentPosition, 0.0f));
		currentPosition += desiredWidth + WidgetMargin;
		GeneratedWidgets[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

FReply UModioFeaturedModCarousel::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) 
{
	if (!HasFocusedDescendants())
	{
		return FReply::Unhandled();
	}

	if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Left) && AnimationTimer > ScrollInputDelay)  // just a bit of delay, keyboards can spam inputs every frame
	{
		if (bIsAnimating)
		{
			CenterWidgetIndex = ScrollTargetIndex;
			RePositionWidgets();
		}
		
		NavigateToIndex(ScrollTargetIndex <= 0 ? GeneratedWidgets.Num() - 1 : ScrollTargetIndex - 1);
		return FReply::Handled();
	}
	else if (GetCommandKeyForEvent(InKeyEvent).Contains(FModioInputKeys::Right) && AnimationTimer > ScrollInputDelay)
	{
		if (bIsAnimating)
		{
			CenterWidgetIndex = ScrollTargetIndex;
			RePositionWidgets();
		}
		NavigateToIndex(ScrollTargetIndex + 1 >= GeneratedWidgets.Num() ? 0 : ScrollTargetIndex + 1);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UModioFeaturedModCarousel::SetFocusToCenterWidget()
{
	if (GeneratedWidgets.IsValidIndex(CenterWidgetIndex))
	{
		GeneratedWidgets[CenterWidgetIndex]->SetKeyboardFocus();
	}
}
