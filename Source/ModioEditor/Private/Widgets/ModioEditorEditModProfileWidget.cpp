/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/ModioEditorEditModProfileWidget.h"
#include "EditorDirectories.h"
#include "Widgets/Input/SFilePathPicker.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/STextComboBox.h"

#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "ModioEditorEditModProfileWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorEditModProfileWidget::Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem)
{
	ModioSubsystem = InModioSubsystem;

	ParentWindow = InArgs._ParentWindow;
	NavigationHandler = InArgs._NavigationHandler;
	ModInfo = InArgs._ModInfo;

	FString FilePath;
	EditModParams = FModioEditModParams();

	EditModParams.Name = ModInfo.ProfileName;
	EditModParams.MaturityFlags = ModInfo.ProfileMaturityOption;
	EditModParams.Summary = ModInfo.ProfileSummary;
	EditModParams.Visibility = ModInfo.Visibility;

	ModVisibilityOptions.Empty();
	ModVisibilityOptions.Add(MakeShared<FString>("Hidden"));
	ModVisibilityOptions.Add(MakeShared<FString>("Public"));

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
			.Text(LOCTEXT("ModioEditModProfileHeader", "Edit Mod Profile"))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 16))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 20.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ModioEditModProfileSubheader", "Edit an entry on mod.io for this mod. This will contain all the details, images, files and settings for the mod."))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 8))
		]
		+ SVerticalBox::Slot()
		[
			SAssignNew(ContentBox, SBox)
		]
	];

	UpdateWindowContent();
}

void SModioEditorEditModProfileWidget::UpdateWindowContent()
{
	if (bIsLoading)
	{
		ContentBox->SetContent(
			// clang-format off
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 24, 0, 8))
			.HAlign(HAlign_Center)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModioEditModProfileCreating", "Editing mod profile..."))
				.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 12))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SThrobber)
			]
			// clang-format on
		);
		return;
	}

	ContentBox->SetContent(
		// clang-format off
		SNew(SVerticalBox)
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
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModThumbnailImageLabel", "Mod Thumbnail Image"))
					]

					+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModThumbnailRequirements", "16:9 | Min 512x288 | Max 8MB | PNG"))
						.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 8))
						.ColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f))
					]
				]
			
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SFilePathPicker)
					.BrowseButtonImage(FAppStyle::GetBrush("PropertyWindow.Button_Ellipsis"))
					.BrowseButtonStyle(FAppStyle::Get(), "HoverHintOnly")
					.BrowseButtonToolTip(LOCTEXT("FileButtonToolTipText", "Choose Thumbnail file"))
					.FilePath(this, &SModioEditorEditModProfileWidget::GetThumbnailPath)
					.OnPathPicked(this, &SModioEditorEditModProfileWidget::OnThumbnailPathPicked)
					.BrowseDirectory(FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_OPEN))
					.BrowseTitle(LOCTEXT("BrowseButtonTitle", "Choose Thumbnail file"))
					.FileTypeFilter(TEXT("PNG File (*.png)|*.png|JPG File (*.jpg)|*.jpg|JPEG File (*.jpeg)|*.jpeg"))
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
					.Text(LOCTEXT("ModNameLabel", "Mod Name"))
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.FillWidth(2)
				[
					SNew(SEditableTextBox)
					.HintText(LOCTEXT("ModioModNameHint", "Enter mod name"))
					.Text(FText::FromString(ModInfo.ProfileName))
					.OnTextChanged_Lambda([this](const FText& NewText) {
						bIsDirty = true;

						EditModParams.Name = NewText.ToString();
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
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModSummaryLabel", "Summary"))
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModSummaryRequirement", "* max 250 characters"))
						.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 8))
						.ColorAndOpacity(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f))
					]
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
						.Text(FText::FromString(ModInfo.ProfileSummary))
						.OnTextChanged_Lambda([this](const FText& NewText) {
							bIsDirty = true;

							EditModParams.Summary = NewText.ToString();
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
					.Text(LOCTEXT("ModVisibilityLabel", "Visibility"))
				]

				+ SHorizontalBox::Slot()
				.FillWidth(2)
				.HAlign(HAlign_Fill)
				[
					SAssignNew(VisibilityComboBox, STextComboBox)
					.OnSelectionChanged(this, &SModioEditorEditModProfileWidget::OnModVisibilityChanged)
					.OptionsSource(&ModVisibilityOptions)
					.InitiallySelectedItem(ModVisibilityOptions[static_cast<uint8_t>(ModInfo.Visibility)])
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
					.Text(LOCTEXT("ModMatureContentLabel", "Contains Mature Content"))
				]

				+ SHorizontalBox::Slot()
				.FillWidth(2.f)
				[
					SAssignNew(MaturityGridPanel, SGridPanel)
					.FillColumn(0, 1.f)
					.FillColumn(1, 1.f)
				]
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
		.AutoHeight()
		.Padding(2.5f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(12.f)
				.Text(LOCTEXT("ModioCreateModProfileBack", "Back"))
				.OnClicked_Lambda([this]()
					{ 
						NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::BrowseMods);
						return FReply::Handled();
					})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(12.f)
				.Text(LOCTEXT("AddModFile", "Add Mod File"))
				.OnClicked_Lambda([this]() 
				{ 
					NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::EditMod_AddNewModFile );
					return FReply::Handled();
				})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("CookSourceFile", "Cook Source File"))
				.OnClicked_Lambda([this]() 
				{ 
					NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::EditMod_AddNewSourceModFile );
					return FReply::Handled();
				})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this]() { return bIsDirty; })
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("Save", "Save"))
				.OnClicked_Lambda([this]() 
				{ 
					bIsLoading = true;
					UpdateWindowContent();

					ModioSubsystem->SubmitModChangesAsync(ModInfo.ModId, EditModParams, FOnGetModInfoDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfo> MaybeModInfo)
					{
						bIsLoading = false;

						if (!ErrorCode)
						{
							const FText Title = LOCTEXT("ModEdited", "Mod Edited");
							const FText Message = LOCTEXT("ModEditedMessage", "Mod successfully edited!");
#if UE_VERSION_OLDER_THAN(5, 3, 0)
							EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
#else
							EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
#endif
							ModInfo = *MaybeModInfo;
							bIsDirty = false;
						}
						else
						{
							const FText Title = LOCTEXT("ModCreationFailed", "Failed To Create Mod");
							const FText Message =
							FText::Format(LOCTEXT("ModEditingFailedMessage", "The mod was unable to be edited. \n {0}"), FText::FromString(ErrorCode.GetErrorMessage()));
#if UE_VERSION_OLDER_THAN(5, 3, 0)
							EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
#else
							EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
#endif
						}

						UpdateWindowContent();
					}));

					return FReply::Handled();
				})
			]
		]
		// clang-format on
	);

	PopulateModMaturityOptions(MaturityGridPanel);
}

