/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "DetailCustomizations/ModioCreateModFileParamsDetails.h"
#include "DetailLayoutBuilder.h"
#include "Objects/ModioCreateNewModFileParamsObject.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "WindowManager.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> ModioCreateModFileParamsDetails::MakeInstance()
{
	return MakeShareable(new ModioCreateModFileParamsDetails);
}

void ModioCreateModFileParamsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Target = Cast<UModioCreateNewModFileParamsObject>(Object);
			if (Target)
			{
				break;
			}
		}
	}
	DrawCreateNewModFile(DetailBuilder);
}

void ModioCreateModFileParamsDetails::DrawCreateNewModFile(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> NewModFile = DetailBuilder.GetProperty("NewModFile");
	TSharedRef<IPropertyHandle> PathToModRootDirectory = DetailBuilder.GetProperty("PathToModRootDirectory");
	TSharedRef<IPropertyHandle> VersionString = DetailBuilder.GetProperty("VersionString");
	TSharedRef<IPropertyHandle> Changelog = DetailBuilder.GetProperty("Changelog");
	TSharedRef<IPropertyHandle> SetAsActiveReleas = DetailBuilder.GetProperty("bSetAsActiveRelease");
	TSharedRef<IPropertyHandle> MetadataBlob = DetailBuilder.GetProperty("MetadataBlob");
	TSharedRef<IPropertyHandle> ModfilePlatforms = DetailBuilder.GetProperty("ModfilePlatforms");
	DetailBuilder.HideProperty(NewModFile);
	DetailBuilder.HideProperty(PathToModRootDirectory);
	DetailBuilder.HideProperty(MetadataBlob);
	DetailBuilder.HideProperty(ModfilePlatforms);

	IDetailCategoryBuilder& ModPropertiesCategory = DetailBuilder.EditCategory("Modio Create New Mod File Params", FText::FromString("Upload Mod File"));

	ModPropertiesCategory.AddCustomRow(FText::FromString("PathToModRootDirectory"))
	.NameContent()
	[	
		PathToModRootDirectory->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.MaxWidth(256.f)
		.AutoWidth()
		[
			SAssignNew(PathToModRootDirectoryEditableTextBox, SEditableTextBox)
			.MinDesiredWidth(256.f)
			.OnTextChanged_Lambda([this](const FText& InText)
			{
				Target->PathToModRootDirectory = InText.ToString();
			})
		]

		+ SHorizontalBox::Slot()
		.MaxWidth(128.f)
		.AutoWidth()
		[
			SNew(SButton)
			.OnClicked_Lambda([this]()
			{
				WindowManager::Get().OpenFolderDialog("Choose your workspace:", Target->PathToModRootDirectory);
				PathToModRootDirectoryEditableTextBox->SetText(FText::FromString(Target->PathToModRootDirectory));
				return FReply::Handled();
			})
			[
				SNew(STextBlock).Text(FText::FromString("Browse"))
			]
		]
	];
}
