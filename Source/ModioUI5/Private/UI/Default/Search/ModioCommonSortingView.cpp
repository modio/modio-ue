/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Default/Search/ModioCommonSortingView.h"

#include "Algo/Count.h"
#include "Types/ModioFilterParams.h"
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBoxStyle.h"

UModioCommonSortingView::UModioCommonSortingView()
	: CurrentSortFieldType(EModioSortFieldType::DownloadsToday),
	  CurrentSortDirection(EModioSortDirection::Ascending)
{
	bAutoFocusOnActivation = false;
}

bool UModioCommonSortingView::GetSortFieldTypeAndDirection_Implementation(EModioSortFieldType& OutSortFieldType, EModioSortDirection& OutSortDirection) const
{
	OutSortFieldType = CurrentSortFieldType;
	OutSortDirection = CurrentSortDirection;
	return true;
}

void UModioCommonSortingView::ResetSorting()
{
	if (SortingRadioButtonsContainer)
	{
		for (int32 Index = 0; Index < SortingRadioButtonsContainer->GetChildrenCount(); ++Index)
		{
			if (UModioCommonCheckBox* RadioButton = Cast<UModioCommonCheckBox>(SortingRadioButtonsContainer->GetChildAt(Index)))
			{
				RadioButton->SetIsChecked(Index == 0);
			}
		}
	}
}

void UModioCommonSortingView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!SortingRadioButtonsContainer)
	{
		return;
	}

	if (!SortingRadioButtonsContainer->HasAnyChildren())
	{
		AddSortingRadioButton("Trending", EModioSortFieldType::DownloadsToday, EModioSortDirection::Ascending, true);
		AddSortingRadioButton("Highest Rated", EModioSortFieldType::Rating, EModioSortDirection::Descending);
		AddSortingRadioButton("Most Popular", EModioSortFieldType::DownloadsTotal, EModioSortDirection::Descending);
		AddSortingRadioButton("Most Subscribers", EModioSortFieldType::SubscriberCount, EModioSortDirection::Descending);
		AddSortingRadioButton("Recently Added", EModioSortFieldType::DateMarkedLive, EModioSortDirection::Descending);
		AddSortingRadioButton("Last Updated", EModioSortFieldType::DateUpdated, EModioSortDirection::Descending);
	}
}

UWidget* UModioCommonSortingView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (SortingRadioButtonsContainer)
	{
		if (UModioCommonCheckBox* CheckedRadioButton = Cast<UModioCommonCheckBox>(SortingRadioButtonsContainer->GetChildAt(0)))
		{
			return CheckedRadioButton;
		}
	}
	return nullptr;
}

void UModioCommonSortingView::UpdateSelection_Implementation(UModioCommonCheckBox* CheckedRadioButton, bool bIsChecked, EModioSortFieldType ByField, EModioSortDirection ByDirection)
{
	if (!CheckedRadioButton)
	{
		return;
	}
	
	if (!bIsChecked && GetNumSortingOptionsChecked() == 0)
	{
		CheckedRadioButton->SetIsChecked(true);
	}

	if (bIsChecked)
	{
		if (SortingRadioButtonsContainer)
		{
			for (UWidget* ChildWidget : SortingRadioButtonsContainer->GetAllChildren())
			{
				if (UModioCommonCheckBox* RadioButton = Cast<UModioCommonCheckBox>(ChildWidget))
				{
					if (RadioButton != CheckedRadioButton)
					{
						RadioButton->SetIsChecked(false);
					}
				}
			}
		}
		
		CurrentSortFieldType = ByField;
		CurrentSortDirection = ByDirection;
	}
}

void UModioCommonSortingView::AddSortingRadioButton_Implementation(const FString& LabelText, EModioSortFieldType ByField, EModioSortDirection ByDirection, bool bCheckedByDefault)
{
	UModioCommonCheckBox* RadioButton = NewObject<UModioCommonCheckBox>(this);
	if (SortingRadioButtonStyle)
	{
		RadioButton->SetStyle(SortingRadioButtonStyle);
	}
	RadioButton->SetLabel(FText::FromString(LabelText));
	RadioButton->OnModioCommonCheckBoxStateChanged.AddWeakLambda(this, [this, RadioButton, ByField, ByDirection](bool bIsChecked) {
		UpdateSelection(RadioButton, bIsChecked, ByField, ByDirection);
	});
	
	if (SortingRadioButtonsContainer)
	{
		SortingRadioButtonsContainer->AddChild(RadioButton);
	}
	
	if (bCheckedByDefault)
	{
		RadioButton->SetIsChecked(true);
	}
}

int32 UModioCommonSortingView::GetNumSortingOptionsChecked_Implementation() const
{
	if (!SortingRadioButtonsContainer)
	{
		return 0;
	}

	const TArray<UWidget*> Children = SortingRadioButtonsContainer->GetAllChildren();
	return Algo::CountIf(Children, [](UWidget* Child) {
		UModioCommonCheckBox* RadioButton = Cast<UModioCommonCheckBox>(Child);
		return RadioButton && RadioButton->IsChecked();
	});
}
