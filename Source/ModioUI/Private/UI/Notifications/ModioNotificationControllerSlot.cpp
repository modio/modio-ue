// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Notifications/ModioNotificationControllerSlot.h"

void UModioNotificationControllerSlot::SynchronizeProperties()
{
	switch (Edge)
	{
		case EModioNotificationControllerSlotEdge::Top:
			VerticalAlignment = VAlign_Top;
			HorizontalAlignment = NotificationHorizontalAlignment;
			break;
		case EModioNotificationControllerSlotEdge::Bottom:
			VerticalAlignment = VAlign_Bottom;
			HorizontalAlignment = NotificationHorizontalAlignment;
			break;
		case EModioNotificationControllerSlotEdge::Left:
			HorizontalAlignment = HAlign_Left;
			VerticalAlignment = NotificationVerticalAlignment;
			break;
		case EModioNotificationControllerSlotEdge::Right:
			HorizontalAlignment = HAlign_Right;
			VerticalAlignment = NotificationVerticalAlignment;
			break;
	}

	Super::SynchronizeProperties();
	UpdateWidgetTransform();
}

#if WITH_EDITOR
bool UModioNotificationControllerSlot::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty && (InProperty->GetFName() == FName("HorizontalAlignment") ||
					   InProperty->GetFName() == FName("VerticalAlignment")))
	{
		return false;
	}
	else
	{
		return Super::CanEditChange(InProperty);
	}
}

PRAGMA_DISABLE_OPTIMIZATION
void UModioNotificationControllerSlot::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
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

EActiveTimerReturnType UModioNotificationControllerSlot::TickAnimate(double InCurrentTime, float InDeltaTime,
																	 bool bAnimateIn)
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
		bool bDone = FMath::IsNearlyEqual(CurrentAnimationProgress, 1);
		if (bDone)
		{
			CurrentAnimationTimer = Slot->GetWidget()->RegisterActiveTimer(
				VisibleDuration,
				FWidgetActiveTimerDelegate::CreateUObject(this, &UModioNotificationControllerSlot::BeginAnimateOut));
		}
		return bDone ? EActiveTimerReturnType::Stop : EActiveTimerReturnType::Continue;
	}
	else
	{
		return FMath::IsNearlyEqual(CurrentAnimationProgress, 0) ? EActiveTimerReturnType::Stop
																 : EActiveTimerReturnType::Continue;
	}
}

void UModioNotificationControllerSlot::UpdateWidgetTransform()
{
	if (Slot)
	{
		FVector2D EdgeTransform;
		switch (Edge)
		{
			case EModioNotificationControllerSlotEdge::Top:
				EdgeTransform = FVector2D(0, -1);
				break;
			case EModioNotificationControllerSlotEdge::Bottom:
				EdgeTransform = FVector2D(0, 1);
				break;
			case EModioNotificationControllerSlotEdge::Left:
				EdgeTransform = FVector2D(-1, 0);
				break;
			case EModioNotificationControllerSlotEdge::Right:
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

void UModioNotificationControllerSlot::AnimateIn()
{
	if (Slot)
	{
		if (CurrentAnimationTimer)
		{
			Slot->GetWidget()->UnRegisterActiveTimer(CurrentAnimationTimer.ToSharedRef());
		}
		CurrentAnimationTimer = Slot->GetWidget()->RegisterActiveTimer(
			1 / 60.f,
			FWidgetActiveTimerDelegate::CreateUObject(this, &UModioNotificationControllerSlot::TickAnimate, true));
	}
}

EActiveTimerReturnType UModioNotificationControllerSlot::BeginAnimateOut(double, float)
{
	AnimateOut();
	return EActiveTimerReturnType::Stop;
}

void UModioNotificationControllerSlot::AnimateOut()
{
	if (Slot)
	{
		if (CurrentAnimationTimer)
		{
			Slot->GetWidget()->UnRegisterActiveTimer(CurrentAnimationTimer.ToSharedRef());
		}
		CurrentAnimationTimer = Slot->GetWidget()->RegisterActiveTimer(
			1 / 60.f,
			FWidgetActiveTimerDelegate::CreateUObject(this, &UModioNotificationControllerSlot::TickAnimate, false));
	}
}


bool UModioNotificationControllerSlot::IsShowing() 
{
	if (FMath::IsNearlyEqual(CurrentAnimationProgress, 0)) {
		return false;
	}
	else
	{
		return CurrentAnimationProgress > 0;
	}
}

void UModioNotificationControllerSlot::Show(float InVisibleDuration)
{
	VisibleDuration = InVisibleDuration;
	AnimateIn();
}


void UModioNotificationControllerSlot::SetSlotLeftEdge(EVerticalAlignment NewVAlign)
{
	Edge = EModioNotificationControllerSlotEdge::Left;
	NotificationVerticalAlignment = NewVAlign;
	UpdateWidgetTransform();
}

void UModioNotificationControllerSlot::SetSlotRightEdge(EVerticalAlignment NewVAlign)
{
	Edge = EModioNotificationControllerSlotEdge::Right;
	NotificationVerticalAlignment = NewVAlign;
	UpdateWidgetTransform();
}

void UModioNotificationControllerSlot::SetSlotTopEdge(EHorizontalAlignment NewHAlign)
{
	Edge = EModioNotificationControllerSlotEdge::Top;
	NotificationHorizontalAlignment = NewHAlign;
	UpdateWidgetTransform();
}

void UModioNotificationControllerSlot::SetSlotBottomEdge(EHorizontalAlignment NewHAlign)
{
	Edge = EModioNotificationControllerSlotEdge::Bottom;
	NotificationHorizontalAlignment = NewHAlign;
	UpdateWidgetTransform();
}
