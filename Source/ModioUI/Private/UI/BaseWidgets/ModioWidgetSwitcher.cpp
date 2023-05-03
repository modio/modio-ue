/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioWidgetSwitcher.h"
#include "Framework/Application/SlateApplication.h"

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
		if (MyWidgetSwitcher && MyWidgetSwitcher->GetActiveWidget()) 
		{
			FSlateApplication::Get().SetUserFocus(0, MyWidgetSwitcher->GetActiveWidget());
		}
		
		if (OnActiveWidgetChanged.IsBound())
		{
			OnActiveWidgetChanged.Broadcast(SafeIndex);
		}
		ActiveWidgetIndex = SafeIndex;
	}
}
