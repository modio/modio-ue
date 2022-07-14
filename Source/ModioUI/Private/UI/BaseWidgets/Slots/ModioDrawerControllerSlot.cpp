// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"

void UModioDrawerControllerSlot::SynchronizeProperties() 
{
	switch (Edge)
	{
	case EModioDrawerControllerSlotEdge::Top:
		VerticalAlignment = VAlign_Top;
		HorizontalAlignment = DrawerHorizontalAlignment;
		break;
	case EModioDrawerControllerSlotEdge::Bottom:
		VerticalAlignment = VAlign_Bottom;
		HorizontalAlignment = DrawerHorizontalAlignment;
		break;
	case EModioDrawerControllerSlotEdge::Left:
		HorizontalAlignment = HAlign_Left;
		VerticalAlignment = DrawerVerticalAlignment;
		break;
	case EModioDrawerControllerSlotEdge::Right:
		HorizontalAlignment = HAlign_Right;
		VerticalAlignment = DrawerVerticalAlignment;
		break;
	}

	Super::SynchronizeProperties();
	UpdateWidgetTransform();
}


#if WITH_EDITOR
bool UModioDrawerControllerSlot::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty && (InProperty->GetFName() == FName("HorizontalAlignment") || InProperty->GetFName() == FName("VerticalAlignment")))
	{
		return false;
	}
	else
	{
		return Super::CanEditChange(InProperty);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void UModioDrawerControllerSlot::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == FName("bExpanded"))
	{
		if (bExpanded)
		{
			AnimateIn();
		}
		else
		{
			AnimateOut();
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION

#endif


EActiveTimerReturnType UModioDrawerControllerSlot::TickAnimate(double InCurrentTime, float InDeltaTime, bool bAnimateIn)
{
	if (bAnimateIn)
	{
		CurrentAnimationProgress -= (InDeltaTime / AnimationTime);
	}
	else
	{
		CurrentAnimationProgress += (InDeltaTime / AnimationTime);
	}
	CurrentAnimationProgress = FMath::Clamp<float>(CurrentAnimationProgress, 0, 1);

	UpdateWidgetTransform();

	// check for the animation finishing and kill the timer if we're done
	if (bAnimateIn)
	{
		return FMath::IsNearlyEqual(CurrentAnimationProgress, 1) ? EActiveTimerReturnType::Stop : EActiveTimerReturnType::Continue;
	}
	else
	{
		return FMath::IsNearlyEqual(CurrentAnimationProgress, 0) ? EActiveTimerReturnType::Stop
																 : EActiveTimerReturnType::Continue;
	}
}

void UModioDrawerControllerSlot::UpdateWidgetTransform()
{
	if (Slot)
	{
		FVector2D EdgeTransform;
		switch (Edge)
		{
			case EModioDrawerControllerSlotEdge::Top:
				EdgeTransform = FVector2D(0, -1);
				break;
			case EModioDrawerControllerSlotEdge::Bottom:
				EdgeTransform = FVector2D(0, 1);
				break;
			case EModioDrawerControllerSlotEdge::Left:
				EdgeTransform = FVector2D(-1, 0);
				break;
			case EModioDrawerControllerSlotEdge::Right:
				EdgeTransform = FVector2D(1, 0);
				break;
		}
		//* AllottedGeometry.GetLocalSize();
		TOptional<FSlateRenderTransform> CurrentTransform = Slot->GetWidget()->GetRenderTransform();
		if (!CurrentTransform.IsSet())
		{
			CurrentTransform = FSlateRenderTransform {};
		}
		CurrentTransform->SetTranslation(CurrentAnimationProgress * EdgeTransform *
										 Slot->GetWidget()->GetCachedGeometry().GetLocalSize());
		Slot->GetWidget()->SetRenderTransform(CurrentTransform);
	}
}

void UModioDrawerControllerSlot::AnimateIn()
{
	if (Slot)
	{
		if (CurrentAnimationTimer)
		{
			Slot->GetWidget()->UnRegisterActiveTimer(CurrentAnimationTimer.ToSharedRef());
		}
		CurrentAnimationTimer = Slot->GetWidget()->RegisterActiveTimer(
			1 / 60.f, FWidgetActiveTimerDelegate::CreateUObject(this, &UModioDrawerControllerSlot::TickAnimate, true));
	}
}

void UModioDrawerControllerSlot::AnimateOut()
{
	if (Slot)
	{
		if (CurrentAnimationTimer)
		{
			Slot->GetWidget()->UnRegisterActiveTimer(CurrentAnimationTimer.ToSharedRef());
		}
		CurrentAnimationTimer = Slot->GetWidget()->RegisterActiveTimer(
			1 / 60.f, FWidgetActiveTimerDelegate::CreateUObject(this, &UModioDrawerControllerSlot::TickAnimate, false));
	}
}

void UModioDrawerControllerSlot::SetExpanded(bool bNewState)
{
	if (bExpanded != bNewState)
	{
		if (bNewState)
		{
			AnimateIn();
		}
		else
		{
			AnimateOut();
		}
		bExpanded = bNewState;
	}
}

bool UModioDrawerControllerSlot::GetExpandedState()
{
	return bExpanded;
}

void UModioDrawerControllerSlot::SetSlotLeftEdge(EVerticalAlignment NewVAlign) 
{
	Edge = EModioDrawerControllerSlotEdge::Left;
	DrawerVerticalAlignment = NewVAlign;
	UpdateWidgetTransform();
}

void UModioDrawerControllerSlot::SetSlotRightEdge(EVerticalAlignment NewVAlign) 
{
	Edge = EModioDrawerControllerSlotEdge::Right;
	DrawerVerticalAlignment = NewVAlign;
	UpdateWidgetTransform();
}

void UModioDrawerControllerSlot::SetSlotTopEdge(EHorizontalAlignment NewHAlign) 
{
	Edge = EModioDrawerControllerSlotEdge::Top;
	DrawerHorizontalAlignment = NewHAlign;
	UpdateWidgetTransform();
}

void UModioDrawerControllerSlot::SetSlotBottomEdge(EHorizontalAlignment NewHAlign) 
{
	Edge = EModioDrawerControllerSlotEdge::Bottom;
	DrawerHorizontalAlignment = NewHAlign;
	UpdateWidgetTransform();
}
