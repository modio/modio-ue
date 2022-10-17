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

#include "UObject/Object.h"
#include "Widgets/Views/SListView.h"

template<typename ItemType = UObject*>
class MODIOUI_API SModioListView : public SListView<ItemType>
{
public:
	using FOnSelectionChanged = typename SListView<ItemType>::FOnSelectionChanged;

	FOnSelectionChanged& GetOnSelectionChanged()
	{
		return SListView<ItemType>::OnSelectionChanged;
	}
};
