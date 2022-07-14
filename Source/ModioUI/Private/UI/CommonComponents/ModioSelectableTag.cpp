// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioSelectableTag.h"
#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "UI/CommonComponents/ModioListViewString.h"

void UModioSelectableTag::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TagSelectedCheckbox)
	{
		TagSelectedCheckbox->OnCheckStateChanged.AddDynamic(this, &UModioSelectableTag::OnCheckboxStateChanged);
	}
}

void UModioSelectableTag::OnCheckboxStateChanged(bool bIsChecked)
{
	if (UListViewBase* BaseListView = GetOwningListView())
	{
		if (UListView* MyListView = Cast<UListView>(BaseListView))
		{
			MyListView->SetItemSelection(*MyListView->ItemFromEntryWidget(*this), bIsChecked,
										 ESelectInfo::OnMouseClick);
		}
		else if (UModioListViewString* MyListViewString = Cast<UModioListViewString>(BaseListView))
		{
			MyListViewString->SetItemSelection(*MyListViewString->ItemFromEntryWidget(*this), bIsChecked,
										 ESelectInfo::OnMouseClick);
		}
	}
}
