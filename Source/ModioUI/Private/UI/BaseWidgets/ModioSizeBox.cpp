/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioSizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "UI/BaseWidgets/Slate/SModioBox.h"

TSharedRef<SWidget> UModioSizeBox::RebuildWidget()
{
	// Passing aspect ratio settings through in the call to Construct because that's where we set ModioBox specific
	// versions of those variables
	MySizeBox = SNew(SModioBox).MinAspectRatio(MinAspectRatio).MaxAspectRatio(MaxAspectRatio);

	if (GetChildrenCount() > 0)
	{
		Cast<USizeBoxSlot>(GetContentSlot())->BuildSlot(MySizeBox.ToSharedRef());
	}

	MySizeBox->SetHAlign(HAlign);
	MySizeBox->SetVAlign(VAlign);
	return MySizeBox.ToSharedRef();
}
