#pragma once

#include "Core/ModioUIHelpers.h"
#include "Layout/LayoutUtils.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"

class MODIOUI_API SModioBox : public SBox
{
protected:
	TAttribute<FOptionalSize> ModioBoxMinAspectRatio;

	TAttribute<FOptionalSize> ModioBoxMaxAspectRatio;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
						  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
						  bool bParentEnabled) const override
	{
		return SBox::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							 bParentEnabled);
	}

public:
	using FArguments = SBox::FArguments;
	void Construct(const FArguments& InArgs)
	{
		SBox::Construct(InArgs);
		ModioBoxMaxAspectRatio = InArgs._MaxAspectRatio;
		ModioBoxMinAspectRatio = InArgs._MinAspectRatio;
	}

protected:
	void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
	{
		const EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();
		if (ArrangedChildren.Accepts(ChildVisibility))
		{
			const FOptionalSize CurrentMinAspectRatio = ModioBoxMinAspectRatio.Get();
			const FOptionalSize CurrentMaxAspectRatio = ModioBoxMaxAspectRatio.Get();

			const FMargin SlotPadding = ModioUIHelpers::GetPadding(ChildSlot);

			bool bAlignChildren = true;

			AlignmentArrangeResult XAlignmentResult(0, 0);
			AlignmentArrangeResult YAlignmentResult(0, 0);

			if (CurrentMaxAspectRatio.IsSet() || CurrentMinAspectRatio.IsSet())
			{
				float CurrentWidth = FMath::Min(AllottedGeometry.Size.X, ChildSlot.GetWidget()->GetDesiredSize().X);
				float CurrentHeight = FMath::Min(AllottedGeometry.Size.Y, ChildSlot.GetWidget()->GetDesiredSize().Y);

				float MinAspectRatioWidth = CurrentMinAspectRatio.IsSet() ? CurrentMinAspectRatio.Get() : 0;
				float MaxAspectRatioWidth = CurrentMaxAspectRatio.IsSet() ? CurrentMaxAspectRatio.Get() : 0;
				if (CurrentHeight > 0 && CurrentWidth > 0)
				{
					const float CurrentRatioWidth =
						(AllottedGeometry.GetLocalSize().X / AllottedGeometry.GetLocalSize().Y);

					bool bFitMaxRatio = (CurrentRatioWidth > MaxAspectRatioWidth && MaxAspectRatioWidth != 0);
					bool bFitMinRatio = (CurrentRatioWidth < MinAspectRatioWidth && MinAspectRatioWidth != 0);
					if (bFitMaxRatio || bFitMinRatio)
					{
						XAlignmentResult =
							AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ChildSlot, SlotPadding);
						YAlignmentResult =
							AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ChildSlot, SlotPadding);

						float NewWidth;
						float NewHeight;

						if (bFitMaxRatio)
						{
							const float MaxAspectRatioHeight = 1.0f / MaxAspectRatioWidth;
							NewWidth = MaxAspectRatioWidth * XAlignmentResult.Size;
							NewHeight = MaxAspectRatioHeight * NewWidth;
						}
						else
						{
							const float MinAspectRatioHeight = 1.0f / MinAspectRatioWidth;
							NewWidth = MinAspectRatioWidth * XAlignmentResult.Size;
							NewHeight = MinAspectRatioHeight * NewWidth;
						}

						const float MaxWidth =
							AllottedGeometry.Size.X - SlotPadding.GetTotalSpaceAlong<Orient_Horizontal>();
						const float MaxHeight =
							AllottedGeometry.Size.Y - SlotPadding.GetTotalSpaceAlong<Orient_Vertical>();

						if (NewWidth > MaxWidth)
						{
							float Scale = MaxWidth / NewWidth;
							NewWidth *= Scale;
							NewHeight *= Scale;
						}

						if (NewHeight > MaxHeight)
						{
							float Scale = MaxHeight / NewHeight;
							NewWidth *= Scale;
							NewHeight *= Scale;
						}

						XAlignmentResult.Size = NewWidth;
						YAlignmentResult.Size = NewHeight;
						bAlignChildren = false;
					}
				}
			}

			if (bAlignChildren)
			{
				XAlignmentResult =
					AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ChildSlot, SlotPadding);
				YAlignmentResult =
					AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ChildSlot, SlotPadding);
			}

			const float AlignedSizeX = XAlignmentResult.Size;
			const float AlignedSizeY = YAlignmentResult.Size;

			if (ModioUIHelpers::GetVerticalAlignment(ChildSlot) == VAlign_Center)
			{
				YAlignmentResult.Offset = (AllottedGeometry.GetLocalSize().Y - AlignedSizeY) / 2;
			}
			if (ModioUIHelpers::GetHorizontalAlignment(ChildSlot) == HAlign_Center)
			{
				XAlignmentResult.Offset = (AllottedGeometry.GetLocalSize().X - AlignedSizeX) / 2;
			}

			ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
				ChildSlot.GetWidget(), FVector2D(XAlignmentResult.Offset, YAlignmentResult.Offset),
				FVector2D(AlignedSizeX, AlignedSizeY)));
		}
	}
};