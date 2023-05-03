/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioSpecifiedMaterialParams.h"
#include "Materials/MaterialInstance.h"

UMaterialInterface* UModioSpecifiedMaterialParams::GetMaterialInstance()
{
	return SpecifiedMaterial;
}

void UModioSpecifiedMaterialParams::PostLoad()
{
	Super::PostLoad();
}
