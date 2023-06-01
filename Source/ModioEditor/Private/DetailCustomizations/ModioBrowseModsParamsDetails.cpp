/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "../../Public/DetailCustomizations/ModioBrowseModsParamsDetails.h"
#include "Engine/StaticMesh.h"
#include "../../Public/Objects/ModioBrowseModsObject.h"
#include "PropertyEditing.h"
#include <Framework/Application/SlateApplication.h>
#include <WindowManager.h>
#include <PropertyCustomizationHelpers.h>
#include <Widgets/Input/SSearchBox.h>

#define LOCTEXT_NAMESPACE "ModioBrowseModsParamsDetails"

TSharedRef<IDetailCustomization> ModioBrowseModsParamsDetails::MakeInstance()
{
	return MakeShareable(new ModioBrowseModsParamsDetails);
}

void ModioBrowseModsParamsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Target = Cast<UModioBrowseModsObject>(Object);
			if (Target)
			{
				break;
			}
		}
	}
	DrawEditMod(DetailBuilder);
}

void ModioBrowseModsParamsDetails::DrawEditMod(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> ItemsProperty = DetailBuilder.GetProperty("Items");
	DetailBuilder.HideProperty(ItemsProperty);

	OriginalSource.Empty();
	for (int32 i = 0; i < Target->Items.Num(); i++)
	{
		TSharedPtr<FModioModInfo> Element = MakeShared<FModioModInfo>(Target->Items[i]);
		OriginalSource.Add(Element);
	}
	Source = OriginalSource;

	IDetailCategoryBuilder& ModPropertiesCategory = DetailBuilder.EditCategory("Browse Mods Params", FText::FromString("Browse Mods"));

	ModPropertiesCategory.AddCustomRow(FText::FromString(""))
	.WholeRowContent()
	[
		SAssignNew(SearchBox, SSearchBox)
		.OnTextChanged_Lambda([this](const FText& NewText)
		{
			if(NewText.IsEmpty())
			{
				Source = OriginalSource;
			}

			SearchFilter.Empty();
			for (int32 i = 0; i < Target->Items.Num(); i++)
			{
				TSharedPtr<FModioModInfo> Element = MakeShared<FModioModInfo>(Target->Items[i]);
				if (Element->ProfileName.Contains(NewText.ToString()))
				{
					SearchFilter.Add(Element);
					Source = SearchFilter;
					ListView->RequestListRefresh();
				}
			}
		})
	];

	ModPropertiesCategory.AddCustomRow(FText::FromString(""))
	.WholeRowContent()
	[
		SAssignNew(ListView, SListView<TSharedPtr<FModioModInfo>>)
		.ItemHeight(50)
		.ListItemsSource(&Source)
		.HeaderRow
		(
			SNew(SHeaderRow)

			+ SHeaderRow::Column("Name")
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.Padding(5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Name"))
				]
			]

			+ SHeaderRow::Column("Description")
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Description"))
				]
			]
		)
		.OnGenerateRow_Lambda([this](TSharedPtr<FModioModInfo> Item, const TSharedRef<STableViewBase>& OwnerTable)
		{
			return SNew(STableRow< TSharedPtr<FModioModInfo> >, OwnerTable)
				   .Padding(2.0f)
				   [
					   SNew(SHorizontalBox)

					   +SHorizontalBox::Slot()
					   [
							SNew(STextBlock).Text(FText::FromString(*Item.Get()->ProfileName))
					   ]
						
						+SHorizontalBox::Slot()
					   [
							SNew(STextBlock).Text(FText::FromString(*Item.Get()->ProfileDescription))
					   ]
				   ];
		})
		.OnSelectionChanged_Lambda([this](TSharedPtr<FModioModInfo> Item, ESelectInfo::Type SelectInfoType)
		{
			if (Item.IsValid())
			{
				Target->SelectedItem = Item;
			}
		})
	];
}

#undef LOCTEXT_NAMESPACE