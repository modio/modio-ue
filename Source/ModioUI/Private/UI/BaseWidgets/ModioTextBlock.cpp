/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioTextBlock.h"
#include "Settings/ModioUISettings.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/Text/STextBlock.h"

void UModioTextBlock::ApplyStyleOverrides()
{
	if (!bOverrideGlobalStyle)
	{
		UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
		// Could we load the default style set?
		if (Settings && !Settings->DefaultStyleSet.IsNull()) {}
		// Couldn't resolve the default style set so don't do anything
	}
}

void UModioTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const FTextBlockStyle* const ResolvedStyle = StyleRef.FindStyle<FTextBlockStyle>();
	if (ResolvedStyle)
	{
		MyTextBlock->SetTextStyle(ResolvedStyle);
		MyTextBlock->SetFont(ResolvedStyle->Font);
		MyTextBlock->SetColorAndOpacity(ResolvedStyle->ColorAndOpacity);
		MyTextBlock->SetStrikeBrush(&ResolvedStyle->StrikeBrush);
		MyTextBlock->SetShadowOffset(ResolvedStyle->ShadowOffset);
		MyTextBlock->SetShadowColorAndOpacity(ResolvedStyle->ShadowColorAndOpacity);
	}

	// ApplyStyleOverrides();
}