void SModioEditorEditModProfileWidget::OnModVisibilityChanged(TSharedPtr<FString> ModVisibility,
															  ESelectInfo::Type SelectInfo)
{
	bIsDirty = true;
	EditModParams.Visibility =
		*ModVisibility == "Public" ? EModioObjectVisibilityFlags::Public : EModioObjectVisibilityFlags::Hidden;
}

void SModioEditorEditModProfileWidget::OnThumbnailPathPicked(const FString& PickedPath)
{
	bIsDirty = true;
	EditModParams.LogoPath = FPaths::ConvertRelativePathToFull(PickedPath);
}

FString SModioEditorEditModProfileWidget::GetThumbnailPath() const
{
	return (EditModParams.LogoPath.IsSet() ? *EditModParams.LogoPath : FString());
}

void SModioEditorEditModProfileWidget::PopulateModMaturityOptions(TSharedPtr<SGridPanel> Grid)
{
	Grid->ClearChildren();

	TArray<TTuple<FText, EModioMaturityFlags>> MaturityOptions;
	MaturityOptions.Add(
		TTuple<FText, EModioMaturityFlags>(LOCTEXT("ModioMaturityAlcohol", "Alcohol"), EModioMaturityFlags::Alcohol));
	MaturityOptions.Add(
		TTuple<FText, EModioMaturityFlags>(LOCTEXT("ModioMaturityDrugs", "Drugs"), EModioMaturityFlags::Drugs));
	MaturityOptions.Add(TTuple<FText, EModioMaturityFlags>(LOCTEXT("ModioMaturityViolence", "Violence"),
														   EModioMaturityFlags::Violence));
	MaturityOptions.Add(TTuple<FText, EModioMaturityFlags>(LOCTEXT("ModioMaturityExplicit", "Explicit"),
														   EModioMaturityFlags::Explicit));

	EModioMaturityFlags ExistingFlags = ModInfo.ProfileMaturityOption;

	for (int i = 0; i < MaturityOptions.Num(); ++i)
	{
		TSharedPtr<SWidget> Checkbox;

		bool bCheck = EnumHasAnyFlags(ExistingFlags, MaturityOptions[i].Value);
		Checkbox = CreateLabeledCheckbox(MaturityOptions[i], bCheck);

		Grid->AddSlot(i / 2, i % 2).AttachWidget(Checkbox.ToSharedRef());
	}
}

TSharedPtr<SWidget> SModioEditorEditModProfileWidget::CreateLabeledCheckbox(
	const TTuple<FText, EModioMaturityFlags>& Option, bool bCheck)
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
			.IsChecked(bCheck)
			.OnCheckStateChanged_Lambda([this, OptionFlag = Option.Value](ECheckBoxState NewState) 
				{
					bIsDirty = true;

					if(NewState == ECheckBoxState::Checked)
					{
						if (!EditModParams.MaturityFlags)
						{
							EditModParams.MaturityFlags = OptionFlag;
							return;
						}

						EnumAddFlags(*EditModParams.MaturityFlags, OptionFlag);
					}
					else
					{
						if(EditModParams.MaturityFlags)
						{
							EnumRemoveFlags(*EditModParams.MaturityFlags, OptionFlag);
						}
					}
				})
		]
		+ SHorizontalBox::Slot()
		.Padding(2.f)
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(Option.Key)
			.Justification(ETextJustify::Left)
		];
	// clang-format on
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
