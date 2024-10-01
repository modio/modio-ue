/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Objects/ModioSelectContentAction.h"

#include "ModioEditorUtilityFunctions.h"

bool UModioSelectContentAction::ExecuteAction(FString Args) const
{
	if (!Args.IsEmpty())
	{
		TArray<FString> Content;
		Content.Add(Args);
		UModioEditorUtilityFunctions::SelectAssetsInContentBrowser(Content);
		return true;
	}
	return false;
}