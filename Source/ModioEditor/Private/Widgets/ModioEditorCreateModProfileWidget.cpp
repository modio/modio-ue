/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/ModioEditorCreateModProfileWidget.h"
#include "EditorDirectories.h"
#include "SListViewSelectorDropdownMenu.h"
#include "SlateOptMacros.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Input/SFilePathPicker.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "CreateModProfileWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorCreateModProfileWidget::Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem)
{
	ModioSubsystem = InModioSubsystem;

	ParentWindow = InArgs._ParentWindow;
	NavigationHandler = InArgs._NavigationHandler;

	CreateModParams = FModioCreateModParams();

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
			.Text(LOCTEXT("ModioCreateModProfileHeader", "Create Mod Profile"))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 16))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 0.f, 0.f, 20.f)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ModioCreateModProfileSubheader", 
				"Create an entry on mod.io for this mod. This will contain all "
				"the details, images, files and settings for the mod."))
			.Font(ParentWindow->GetTextStyle("EmbossedText", "Normal", 8))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ContentBox, SBox)
			.MaxDesiredHeight(365)
		]

		+ SVerticalBox::Slot()
		.Padding(FMargin(0, 12))
		.VAlign(VAlign_Bottom)
		[
			SNew(SSeparator)
			.Visibility_Lambda([this]() {
				return bIsLoading ? EVisibility::Collapsed : EVisibility::Visible;
			})
			.SeparatorImage(ParentWindow->BoldSeperatorBrush)
			.Thickness(1.0f)
		]

		+ SVerticalBox::Slot()
		.VAlign(VAlign_Bottom)
		.Padding(2.5f)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			.Visibility_Lambda([this]() {
				return bIsLoading ? EVisibility::Collapsed : EVisibility::Visible;
			})
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("ModioCreateModProfileBack", "Back"))
				.OnClicked_Lambda([this]()
					{ 
						NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::BrowseMods);
						return FReply::Handled();
					})
			]

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				SNew(SButton)
				.ContentPadding(ParentWindow->BottomButtonPadding)
				.Text(LOCTEXT("ModioCreateModProfileCreateMod", "Create Mod Profile"))
				.IsEnabled_Lambda([this]() { return !CreateModParams.Name.IsEmpty() && !CreateModParams.PathToLogoFile.IsEmpty(); })
				.OnClicked_Lambda([this]() 
				{
					bIsLoading = true;
					UpdateWindowContent();

					FModioModCreationHandle Handle = ModioSubsystem->GetModCreationHandle();
					
					ModioSubsystem->SubmitNewModAsync(
						Handle, 
						CreateModParams, 
						FOnSubmitNewModDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModID> ModId)
						{
							bIsLoading = false;

							if (!ErrorCode)
							{
								const FText Title = LOCTEXT("ModCreated", "Mod Created");
								const FText Message = LOCTEXT("ModCreatedMessage", "Mod successfully created!");
#if UE_VERSION_OLDER_THAN(5, 3, 0)
								EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
#else
								EAppReturnType::Type UserSelection =
								FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
#endif
								NavigationHandler.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::BrowseMods);
							}
							else
							{
								const FText Title = LOCTEXT("ModCreationFailed", "Failed To Create Mod");
								const FText Message =  FText::Format(
									LOCTEXT("ModCreationFailedMessage","The mod was unable to be created. \n {0}"), 
									FText::FromString(ErrorCode.GetErrorMessage()));
#if UE_VERSION_OLDER_THAN(5, 3, 0)
								EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
#else
								EAppReturnType::Type UserSelection =
								FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
#endif
								UpdateWindowContent();
							}

						}));

					return FReply::Handled();
				})
			]
		]
	];

	UpdateWindowContent();

	// clang-format on
}

void SModioEditorCreateModProfileWidget::UpdateWindowContent()
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
				.Text(LOCTEXT("ModioCreateModProfileCreating", "Creating mod profile..."))
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
		SNew(SScrollBox)
		.ScrollBarAlwaysVisible(true)
		.Orientation(EOrientation::Orient_Vertical)
		+ SScrollBox::Slot()
		[
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
						.OnTextChanged_Lambda([this](const FText& Updated)
						{
							CreateModParams.Name = Updated.ToString();
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
						.FilePath(this, &SModioEditorCreateModProfileWidget::GetThumbnailPath)
						.OnPathPicked(this, &SModioEditorCreateModProfileWidget::OnThumbnailPathPicked)
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
								.OnTextChanged_Lambda([this](const FText& Updated) 
								{
									CreateModParams.Summary = Updated.ToString();
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
						.OnSelectionChanged(this, &SModioEditorCreateModProfileWidget::OnModVisibilityChanged)
						.OptionsSource(&ModVisibilityOptions)
						.InitiallySelectedItem(ModVisibilityOptions[1])
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
					.VAlign(VAlign_Top)
					.FillWidth(1)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModTagsLabel", "Tags"))
					]

					+ SHorizontalBox::Slot()
					.FillWidth(2)
					[
						SAssignNew(TagsGridPanel, SGridPanel)
						.FillColumn(0, 1.f)
						.FillColumn(1, 1.f)
						.FillColumn(2, 1.f)
						.FillColumn(3, 1.f)
					]
				]
			]
		]
		// clang-format on
	);

	PopulateModMaturityOptions(MaturityGridPanel);
	PopulateModTagCheckboxes(TagsGridPanel);
}

