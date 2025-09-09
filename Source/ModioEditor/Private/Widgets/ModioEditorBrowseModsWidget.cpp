/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/ModioEditorBrowseModsWidget.h"

#include "DetailCustomizations/ModioModBrowserEntry.h"
#include "SlateOptMacros.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Views/SListView.h"

#define LOCTEXT_NAMESPACE "BrowseModsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorBrowseModsWidget::Construct(const FArguments& InArgs, UModioSubsystem* InModioSubsystem)
{
	ModioSubsystem = InModioSubsystem;

	ModSelectedHandler = InArgs._ModSelectedHandler;

	// clang-format off
	ChildSlot
	[
		SNew(SBox)
		[
			SAssignNew(MainContent, SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 8.f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MyContent", "My Content"))
				.Font(InArgs._ParentWindow->GetTextStyle("EmbossedText", "Normal", 16))
				.Justification(ETextJustify::Center)
			]

			+ SVerticalBox::Slot()
			.Padding(0.f, 8.f)
			.AutoHeight()
			[
				SNew(SSearchBox)
					.OnTextChanged_Lambda([this](const FText& NewText)
					{
						FilterString = &NewText;
						FilterModList();

					})

			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(ModListBox, SBox)
				.MaxDesiredHeight(325)
			]

			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 12))
			.VAlign(VAlign_Bottom)
			[
				SNew(SSeparator)
				.SeparatorImage(InArgs._ParentWindow->BoldSeperatorBrush)
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
				[
					SNew(SButton)
					.ContentPadding(InArgs._ParentWindow->BottomButtonPadding)
					.Text(LOCTEXT("Back", "Back"))
						.OnClicked_Lambda([this, CompoundWindow = InArgs._ParentWindow]() 
						{
							CompoundWindow->DrawToolLanding();
							return FReply::Handled();
						})
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SButton)
					.ContentPadding(InArgs._ParentWindow->BottomButtonPadding)
					.Text(LOCTEXT("EditListing", "Edit Selected Listing"))
					.IsEnabled_Lambda([this, OuterWidget = InArgs._UploadAndManageWidget] () { return OuterWidget->CurrentModInfo.IsSet(); })
					.OnClicked_Lambda([this, Navigate = InArgs._NavigationHandler]() 
						{ 
							Navigate.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::EditMod_EditProfile );
							return FReply::Handled();
						})
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SButton)
					.ContentPadding(InArgs._ParentWindow->BottomButtonPadding)
					.Text(LOCTEXT("AddModListing", "Add New Mod Listing"))
					.OnClicked_Lambda([this, Navigate = InArgs._NavigationHandler]() 
						{ 
							Navigate.ExecuteIfBound(SModioEditorUploadAndManageUGCWidget::EUploadAndManageUIState::CreateMod );
							return FReply::Handled();
						})
				]
			]
		]
	];

	DrawModList();

	// clang-format on	
}


void SModioEditorBrowseModsWidget::DrawModList()
{
	FModioFilterParams FilterParams;
	ModioSubsystem->ListUserCreatedModsAsync(
		FilterParams, FOnListAllModsDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode,
																	  TOptional<FModioModInfoList> OptionalModList) {
			ModInfoOriginalSource.Empty();
			if (!ErrorCode)
			{
				const TArray<FModioModInfo>& RawList = OptionalModList.GetValue().GetRawList();
				for (const FModioModInfo& Mod : RawList)
				{
					ModInfoOriginalSource.Add(MakeShared<FModioModInfo>(Mod));
				}

				FilterModList();

				// clang-format off
				ModListBox->SetContent(
					SAssignNew(ListView, SListView<TSharedPtr<FModioModInfo>>)
					.SelectionMode(ESelectionMode::Single)
					.ListItemsSource(&ModInfoFilteredSource)
					.OnSelectionChanged(this, &SModioEditorBrowseModsWidget::OnModSelected)
					.OnGenerateRow(this, &SModioEditorBrowseModsWidget::GenerateModInfoRow)
				);

				// clang-format on
			}
		}));
}

void SModioEditorBrowseModsWidget::FilterModList()
{
	if (!FilterString || FilterString->IsEmpty())
	{
		ModInfoFilteredSource = ModInfoOriginalSource;
		return;
	}

	ModInfoFilteredSource.Empty();
	for (const auto& ModInfo : ModInfoOriginalSource)
	{
		if (ModInfo->ProfileName.Contains(FilterString->ToString()))
		{
			ModInfoFilteredSource.Add(ModInfo);
		}
	}

	if (ListView)
	{
		ListView->RequestListRefresh();
	}
}

TSharedRef<ITableRow> SModioEditorBrowseModsWidget::GenerateModInfoRow(TSharedPtr<FModioModInfo> ModInfo,
																	   const TSharedRef<STableViewBase>& OwnerTable)
{
	// clang-format off
	return SNew(STableRow<TSharedPtr<FModioModInfo>>, OwnerTable)
		[
			SNew(SModioModBrowserEntry, ModInfo, ModioSubsystem)
		];
	// clang-format on
}
void SModioEditorBrowseModsWidget::OnModSelected(TSharedPtr<FModioModInfo> Item, ESelectInfo::Type SelectInfoType)
{
	ModSelectedHandler.ExecuteIfBound(Item);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE