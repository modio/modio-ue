// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioWidgetSwitcher.h"

void UModioWidgetSwitcher::PushActiveWidgetIndex(int32 Index)
{
	if (Index != GetActiveWidgetIndex())
	{
		ActiveWidgetIndexStack.Push(GetActiveWidgetIndex());
		SetActiveWidgetIndex(Index);
	}
}

int32 UModioWidgetSwitcher::PopActiveWidgetIndex()
{
	if (ActiveWidgetIndexStack.Num())
	{
		int32 PrevWidgetIndex = ActiveWidgetIndexStack.Pop();
		SetActiveWidgetIndex(PrevWidgetIndex);
		return PrevWidgetIndex;
	}
	return INDEX_NONE;
}

void UModioWidgetSwitcher::SetActiveWidgetIndex(int32 Index)
{
	ActiveWidgetIndex = Index;
	if (MyWidgetSwitcher.IsValid())
	{
		// Ensure the index is clamped to a valid range.
		int32 SafeIndex = FMath::Clamp(ActiveWidgetIndex, 0, FMath::Max(0, Slots.Num() - 1));
		MyWidgetSwitcher->SetActiveWidgetIndex(SafeIndex);
		if (OnActiveWidgetChanged.IsBound())
		{
			OnActiveWidgetChanged.Broadcast(SafeIndex);
		}
		ActiveWidgetIndex = SafeIndex;
	}
}
