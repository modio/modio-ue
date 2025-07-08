/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "DetailCustomizations/ModioCreateModParamsDetails.h"
#include "Engine/StaticMesh.h"
#include "Misc/EngineVersionComparison.h"
#include "Objects/ModioCreateModParamsObject.h"
#include "Widgets/Input/SCheckBox.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "DesktopPlatform/Public/DesktopPlatformModule.h"
	#include "DesktopPlatform/Public/IDesktopPlatform.h"
#else
	#include "DesktopPlatformModule.h"
	#include "IDesktopPlatform.h"
#endif
#include "DetailLayoutBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "WindowManager.h"

#define LOCTEXT_NAMESPACE "ModioCreateModParamsDetails"

TSharedRef<IDetailCustomization> ModioCreateModParamsDetails::MakeInstance()
{
	return MakeShareable(new ModioCreateModParamsDetails);
}

void ModioCreateModParamsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> Object : CustomizedObjects)
	{
		if (Object.IsValid())
		{
			Target = Cast<UModioCreateModParamsObject>(Object);
			if (Target)
			{
				break;
			}
		}
	}
	DrawCreateMod(DetailBuilder);
}

void ModioCreateModParamsDetails::DrawCreateMod(IDetailLayoutBuilder& DetailBuilder)
{
	TSharedRef<IPropertyHandle> NameProperty = DetailBuilder.GetProperty("Name");
	TSharedRef<IPropertyHandle> PathToLogoFileProperty = DetailBuilder.GetProperty("PathToLogoFile");
	TSharedRef<IPropertyHandle> SummaryProperty = DetailBuilder.GetProperty("Summary");

	DetailBuilder.HideProperty(NameProperty);
	DetailBuilder.HideProperty(PathToLogoFileProperty);
	DetailBuilder.HideProperty(SummaryProperty);

	IDetailCategoryBuilder& ModPropertiesCategory =
		DetailBuilder.EditCategory("Modio Create Mod Param", FText::FromString("Create Mod Properties"));
	ModPropertiesCategory.AddCustomRow(FText::FromString("PathToLogoFile"))
		.NameContent()[PathToLogoFileProperty->CreatePropertyNameWidget()]
		.ValueContent()
			[SNew(SHorizontalBox) +
			 SHorizontalBox::Slot()
				 .HAlign(HAlign_Left)
				 .VAlign(VAlign_Center)
				 .MaxWidth(256.f)
				 .AutoWidth()[SAssignNew(PathToLogoFileEditableTextBox, SEditableTextBox).MinDesiredWidth(256.f)]

			 + SHorizontalBox::Slot().MaxWidth(128.f).AutoWidth()[SNew(SButton).OnClicked_Lambda([this]() {
				   TArray<FString> Thumbnails;
				   WindowManager::Get().OpenFileDialog("Choose your thumbnail:", "PNG Files (*.png)|*.png", false,
													   Thumbnails);
				   if (Thumbnails.Num() > 0)
				   {
					   Target->PathToLogoFile = Thumbnails[0];
					   PathToLogoFileEditableTextBox->SetText(FText::FromString(Target->PathToLogoFile));
				   }
				   return FReply::Handled();
			   })[SNew(STextBlock).Text(FText::FromString("Browse"))]]];

	ModPropertiesCategory.AddCustomRow(FText::FromString("Name"))
		.NameContent()[NameProperty->CreatePropertyNameWidget()]
		.ValueContent()[SNew(SHorizontalBox) + SHorizontalBox::Slot()
												   .HAlign(HAlign_Left)
												   .VAlign(VAlign_Center)
												   .MaxWidth(256.f)
												   .AutoWidth()[SAssignNew(NameEditableTextBox, SEditableTextBox)
																	.MinDesiredWidth(256.f)
																	.OnTextChanged_Lambda([this](const FText& Text) {
																		Target->Name =
																			NameEditableTextBox->GetText().ToString();
																	})]];

	ModPropertiesCategory.AddCustomRow(FText::FromString("Summary"))
		.NameContent()[SummaryProperty->CreatePropertyNameWidget()]
		.ValueContent()
		.MinDesiredWidth(
			512.f)[SNew(SHorizontalBox) +
				   SHorizontalBox::Slot()
					   .HAlign(HAlign_Fill)
					   .VAlign(VAlign_Center)
					   .MaxWidth(256.f)[SAssignNew(SummaryMultiLineEditableTextBox, SMultiLineEditableTextBox)
											.AllowMultiLine(true)
											.AutoWrapText(true)
											.Justification(ETextJustify::Left)
											.OnTextChanged_Lambda([this](const FText& Text) {
												Target->Summary = SummaryMultiLineEditableTextBox->GetText().ToString();
											})]];
}

#undef LOCTEXT_NAMESPACE