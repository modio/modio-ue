/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/SModioEditorUserGamesList.h"

#include "Engine/Engine.h"
#include "IDetailsView.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/EngineVersionComparison.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/SOverlay.h"
#include "WindowManager.h"

#define LOCTEXT_NAMESPACE "EditorGamesListWidget"

void SModioEditorUserGamesList::Construct(const FArguments& InArgs)
{
	// clang-format off
	ChildSlot
	[	
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SAssignNew(Root, SVerticalBox)
		]
	];
	// clang-format on
	DrawThrobber();
	LoadModioSubsystem();
}

void SModioEditorUserGamesList::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
									 const float InDeltaTime)
{
	if (ModioSubsystem && !ModioSubsystem->IsUsingBackgroundThread())
	{
		ModioSubsystem->RunPendingHandlers();
	}
}

void SModioEditorUserGamesList::LoadModioSubsystem()
{
	FModioInitializeOptions InitializeOptions =
		UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(FString("ModioUnrealEditor"));

	if (GEngine)
	{
		ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (ModioSubsystem)
		{
			ModioSubsystem->InitializeAsync(
				InitializeOptions,
				FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorUserGamesList::OnModioInitCallback));
		}
	}
}

void SModioEditorUserGamesList::OnModioInitCallback(FModioErrorCode ErrorCode)
{
	if (!ErrorCode ||
		UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::SDKAlreadyInitialized))
	{
		DrawGameList();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error while initialising mod.io subsystem: %d: %s"), ErrorCode.GetValue(),
			   *ErrorCode.GetErrorMessage());
	}
}

void SModioEditorUserGamesList::ClearWidget()
{
	if (Root.IsValid())
	{
		if (Root->GetAllChildren()->Num() > 0)
		{
			Root->ClearChildren();
		}
	}
}

void SModioEditorUserGamesList::DrawThrobber()
{
	ClearWidget();
	// clang-format off
	Root->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SCircularThrobber).Radius(50.f)
		];
	// clang-format on
}

void SModioEditorUserGamesList::DrawGameList()
{
	DrawThrobber();

	ModioSubsystem->ListUserGamesAsync(
		FModioFilterParams(), FOnListUserGamesDelegateFast::CreateLambda([&](const FModioErrorCode ErrorCode,
																			 TOptional<FModioGameInfoList> GamesList) {
			if (ErrorCode)
			{
				UE_LOG(LogTemp, Warning, TEXT("Error when getting user game list: %s"), *ErrorCode.GetErrorMessage());
				return;
			}
			if (GamesList.IsSet() && GamesList.GetValue().GetTotalResultCount() > 0)
			{
				Games.Empty();
				for (FModioGameInfo Game : GamesList.GetValue().InternalList)
				{
					Games.Add(MakeShared<FModioGameInfo>(Game));
				}

				ClearWidget();
				// clang-format off
				Root->AddSlot()
					.FillHeight(1.f)
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SNew(SBorder)
							.Padding(FMargin(5.f, 5.f, 5.f, 5.f))
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							[
								SAssignNew(GamesListView, SListView<TSharedPtr<FModioGameInfo>>)
								.HeaderRow
								(
									SNew(SHeaderRow)
									+ SHeaderRow::Column("Name")
									.DefaultLabel(LOCTEXT("GameListHeaderGames", "Name"))
									+ SHeaderRow::Column("ID")
									.DefaultLabel(LOCTEXT("GameListHeaderId", "ID"))
								)
								.SelectionMode(ESelectionMode::Single)
#if UE_VERSION_OLDER_THAN(5, 5, 0)
								.ItemHeight(32)
#endif
								.ListItemsSource(&Games)
								.OnGenerateRow(this, &SModioEditorUserGamesList::GenerateGameInfoRow)
								.OnSelectionChanged(this, &SModioEditorUserGamesList::OnGameSelectedFromList)
							]
					];
				// clang-format on
			}
		}));
}

TSharedRef<ITableRow> SModioEditorUserGamesList::GenerateGameInfoRow(TSharedPtr<FModioGameInfo> GameInfo,
																	 const TSharedRef<STableViewBase>& OwnerTable)
{
	// clang-format off
	return SNew(STableRow<TSharedPtr<FModioGameInfo>>, OwnerTable)
		[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(4.f, 2.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString(GameInfo.Get()->Name))
				]
				+ SHorizontalBox::Slot()
				.Padding(4.f, 2.f)
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString(GameInfo.Get()->GameID.ToString()))
				]
		];
	// clang-format on
}

void SModioEditorUserGamesList::OnGameSelectedFromList(TSharedPtr<FModioGameInfo> SelectedGame,
													   ESelectInfo::Type SelectInfo)
{
	OnGameSelected.ExecuteIfBound(FModioErrorCode(), *SelectedGame.Get());
}

#undef LOCTEXT_NAMESPACE
