/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Dialog/ModioCommonDialogInfo.h"

void UModioCommonDialogInfo::HandleDialogButtonClicked(EModioCommonDialogButtonType ClickedButtonType)
{
	OnDialogButtonClicked.Broadcast(ClickedButtonType);
	OnDialogButtonClickedFast.Broadcast(ClickedButtonType);
}

