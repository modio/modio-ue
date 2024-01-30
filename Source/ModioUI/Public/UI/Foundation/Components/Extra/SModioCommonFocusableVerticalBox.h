/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "Layout/WidgetPath.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"

#pragma once

/**
 * Special version of SVerticalBox that supports keyboard focus (forwards focus to the first child that supports it)
 */
class MODIOUI_API SModioCommonFocusableVerticalBox : public SVerticalBox
{
	/**
	 * Finds all children of a widget that match a predicate
	 * @param ParentWidget The widget to search
	 * @param ComparePredicate The predicate to match
	 * @return All children of the widget that match the predicate
	 */
	static TArray<TSharedRef<SWidget>> FindNestedChildrenIf(TSharedRef<SWidget> ParentWidget, TFunctionRef<bool(TSharedRef<const SWidget>)> ComparePredicate)
	{
		TArray<TSharedRef<SWidget>> FoundChildren;
		ParentWidget->GetChildren()->ForEachWidget([&ComparePredicate, &FoundChildren](SWidget& ChildWidget) {
			TSharedRef<SWidget> ChildWidgetRef = SharedThis(&ChildWidget);
			if (ComparePredicate(ChildWidgetRef))
			{
				FoundChildren.Add(ChildWidgetRef);
			}
			FoundChildren.Append(FindNestedChildrenIf(ChildWidgetRef, ComparePredicate));
		});
		return FoundChildren;
	}
	
	virtual bool SupportsKeyboardFocus() const override
	{
		const bool bAnyChildHasKeyboardFocus = FindNestedChildrenIf(SharedThis(const_cast<SModioCommonFocusableVerticalBox*>(this)), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->SupportsKeyboardFocus();
		}).Num() > 0;
		return bAnyChildHasKeyboardFocus;
	}

	virtual bool HasKeyboardFocus() const override
	{
		const bool bAnyChildHasKeyboardFocus = FindNestedChildrenIf(SharedThis(const_cast<SModioCommonFocusableVerticalBox*>(this)), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->HasKeyboardFocus();
		}).Num() > 0;
		return bAnyChildHasKeyboardFocus;
	}
	
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		TArray<TSharedRef<SWidget>> FoundChildren = FindNestedChildrenIf(SharedThis(this), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->SupportsKeyboardFocus();
		});

		for (TSharedRef<SWidget> ChildWidget : FoundChildren)
		{
			FWidgetPath WidgetToFocusPath;
			FSlateApplication::Get().GeneratePathToWidgetUnchecked(ChildWidget, WidgetToFocusPath);
			FSlateApplication::Get().SetKeyboardFocus(WidgetToFocusPath, InFocusEvent.GetCause());
		}
		return FReply::Handled();
	}
};
