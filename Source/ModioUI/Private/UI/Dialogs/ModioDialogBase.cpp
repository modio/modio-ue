/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Dialogs/ModioDialogBase.h"
#include "Blueprint/WidgetTree.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"

bool UModioDialogBase::Initialize()
{
	bool bSuperResult = Super::Initialize();
	if (bSuperResult)
	{
		UWidget* Root = WidgetTree->ConstructWidget<UModioDialogBaseInternal>();
		WidgetTree->RootWidget = Root;
	}
	return bSuperResult;
}
