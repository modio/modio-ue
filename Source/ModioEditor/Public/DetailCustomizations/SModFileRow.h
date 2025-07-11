/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Framework/Views/ITypedTableView.h"
#include "Framework/Views/TableViewMetadata.h"
#include "Objects/ModioBrowseModFileObject.h"
#include "SlateBasics.h"
#include "SlateFwd.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"

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