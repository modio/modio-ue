/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Tab/ModioCommonTabButtonBase.h"

#include "ModioUI.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabButtonStyle.h"

void UModioCommonTabButtonBase::SetStyle(TSubclassOf<UModioCommonTabButtonStyle> InStyle)
{
	TSubclassOf<UModioCommonButtonStyle> ParentStyle = InStyle;
	Super::SetStyle(ParentStyle);
}

void UModioCommonTabButtonBase::SetTabLabelInfo_Implementation(const FModioCommonTabDescriptor& InTabLabelInfo)
{
	IModioCommonTabButtonInterface::SetTabLabelInfo_Implementation(InTabLabelInfo);
	UE_LOG(ModioUI, Verbose, TEXT("Setting tab label info for '%s' to '%s'"), *GetName(), *InTabLabelInfo.TabText.ToString());
	SetLabel(InTabLabelInfo.TabText);
}
