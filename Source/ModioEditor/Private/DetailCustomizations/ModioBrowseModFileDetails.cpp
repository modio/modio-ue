/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "../../Public/DetailCustomizations/ModioBrowseModFileDetails.h"
#include "../../Public/Widgets/SModioEditorWindowCompoundWidget.h"
#include <DetailCategoryBuilder.h>
#include <DetailCustomizations/SModFileRow.h>
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>
#include <Objects/ModioBrowseModFileCollectionObject.h>
#include <Objects/ModioBrowseModFileObject.h>
#include <WindowManager.h>

TSharedRef<IDetailCustomization> ModioBrowseModFileDetails::MakeInstance()
{
	return MakeShareable(new ModioBrowseModFileDetails);
}

void ModioBrowseModFileDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Target = Cast<UModioBrowseModFileCollectionObject>(Object);
			if (Target)
			{
				break;
			}
		}
	}
	DrawBrowseModFile(DetailBuilder);
}

void ModioBrowseModFileDetails::DrawBrowseModFile(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> ItemsProperty = DetailBuilder.GetProperty("Items");
	DetailBuilder.HideProperty(ItemsProperty);
	
	for (int32 i = 0; i < Target->Items.Num(); i++)
	{
		Source.Add(Target->Items[i].ToSharedPtr());
	}
	
	IDetailCategoryBuilder& ModPropertiesCategory = DetailBuilder.EditCategory("Modio Browse Mod File Params", FText::FromString("Modfile"));
	ModPropertiesCategory.AddCustomRow(FText::FromString(""))
	.WholeRowContent()
	[
		SAssignNew(ListView, SListView<TSharedPtr<FModioBrowseModFileStruct>>)
		.ItemHeight(50)
		.SelectionMode(ESelectionMode::Multi)
		.ListItemsSource(&Source)
		.OnGenerateRow_Lambda([this](TSharedPtr<FModioBrowseModFileStruct> Item, const TSharedRef<STableViewBase>& OwnerTable)->TSharedRef<ITableRow>
		{
			return SNew(SModFileRow, OwnerTable).Item(Item);
		})
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

			+ SHeaderRow::Column("Platform")
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Platform"))
				]
			]

			+ SHeaderRow::Column("Version")
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Version"))
				]
			]

			+ SHeaderRow::Column("Status")
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(FText::FromString("Status"))
				]
			]
		)
	];
}
