/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "DetailCustomizations/ModioSettingsDetails.h"

#include "Widgets/SModioEditorUserAuthWidget.h"
#include "Widgets/SModioEditorGameInfoWidget.h"
#include "Widgets/SModioEditorUserGamesList.h"
#include "ModioSettings.h"

#include "Widgets/Input/SButton.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "SHyperlinkLaunchURL.h"
#include "Interfaces/IMainFrameModule.h"
#include "Widgets/Images/SThrobber.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "ModioSettingsDetails"

TSharedRef<IDetailCustomization> FModioSettingsDetails::MakeInstance()
{
	return MakeShareable(new FModioSettingsDetails);
}

void FModioSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	IDetailCategoryBuilder& BasicCategory = DetailLayout.EditCategory(TEXT("Configuration"));

	BasicCategory.AddCustomRow(LOCTEXT("Initialization", "Initialization"), false)
		.WholeRowWidget
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 5, 5, 5))
			.AutoHeight()
			[
				SNew(SBox).HAlign(HAlign_Center)
				[
					SNew(SHyperlinkLaunchURL, TEXT("https://pubdocs.modapi.io/ue/documentation.html#_configuration"))
						.Text(LOCTEXT("ModioPluginSettingsDocumentation",
						"mod.io UE Plugin Configuration Documentation"))
						.ToolTipText(LOCTEXT("ModioPluginSettingsDocumentationTooltip",
						"Opens a page that outlines the configuration settings for the mod.io plugin."))
				]
			]
			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 5, 5, 5))
			.AutoHeight()
			[
				SNew(STextBlock)
					.Text(LOCTEXT("ModioPluginsSettingsConfigGuid", "If this is your first time setting up the plugin and/or working with mod.io, press the button below for and assisted setup process.\n\r"
																	"If you already know what you're doing you can enter the settings manually."))
			]
			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 5, 5, 5))
			.AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
								   .OnClicked(this, &FModioSettingsDetails::OnConfigureClicked)
				[
					SNew(STextBlock).Text(LOCTEXT("AuthenticateAndConfigure", "Configure"))
				]
			]
		];
}

void FModioSettingsDetails::ClearConfigContent()
{
	if(ConfigContentBox.IsValid())
	{
		if (ConfigContentBox->GetAllChildren()->Num() > 0)
		{
			ConfigContentBox->ClearChildren();
		}
	}
}

void FModioSettingsDetails::DrawThrobber()
{
	ClearConfigContent();
	ConfigContentBox->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SCircularThrobber).Radius(50.f)
		];
}

void FModioSettingsDetails::DrawConfigGuide()
{
	ClearConfigContent();

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.AutoHeight()
	[
		SNew(STextBlock)
			.Text(LOCTEXT("FirstTimePreamble", "If you have not made an account with mod.io, do so now here:"))
	];

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.AutoHeight()
	[
		SNew(SHyperlinkLaunchURL, TEXT("https://mod.io/login"))
				 .Text(LOCTEXT("ModioPluginLogin", "mod.io Sign up"))
				 .ToolTipText(LOCTEXT("ModioPluginLoginTooltip",
									  "Opens the login/sign up page for mod.io"))];

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()[SNew(STextBlock)
						  .Text(LOCTEXT("FirstTimeMakeGame",
										"Next, if you haven't already, you will need to make/register your game with mod.io, here:"))];

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()[SNew(SHyperlinkLaunchURL, TEXT("https://mod.io/g/add/"))
						  .Text(LOCTEXT("ModioPluginIntegrate", "mod.io Integration Setup"))
						  .ToolTipText(LOCTEXT("ModioPluginIntegrateTooltip", "Opens the game integration page for mod.io"))];

		ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()[SNew(STextBlock)
						  .Text(LOCTEXT("FirstTimeGetKeys", "Finally, you need to agree to the API Access Terms and get your Game ID and API Key.\r\n"
															 "1. Click the link below, you will find the Agreement Terms, and a list of all your games on mod.io\r\n"
															 "2. Scroll down until you find the game you are setting up.\r\n"
															 "3. Copy the GameID and API Key (on the far right) into the fields below."))];

	ConfigContentBox->AddSlot()
			.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()[SNew(SHyperlinkLaunchURL, TEXT("https://mod.io/me/access"))
						  .Text(LOCTEXT("ModioPluginKeys", "mod.io API Access Settings"))
						  .ToolTipText(
							  LOCTEXT("ModioPluginKeysTooltip", "Opens the API access page for the logged in user on mod.io"))];

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
						  .Padding(FMargin(5.f, 5.f, 5.f, 5.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
						.Text(LOCTEXT("GameID", "Game ID:"))
				]
			+ SHorizontalBox::Slot()
						  .Padding(FMargin(5.f, 5.f, 5.f, 5.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(GameIdInput, SEditableTextBox)
						.MinDesiredWidth(256.f)
						.Text(FText::FromString(FString::FromInt(GetDefault<UModioSettings>()->GameId)))
						.HintText(LOCTEXT("GameIDGint", "00000"))
				]
		];
	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
						  .Padding(FMargin(5.f, 5.f, 5.f, 5.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
						.Text(LOCTEXT("APIKey", "API Key:"))
				]
			+ SHorizontalBox::Slot()
						  .Padding(FMargin(5.f, 5.f, 5.f, 5.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SAssignNew(ApiKeyInput, SEditableTextBox)
						.MinDesiredWidth(256.f)
						.Text(FText::FromString(GetDefault<UModioSettings>()->ApiKey))
						.HintText(LOCTEXT("ApiKeyHint", "00000000000000000000000000000000"))
				]
		];

	ConfigContentBox->AddSlot()
		.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(1.f, 1.f))
			.OnClicked(this, &FModioSettingsDetails::OnSettingsSubmittedByUser)
			.Text(LOCTEXT("ApplySettings", "Apply"))
		];
}

FReply FModioSettingsDetails::OnConfigureClicked()
{
	ConfigWindow =
		SNew(SWindow)
		.Title(LOCTEXT("ConfigWindowTitle", "Configure"))
		.ClientSize(FVector2D(625.f, 350.f))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.SizingRule(ESizingRule::FixedSize)
		[
			SAssignNew(ConfigContentBox, SVerticalBox)
		];
	
	DrawConfigGuide();

	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();

	if (ParentWindow.IsValid())
	{
		FSlateApplication::Get().AddModalWindow(ConfigWindow.ToSharedRef(), ParentWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(ConfigWindow.ToSharedRef());
	}

	return FReply::Handled();
}

FReply FModioSettingsDetails::OnSettingsSubmittedByUser()
{
	ApplySettingsAndClose();
	return FReply::Handled();
}

void FModioSettingsDetails::ApplySettingsAndClose()
{
	UModioSettings* ModioSettings = GetMutableDefault<UModioSettings>();

	if (!GameIdInput->GetText().IsEmpty())
	{
		ModioSettings->GameId = FCString::Atoi(*GameIdInput->GetText().ToString());
	}

	if (!ApiKeyInput->GetText().IsEmpty())
	{
		ModioSettings->ApiKey = ApiKeyInput->GetText().ToString();
	}

	ModioSettings->SaveConfig();

	ConfigWindow.Get()->RequestDestroyWindow();
	ConfigWindow = nullptr;
}

#undef LOCTEXT_NAMESPACE
