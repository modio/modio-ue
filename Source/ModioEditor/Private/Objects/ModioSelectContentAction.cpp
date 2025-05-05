/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Objects/ModioSelectContentAction.h"

#include "ModioEditorUtilityFunctions.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioSelectContentAction)

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