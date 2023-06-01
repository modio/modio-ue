/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "SlateBasics.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include <Objects/ModioBrowseModFileObject.h>
#include <Widgets/Views/STableRow.h>

class MODIOEDITOR_API SModFileRow : public SMultiColumnTableRow<TSharedPtr<FModioBrowseModFileStruct>>
{

public:

	SLATE_BEGIN_ARGS(SModFileRow) : _Item() {}
	SLATE_ARGUMENT(TSharedPtr<FModioBrowseModFileStruct>, Item)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

	TSharedPtr<FModioBrowseModFileStruct> Item;
};