void SModioEditorCreateModProfileWidget::OnModVisibilityChanged(TSharedPtr<FString> ModVisibility,
																ESelectInfo::Type SelectInfo)
{
	CreateModParams.Visibility = *ModVisibility == "Public" ? 1 : 0;
}

void SModioEditorCreateModProfileWidget::OnThumbnailPathPicked(const FString& PickedPath)
{
	CreateModParams.PathToLogoFile = FPaths::ConvertRelativePathToFull(PickedPath);
}

FString SModioEditorCreateModProfileWidget::GetThumbnailPath() const
{
	return CreateModParams.PathToLogoFile;
}

void SModioEditorCreateModProfileWidget::PopulateModMaturityOptions(TSharedPtr<SGridPanel> Grid)
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

	for (int i = 0; i < MaturityOptions.Num(); ++i)
	{
		TSharedPtr<SWidget> Checkbox;

		Checkbox = CreateLabeledMaturityCheckbox(MaturityOptions[i].Key, MaturityOptions[i].Value);

		Grid->AddSlot(i % 4, i / 4).AttachWidget(Checkbox.ToSharedRef());
	}
}

void SModioEditorCreateModProfileWidget::PopulateModTagCheckboxes(TSharedPtr<SGridPanel> Grid)
{
	Grid->ClearChildren();

	TArray<FString> Tags;
	for (int OptionIndex = 0; OptionIndex < ParentWindow->ModioGameInfo->TagOptions.Num(); ++OptionIndex)
	{
		for (int TagValue = 0; TagValue < ParentWindow->ModioGameInfo->TagOptions[OptionIndex].TagGroupValues.Num();
			 ++TagValue)
		{
			Tags.Add(ParentWindow->ModioGameInfo->TagOptions[OptionIndex].TagGroupValues[TagValue]);
		}
	}

	for (int i = 0; i < Tags.Num(); ++i)
	{
		TSharedPtr<SWidget> Checkbox;

		Checkbox = CreateLabeledTagCheckbox(Tags[i]);

		Grid->AddSlot(i % 4, i / 4).AttachWidget(Checkbox.ToSharedRef());
	}
}

TSharedPtr<SWidget> SModioEditorCreateModProfileWidget::CreateLabeledTagCheckbox(const FString& Label)
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
				.OnCheckStateChanged_Lambda([this, TagLabel = Label](ECheckBoxState NewState) 
					{
						if(NewState == ECheckBoxState::Checked)
						{
							if (!CreateModParams.Tags)
							{
								CreateModParams.Tags = TArray<FString>();
							}

							CreateModParams.Tags->AddUnique(TagLabel);
						}
						else
						{
							if(CreateModParams.Tags)
							{
								CreateModParams.Tags->Remove(TagLabel);
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
			SNew(STextBlock).Text(FText::FromString(Label))
			.Justification(ETextJustify::Left)
		];
	// clang-format on
}

TSharedPtr<SWidget> SModioEditorCreateModProfileWidget::CreateLabeledMaturityCheckbox(const FText& Label,
																					  EModioMaturityFlags Flag)
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
				.OnCheckStateChanged_Lambda([this, OptionFlag = Flag](ECheckBoxState NewState) 
					{
						if(NewState == ECheckBoxState::Checked)
						{
							if (!CreateModParams.MaturityFlags)
							{
								CreateModParams.MaturityFlags = OptionFlag;
								return;
							}

							EnumAddFlags(*CreateModParams.MaturityFlags, OptionFlag);
						}
						else
						{
							if(CreateModParams.MaturityFlags)
							{
								EnumRemoveFlags(*CreateModParams.MaturityFlags, OptionFlag);
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
			SNew(STextBlock).Text(Label)
			.Justification(ETextJustify::Left)
		];
	// clang-format on
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE