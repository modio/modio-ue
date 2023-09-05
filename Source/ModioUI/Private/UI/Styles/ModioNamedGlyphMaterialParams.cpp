/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioNamedGlyphMaterialParams.h"

#include "ModioUI4Subsystem.h"
#include "Algo/Transform.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

TArray<FString> UModioNamedGlyphMaterialParams::GetValidGlyphNames()
{
	TArray<FString> OutNames;
	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			TArray<FName> GlyphNames = DefaultStyleSet->GetGlyphNames();

			Algo::Transform(GlyphNames, OutNames, [](const FName& InName) { return InName.ToString(); });
		}
	}
	return OutNames;
}

UMaterialInterface* UModioNamedGlyphMaterialParams::GetMaterialInstance()
{
	if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			return DefaultStyleSet->GetGlyphMaterial(GlyphName);
		}
	}
	return nullptr;
}
