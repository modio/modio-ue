/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "../../Public/DetailCustomizations/ModioEditModParamsDetails.h"
#include "Engine/StaticMesh.h"
#include "../../Public/Objects/ModioEditModParamsObject.h"
#include "PropertyEditing.h"
#include "Widgets/Input/SCheckBox.h"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
#include <DesktopPlatform/Public/IDesktopPlatform.h>
	#include <DesktopPlatform/Public/DesktopPlatformModule.h>
#else
	#include "DesktopPlatformModule.h"
	#include "IDesktopPlatform.h"
#endif
#include <Framework/Application/SlateApplication.h>
#include <WindowManager.h>
#include <Widgets/Images/SThrobber.h>
#include <PropertyCustomizationHelpers.h>

#define LOCTEXT_NAMESPACE "ModioCreateModParamsDetails"

TSharedRef<IDetailCustomization> ModioEditModParamsDetails::MakeInstance()
{
	return MakeShareable(new ModioEditModParamsDetails);
}

void ModioEditModParamsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Target = Cast<UModioEditModParamsObject>(Object);
			if (Target)
			{
				break;
			}
		}
	}
	DrawEditMod(DetailBuilder);
}

void ModioEditModParamsDetails::DrawEditMod(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> NameProperty = DetailBuilder.GetProperty("Name");
	TSharedRef<IPropertyHandle> SummaryProperty = DetailBuilder.GetProperty("Summary");
	TSharedRef<IPropertyHandle> NamePathProperty = DetailBuilder.GetProperty("NamePath");
	
	// Deprecated property, use VisibilityProperty
	TSharedRef<IPropertyHandle> VisibleProperty_DEPRECATED = DetailBuilder.GetProperty("bVisible_DEPRECATED");
	
	TSharedRef<IPropertyHandle> Visibilityroperty = DetailBuilder.GetProperty("Visibility");
	TSharedRef<IPropertyHandle> DescriptionProperty = DetailBuilder.GetProperty("Description");
	TSharedRef<IPropertyHandle> HomepageURLProperty = DetailBuilder.GetProperty("HomepageURL");
	TSharedRef<IPropertyHandle> MaturityFlagsProperty = DetailBuilder.GetProperty("MaturityFlags");
	TSharedRef<IPropertyHandle> MetadataBlobProperty = DetailBuilder.GetProperty("MetadataBlob");
	TSharedRef<IPropertyHandle> LogoPathProperty = DetailBuilder.GetProperty("LogoPath");
	
	DetailBuilder.HideProperty(NamePathProperty);
	
	// Deprecated property, use VisibilityProperty
	DetailBuilder.HideProperty(VisibleProperty_DEPRECATED);
	
	DetailBuilder.HideProperty(Visibilityroperty);
	DetailBuilder.HideProperty(DescriptionProperty);
	DetailBuilder.HideProperty(MaturityFlagsProperty);
	DetailBuilder.HideProperty(MetadataBlobProperty);
	DetailBuilder.HideProperty(LogoPathProperty);
	DetailBuilder.HideProperty(HomepageURLProperty);

	IDetailCategoryBuilder& ModPropertiesCategory = DetailBuilder.EditCategory("Modio Edit Mod Params", FText::FromString("Edit Mod Properties"));
	
	TArray<FString> Splits;
	Target->HomepageURL.ParseIntoArray(Splits, TEXT("/"), true);

	FString URL;
	for (int i = 0; i < Splits.Num() - 1; i++) 
	{
		URL += Splits[i] + (i == 0 ? "//" : "/");
	}
	FString name_id = Splits[Splits.Num() - 1];
	
	ModPropertiesCategory.AddCustomRow(FText::FromString("HomepageURL"))
	.NameContent()
	[
		HomepageURLProperty->CreatePropertyNameWidget()
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
			SNew(SEditableTextBox)
			.MinDesiredWidth(100.f)
			.IsReadOnly(true)
			.Text(FText::FromString(URL))
		]

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.MaxWidth(256.f)
		.AutoWidth()
		[
			SAssignNew(HomePageURLEditableTextBox, SEditableTextBox)
			.MinDesiredWidth(256.f)
			.OnTextChanged_Lambda([this, URL](const FText& Text)
			{
				FString NewURL = URL + HomePageURLEditableTextBox->GetText().ToString();
				Target->HomepageURL = NewURL;
			})
		]
	];
	HomePageURLEditableTextBox->SetText(FText::FromString(name_id));
}

#undef LOCTEXT_NAMESPACE