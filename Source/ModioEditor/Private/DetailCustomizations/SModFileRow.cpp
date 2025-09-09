/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "DetailCustomizations/SModFileRow.h"
#include "Framework/Views/ITypedTableView.h"
#include "Misc/EngineVersionComparison.h"
#include "Widgets/SModioEditorWindowCompoundWidget.h"
#include "WindowManager.h"

void SModFileRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Item = InArgs._Item;

	FSuperRowType::Construct(FSuperRowType::FArguments().Padding(0), InOwnerTableView);
}

TSharedRef<SWidget> SModFileRow::GenerateWidgetForColumn(const FName& ColumnName)
{
#if ENGINE_MAJOR_VERSION >= 5
	static const UEnum* PlatformEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModfilePlatform"));
#else
	static const UEnum* PlatformEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EModioModfilePlatform"));
#endif
	FString PlatformString = PlatformEnum->GetNameStringByIndex(static_cast<uint32>(Item->Platform));

	if (ColumnName == TEXT("Name"))
	{
		return SNew(SHorizontalBox)

			   + SHorizontalBox::Slot().Padding(
					 FMargin(5.f, 10.f, 0.f, 10.f))[SNew(STextBlock).Text(FText::FromString(Item->Name))];
	}
	else if (ColumnName == TEXT("Platform"))
	{
		return SNew(SHorizontalBox)

			   + SHorizontalBox::Slot().Padding(
					 FMargin(5.f, 10.f, 0.f, 10.f))[SNew(STextBlock).Text(FText::FromString(PlatformString))];
	}
	else if (ColumnName == TEXT("Version"))
	{
		return SNew(SHorizontalBox)

			   + SHorizontalBox::Slot().Padding(
					 FMargin(5.f, 10.f, 0.f, 10.f))[SNew(STextBlock).Text(FText::FromString(Item->Version))];
	}
	else if (ColumnName == TEXT("Status"))
	{
		return SNew(SHorizontalBox)

			   + SHorizontalBox::Slot().Padding(
					 FMargin(5.f, 10.f, 0.f, 10.f))[SNew(STextBlock).Text(FText::FromString(Item->Status))];
	}
	else
	{
		return SNew(STextBlock);
	}
}
