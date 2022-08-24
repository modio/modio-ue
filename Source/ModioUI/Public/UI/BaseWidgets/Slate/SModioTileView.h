#pragma once
#include "UObject/Object.h"
//#include "Widgets/Views/SListPanel.h"
#include "Blueprint/IUserListEntry.h"
#include "Layout/ArrangedChildren.h"
#include "Widgets/Views/STileView.h"

template<typename ItemType = UObject*>
class MODIOUI_API SModioTileView : public STileView<ItemType>
{
protected:
	bool bDontShowPartialItems = true;
	float CalculatedItemSpacing = 0;
	int32 CalculatedNumItems = 0;
	bool bCenterPanelItems = true;
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		return STileView<ItemType>::OnFocusReceived(MyGeometry, InFocusEvent);
	}

public:
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry,
								   FArrangedChildren& ArrangedChildren) const override
	{
		if (bCenterPanelItems)
		{
			STileView<ItemType>::OnArrangeChildren(
				AllottedGeometry.MakeChild(
					AllottedGeometry.GetLocalSize() /* - FVector2D(CalculatedItemSpacing * 2, 0)*/,
					FSlateLayoutTransform(this->Orientation == EOrientation::Orient_Horizontal
											  ? FVector2D(CalculatedItemSpacing, 0)
											  : FVector2D(0, CalculatedItemSpacing))),
				ArrangedChildren);
		}
		else
		{
			STileView<ItemType>::OnArrangeChildren(AllottedGeometry, ArrangedChildren);
		}
		// None of the below will work either because there is only a single child (the panel)
		/*FArrangedChildren::FArrangedWidgetArray MutableArrangedChildren = ArrangedChildren.GetInternalArray();
		MutableArrangedChildren.Sort([](FArrangedWidget& First, FArrangedWidget& Second) {
			ITableRow* FirstEntry = dynamic_cast<ITableRow*>(&First.Widget.Get());
			ITableRow* SecondEntry = dynamic_cast<ITableRow*>(&Second.Widget.Get());

			if (FirstEntry && SecondEntry)
			{
				// If the first item is selected
				if (FirstEntry->IsItemSelected())
				{
					// And the second item isn't selected
					if (!SecondEntry->IsItemSelected())
					{
						// Sort the first item higher than the second
						return true;
					}
				}
			}
			// if one or other or both aren't IUserListEntries, retain order

			return false;
		});
		FArrangedChildren NewArrangedChildren =
			FArrangedChildren(ArrangedChildren.GetFilter(), ArrangedChildren.Allows3DWidgets());
		NewArrangedChildren.GetInternalArray() = MutableArrangedChildren;
		ArrangedChildren = NewArrangedChildren;*/
	};

	virtual FNavigationReply OnNavigation(const FGeometry& MyGeometry,
										  const FNavigationEvent& InNavigationEvent) override
	{
		FNavigationReply NavReply = STileView<ItemType>::OnNavigation(MyGeometry, InNavigationEvent);
		// if NavReply == Escape, then check if we're done scrolling and don't permit navigation
		if (NavReply.GetBoundaryRule() == EUINavigationRule::Escape)
		{
			if (!FMath::IsNearlyEqual(this->CurrentScrollOffset, this->GetTargetScrollOffset()))
			{
				return FNavigationReply::Stop();
			}
		}
		return NavReply;
	}
	virtual typename STileView<ItemType>::FOnMouseButtonClick& GetOnMouseClicked()
	{
		return SListView<ItemType>::OnClick;
	}

	void SetAllowPartialItems(bool bNewAllowPartialItems)
	{
		bDontShowPartialItems = !bNewAllowPartialItems;
	}
	void SetCenterBasedOnPanelItems(bool bNewCenterBasedOnPanelItems)
	{
		bCenterPanelItems = bNewCenterBasedOnPanelItems;
	}
	virtual void SetConsumeMouseWheel(EConsumeMouseWheel NewValue)
	{
		this->ConsumeMouseWheel = NewValue;
	}

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (this->ConsumeMouseWheel == EConsumeMouseWheel::Never)
		{
			return FReply::Unhandled();
		}
		return STileView<ItemType>::OnMouseWheel(MyGeometry, MouseEvent);
	}

	virtual STableViewBase::FReGenerateResults ReGenerateItems(const FGeometry& MyGeometry) override
	{
		// Clear all the items from our panel. We will re-add them in the correct order momentarily.
		this->ClearWidgets();

		const TArray<ItemType>* SourceItems = this->ItemsSource;
		if (SourceItems && SourceItems->Num() > 0)
		{
			// Item width and height is constant by design.
			FTableViewDimensions TileDimensions = this->GetTileDimensions();
			FTableViewDimensions AllottedDimensions(this->Orientation, MyGeometry.GetLocalSize());

			const int32 NumItems = SourceItems->Num();
			const int32 NumItemsPerLine = this->GetNumItemsPerLine();
			const int32 NumItemsPaddedToFillLastLine =
				(NumItems % NumItemsPerLine != 0) ? NumItems + NumItemsPerLine - NumItems % NumItemsPerLine : NumItems;

			double LinesPerScreen = AllottedDimensions.ScrollAxis / TileDimensions.ScrollAxis;
			if (bDontShowPartialItems)
			{
				LinesPerScreen = FMath::FloorToDouble(LinesPerScreen);
			}
			const double EndOfListOffset = NumItemsPaddedToFillLastLine - NumItemsPerLine * LinesPerScreen;
			double ClampedScrollOffset = FMath::Clamp(this->CurrentScrollOffset, 0.0, EndOfListOffset);
			const float LayoutScaleMultiplier = MyGeometry.GetAccumulatedLayoutTransform().GetScale();

			// Once we run out of vertical and horizontal space, we stop generating widgets.
			FTableViewDimensions DimensionsUsedSoFar(this->Orientation);

			// Index of the item at which we start generating based on how far scrolled down we are
			int32 StartIndex =
				FMath::Max(0, FMath::FloorToInt(ClampedScrollOffset / NumItemsPerLine) * NumItemsPerLine);

			// Let the WidgetGenerator know that we are starting a pass so that it can keep track of data items and
			// widgets.
			this->WidgetGenerator.OnBeginGenerationPass();

			// Actually generate the widgets.
			bool bIsAtEndOfList = false;
			bool bHasFilledAvailableArea = false;
			bool bNewLine = true;
			bool bFirstLine = true;
			double NumLinesShownOnScreen = 0;
			for (int32 ItemIndex = StartIndex; !bHasFilledAvailableArea && ItemIndex < NumItems; ++ItemIndex)
			{
				const ItemType& CurItem = (*SourceItems)[ItemIndex];

				if (bNewLine)
				{
					bNewLine = false;

					float LineFraction = 1.f;
					if (bFirstLine)
					{
						bFirstLine = false;
						LineFraction -= FMath::Fractional(ClampedScrollOffset / NumItemsPerLine);
					}

					DimensionsUsedSoFar.ScrollAxis += TileDimensions.ScrollAxis * LineFraction;

					if (DimensionsUsedSoFar.ScrollAxis > AllottedDimensions.ScrollAxis)
					{
						NumLinesShownOnScreen +=
							FMath::Max(1.0f - ((DimensionsUsedSoFar.ScrollAxis - AllottedDimensions.ScrollAxis) /
											   TileDimensions.ScrollAxis),
									   0.0f);
					}
					else
					{
						NumLinesShownOnScreen += LineFraction;
					}
				}

				SListView<ItemType>::GenerateWidgetForItem(CurItem, ItemIndex, StartIndex, LayoutScaleMultiplier);

				// The widget used up some of the available space for the current line
				DimensionsUsedSoFar.LineAxis += TileDimensions.LineAxis;

				bIsAtEndOfList = ItemIndex >= NumItems - 1;

				if (DimensionsUsedSoFar.LineAxis + TileDimensions.LineAxis > AllottedDimensions.LineAxis)
				{
					// A new line of widgets was completed - time to start another one
					DimensionsUsedSoFar.LineAxis = 0;
					bNewLine = true;
				}

				if (bIsAtEndOfList || bNewLine)
				{
					// We've filled all the available area when we've finished a line that's partially clipped by the
					// end of the view
					const float FloatPrecisionOffset = 0.001f;
					bHasFilledAvailableArea =
						DimensionsUsedSoFar.ScrollAxis > AllottedDimensions.ScrollAxis + FloatPrecisionOffset;
					if (!bHasFilledAvailableArea && bDontShowPartialItems)
					{
						bHasFilledAvailableArea = (DimensionsUsedSoFar.ScrollAxis + TileDimensions.ScrollAxis) >
												  (AllottedDimensions.ScrollAxis + FloatPrecisionOffset);
						if (bHasFilledAvailableArea)
						{
							CalculatedItemSpacing =
								(AllottedDimensions.ScrollAxis - DimensionsUsedSoFar.ScrollAxis) / 2;
							CalculatedNumItems = ItemIndex;
						}
					}
				}
			}

			// We have completed the generation pass. The WidgetGenerator will clean up unused Widgets.
			this->WidgetGenerator.OnEndGenerationPass();
			UE_LOG(LogTemp, Display, TEXT("Generated %g widgets"), NumLinesShownOnScreen);
			const float TotalGeneratedLineAxisSize =
				FMath::FloorToFloat(NumLinesShownOnScreen) * TileDimensions.ScrollAxis;
			return STableViewBase::FReGenerateResults(
				ClampedScrollOffset, TotalGeneratedLineAxisSize,
				(/* bDontShowPartialItems ? CalculatedNumItems : */ FMath::FloorToFloat(NumLinesShownOnScreen)),
				bIsAtEndOfList && !bHasFilledAvailableArea);
		}

		return STableViewBase::FReGenerateResults(0, 0, 0, false);
	}
	PRAGMA_DISABLE_OPTIMIZATION
	virtual typename SListView<ItemType>::EScrollIntoViewResult ScrollIntoView(
		const FGeometry& ListViewGeometry) override
	{
		if (TListTypeTraits<ItemType>::IsPtrValid(this->ItemToScrollIntoView) && this->ItemsSource != nullptr)
		{
			const int32 IndexOfItem = this->ItemsSource->Find(
				TListTypeTraits<ItemType>::NullableItemTypeConvertToItemType(this->ItemToScrollIntoView));
			if (IndexOfItem != INDEX_NONE)
			{
				float NumLinesInView =
					FTableViewDimensions(this->Orientation, ListViewGeometry.GetLocalSize()).ScrollAxis /
					this->GetTileDimensions().ScrollAxis;

				float NumLiveWidgets = this->GetNumLiveWidgets();
				if (NumLiveWidgets == 0 && this->IsPendingRefresh())
				{
					// Use the last number of widgets on screen to estimate if we actually need to scroll.
					NumLiveWidgets = this->LastGenerateResults.ExactNumLinesOnScreen;

					// If we still don't have any widgets, we're not in a situation where we can scroll an item into
					// view (probably as nothing has been generated yet), so we'll defer this again until the next frame
					if (NumLiveWidgets == 0)
					{
						return SListView<ItemType>::EScrollIntoViewResult::Deferred;
					}
				}

				this->EndInertialScrolling();

				// Only scroll the item into view if it's not already in the visible range
				const int32 NumItemsPerLine = this->GetNumItemsPerLine();
				const double ScrollLineOffset = this->GetTargetScrollOffset() / NumItemsPerLine;
				const int32 LineOfItem = FMath::FloorToInt((float)IndexOfItem / (float)NumItemsPerLine);
				const int32 NumFullLinesInView =
					FMath::FloorToInt(ScrollLineOffset + NumLinesInView) - FMath::CeilToInt(ScrollLineOffset);

				const double MinDisplayedLine = this->bNavigateOnScrollIntoView ? FMath::FloorToDouble(ScrollLineOffset)
																				: FMath::CeilToDouble(ScrollLineOffset);
				double MaxDisplayedLine = this->bNavigateOnScrollIntoView
											  ? FMath::CeilToDouble(ScrollLineOffset + NumFullLinesInView)
											  : FMath::FloorToDouble(ScrollLineOffset + NumFullLinesInView);

				MaxDisplayedLine = MaxDisplayedLine - 1;

				if (LineOfItem < MinDisplayedLine ||
					LineOfItem > MaxDisplayedLine /*|| (bDontShowPartialItems&& (LineOfItem == MaxDisplayedLine))*/)
				{
					// Set the line with the item at the beginning of the view area
					float NewLineOffset = LineOfItem;
					
					if (LineOfItem > MaxDisplayedLine) {
						NewLineOffset =
							LineOfItem - NumFullLinesInView + 1.f +
							(this->FixedLineScrollOffset.IsSet() ? 0.f : this->NavigationScrollOffset);
					}
					//// Center the line in the view area
					//NewLineOffset -=
					//	FMath::FloorToFloat((bDontShowPartialItems ? NumFullLinesInView : NumLinesInView) * 0.5f);

					// Convert the line offset into an item offset
					double NewScrollOffset = NewLineOffset * NumItemsPerLine;
					// And clamp the scroll offset within the allowed limits
					NewScrollOffset =
						FMath::Clamp(NewScrollOffset, 0.,
						(double) (this->GetNumItemsBeingObserved() - NumItemsPerLine * NumFullLinesInView));

					this->SetScrollOffset(NewScrollOffset);
				}
				else if (this->bNavigateOnScrollIntoView)
				{
					// Make sure the line containing the existing entry for this item is fully in view
					if (LineOfItem == MinDisplayedLine)
					{
						// This line is clipped at the top/left, so set it as the new offset
						UE_LOG(LogTemp, Display, TEXT("left Set scroll offset %g"),
							   LineOfItem * NumItemsPerLine - (this->FixedLineScrollOffset.IsSet() /*&& LineOfItem > 0*/
																   ? 0.f
																   : this->NavigationScrollOffset));
						this->SetScrollOffset(LineOfItem * NumItemsPerLine -
											  (this->FixedLineScrollOffset.IsSet() /*&& LineOfItem > 0*/
												   ? 0.f
												   : this->NavigationScrollOffset));
					}
					else if (LineOfItem == MaxDisplayedLine)
					{
						// This line is clipped at the end, so we need to advance just enough to bring it fully into
						// view Since all tiles are required to be of the same size, this is straightforward
						const float NewLineOffset =
							LineOfItem - NumLinesInView + 1.f +
							(this->FixedLineScrollOffset.IsSet() ? 0.f : this->NavigationScrollOffset);

						UE_LOG(LogTemp, Display, TEXT("right Set scroll offset %g"),
							   FMath::CeilToFloat(FMath::CeilToFloat(NewLineOffset) * NumItemsPerLine));
						this->SetScrollOffset(FMath::CeilToFloat(FMath::CeilToFloat(NewLineOffset) * NumItemsPerLine));
					}
				}

				this->RequestListRefresh();

				this->ItemToNotifyWhenInView = this->ItemToScrollIntoView;
			}

			TListTypeTraits<ItemType>::ResetPtr(this->ItemToScrollIntoView);
		}

		if (this->bEnableAnimatedScrolling && TListTypeTraits<ItemType>::IsPtrValid(this->ItemToNotifyWhenInView))
		{
			// When we have a target item we're shooting for, we haven't succeeded with the scroll until a widget for it
			// exists
			const bool bHasWidgetForItem =
				this->WidgetFromItem(
						TListTypeTraits<ItemType>::NullableItemTypeConvertToItemType(this->ItemToNotifyWhenInView))
					.IsValid();
			return (bHasWidgetForItem &&
					FMath::IsNearlyEqual(this->CurrentScrollOffset, this->GetTargetScrollOffset()))
					   ? SListView<ItemType>::EScrollIntoViewResult::Success
					   : SListView<ItemType>::EScrollIntoViewResult::Deferred;
		}
		return SListView<ItemType>::EScrollIntoViewResult::Success;
	}
	PRAGMA_ENABLE_OPTIMIZATION
};
