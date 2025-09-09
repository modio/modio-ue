/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/ModioEditorAddModFileWidget.h"

#include "EditorDirectories.h"
#include "ModioEditorUtilityFunctions.h"
#include "SlateOptMacros.h"
#include "Types/ModioCommonTypes.h"
#include "Widgets/Input/SDirectoryPicker.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "ModioEditorAddModFileWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SModioEditorAddModFileWidget::Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem)
{
	ParentWindow = InArgs._ParentWindow;
	ModInfo = InArgs._ModInfo;

	NavigationHandler = InArgs._NavigationHandler;

	ModioSubsystem = InModioSubsystem;

	CreateModFileParams = FModioCreateModFileParams();

	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.Padding(0.f, 0.f, 0.f, 4.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ModioAddModFileHeader", "Add Mod file"))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 16))
		]
		+ SVerticalBox::Slot()
		[
			SAssignNew(ContentBox, SBox)
		]
	];
	// clang-format on

	UpdateWindowContent();
}

void SModioEditorAddModFileWidget::UpdateWindowContent()
{
	ContentBox->SetContent(
		// clang-format off
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 20.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ModioAddModFileSubheader", "ZIP and upload the base folder of your UGC, or if it is a collection of files which live in a pre-existing game folder, you should ZIP those files:\n"
															"- Must be a ZIP file\n"
															"- Cannot exceed 20gb in filesize\n"
															"- Filenames cannot contain any of the following characters: \\/?\"<>|:*\n"
															"- UGC which overwrite files are not supported unless the game manages this\n"
															"- UGC which span multiple game directories are not supported unless the game manages this"))

			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 8))
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(ParentWindow->PanelBackgroundBrush)
			]
			+ SOverlay::Slot()
			.Padding(8.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.FillWidth(1)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModdRootDir", "Mod Root Directory"))
				]
			
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SDirectoryPicker)
					.Directory(CreateModFileParams.PathToModRootDirectory)
					.OnDirectoryChanged(this, &SModioEditorAddModFileWidget::OnModRootPathChosen)
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(ParentWindow->PanelBackgroundBrush)
			]
			+ SOverlay::Slot()
			.Padding(8.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.FillWidth(1.f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModPlatformList", "Platform (select all that apply)"))
				]

				+ SHorizontalBox::Slot()
				.FillWidth(2.f)
				[
					SAssignNew(PlatformGridPanel, SGridPanel)
					.FillColumn(0, 1.f)
					.FillColumn(1, 1.f)
					.FillColumn(2, 1.f)
					.FillColumn(3, 1.f)
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(ParentWindow->PanelBackgroundBrush)
			]
			+ SOverlay::Slot()
			.Padding(8.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(1)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("VersionString", "Version String"))
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SEditableTextBox)
					.OnTextChanged_Lambda([this](const FText& NewText) {
						CreateModFileParams.VersionString = NewText.ToString();
					})
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(ParentWindow->PanelBackgroundBrush)
			]
			+ SOverlay::Slot()
			.Padding(8.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.FillWidth(1)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Changelog", "Changelog"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SBox)
					.MinDesiredHeight(50)
					.MaxDesiredHeight(50)
					[
						SNew(SMultiLineEditableTextBox)
						.OnTextChanged_Lambda([this](const FText& NewText) {
							CreateModFileParams.Changelog = NewText.ToString();
						})
					]
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage).Image(ParentWindow->PanelBackgroundBrush)
			]
			+ SOverlay::Slot()
			.Padding(8.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(1)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ActiveRelease", "Set as Active Release"))
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SCheckBox)
					.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
					{
						CreateModFileParams.bSetAsActiveRelease = NewState == ECheckBoxState::Checked;
					})
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(ParentWindow->PanelPadding)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) 
				{
					bAcceptedTerms = NewState == ECheckBoxState::Checked;
				})
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(4.f, 0.f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AgreeTerms", "I agree to the terms and privacy policy"))
			]
		]

		+ SVerticalBox::Slot()
		.Padding(FMargin(0, 12))
		.VAlign(VAlign_Bottom)
		[
			SNew(SSeparator)
			.SeparatorImage(ParentWindow->BoldSeperatorBrush)
			.Thickness(1.0f)
		]

		+ SVerticalBox::Slot()
		.VAlign(VAlign_Bottom)
		.Padding(2.5f)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("Back", "Back"))
				.OnClicked_Lambda([this]()
					{ 
						NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::EditMod_EditProfile);
						return FReply::Handled();
					})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("UploadModFile", "Upload Mod File"))
				.IsEnabled_Lambda([this]() { return bAcceptedTerms; })
				.OnClicked_Lambda([this]()
				{ 
					ModioSubsystem->SubmitNewModFileForMod(ModInfo.ModId, CreateModFileParams);
					ParentWindow->UploadModID = ModInfo.ModId;
					ParentWindow->ShowProgressBar();
					NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EditMod_EditProfile);
					return FReply::Handled();
				})
			]
		]
		// clang-format on
	);

	PopulatePlatformOptions(PlatformGridPanel);
}

void SModioEditorAddModFileWidget::OnModRootPathChosen(const FString& PickedPath)
{
	CreateModFileParams.PathToModRootDirectory = FPaths::ConvertRelativePathToFull(PickedPath);
}

TSharedPtr<SWidget> SModioEditorAddModFileWidget::CreateLabeledCheckbox(
	const TTuple<FText, EModioModfilePlatform>& Info)
{
	// clang-format off
	return SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		.Padding(2.f)
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged_Lambda([this, PlatformInfo = Info](ECheckBoxState NewState)
			{
				if (NewState == ECheckBoxState::Checked)
				{
					if (!CreateModFileParams.ModfilePlatforms)
					{
						CreateModFileParams.ModfilePlatforms = TArray<EModioModfilePlatform>();
					}

					CreateModFileParams.ModfilePlatforms->AddUnique(PlatformInfo.Value);
				}
				else
				{
					if (!CreateModFileParams.ModfilePlatforms)
					{
						return;
					}

					CreateModFileParams.ModfilePlatforms->Remove(PlatformInfo.Value);
				}
			})
		]
		+ SHorizontalBox::Slot()
		.Padding(2.f)
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(Info.Key)
			.Justification(ETextJustify::Left)
		];
	// clang-format on
}

void SModioEditorAddModFileWidget::PopulatePlatformOptions(TSharedPtr<SGridPanel> Grid)
{
	Grid->ClearChildren();

	TArray<TTuple<FText, EModioModfilePlatform>> PlatformOptions =
		UModioEditorUtilityFunctions::GetModfilePlatformOptions();

	for (int i = 0; i < PlatformOptions.Num(); ++i)
	{
		TSharedPtr<SWidget> Checkbox;

		Checkbox = CreateLabeledCheckbox(PlatformOptions[i]);

		Grid->AddSlot(i % 4, i / 4).AttachWidget(Checkbox.ToSharedRef());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE