#pragma once

#include "UI/Styles/ModioTableRowStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/STableRow.h"

/// @brief Base class for a STableRow with mod.io-specific styling support
/// can either be used as-is or subclassed further.

template<typename ItemType>
class MODIOUI_API SModioTableRowBase : public STableRow<ItemType>
{
public:
	SLATE_BEGIN_ARGS(SModioTableRowBase<ItemType>) 
		:_Style(FName("DefaultTableRowStyle"))
	{}

	SLATE_ARGUMENT(FModioUIStyleRef, Style)
	SLATE_DEFAULT_SLOT(typename SModioTableRowBase<ItemType>::FArguments, Content)
	SLATE_ARGUMENT(bool, ShowSelection)
	SLATE_ARGUMENT(FMargin, Padding)
	SLATE_END_ARGS()

	void Construct(const typename SModioTableRowBase<ItemType>::FArguments& InArgs,
				   const TSharedRef<STableViewBase>& InOwnerTableView)
	{
		Style = InArgs._Style;

		STableRow<ItemType>::Construct(typename STableRow<ItemType>::FArguments().Padding(InArgs._Padding).ShowSelection(
										   InArgs._ShowSelection)[InArgs._Content.Widget],
									   InOwnerTableView);
		// override the delegates for text color etc here

#if UE_VERSION_NEWER_THAN(5, 0, 0)
		STableRow<ItemType>::SetBorderImage(
			TAttribute<const FSlateBrush*>(
			this, &SModioTableRowBase::GetBackgroundColor));
		STableRow<ItemType>::SetForegroundColor(TAttribute<FSlateColor>(this, &SModioTableRowBase::GetTextColor));
#else
		STableRow<ItemType>::BorderImage =
			FInvalidatableBrushAttribute(TAttribute<const FSlateBrush*>(this, &SModioTableRowBase::GetBackgroundColor));
		STableRow<ItemType>::ForegroundColor = TAttribute<FSlateColor>(this, &SModioTableRowBase::GetTextColor);
#endif
	}

	void SetStyle(FModioUIStyleRef NewStyle)
	{
		Style = NewStyle;
	}

protected:
	const FSlateBrush* GetBackgroundColor() const
	{
		const FModioTableRowStyle* ResolvedStyle = Style.FindStyle<FModioTableRowStyle>();
		
		if (!ResolvedStyle)
		{
			return FStyleDefaults::GetNoBrush();
		}

		if (STableRow<ItemType>::IsHovered())
		{
			return ResolvedStyle->BackgroundHoverColor.GetBrush();
		}
		if (STableRow<ItemType>::IsSelected())
		{
			return ResolvedStyle->BackgroundSelectedColor.GetBrush();
		}
		return ResolvedStyle->BackgroundColor.GetBrush();
	}
	FSlateColor GetTextColor() const
	{
		const FModioTableRowStyle* ResolvedStyle = Style.FindStyle<FModioTableRowStyle>();

		if (!ResolvedStyle)
		{
			return FSlateColor(FLinearColor::White);
		}

		if (STableRow<ItemType>::IsHovered())
		{
			return ResolvedStyle->TextHoverColor;
		}
		if (STableRow<ItemType>::IsSelected())
		{
			return ResolvedStyle->TextSelectedColor;
		}
		return ResolvedStyle->TextColor;
	}
	FModioUIStyleRef Style;
};
