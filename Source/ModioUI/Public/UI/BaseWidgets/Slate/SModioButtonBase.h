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

#include "Widgets/Input/SButton.h"
#include "Framework/Application/SlateApplication.h"
#include "ModioUI.h"

class MODIOUI_API SModioButtonBase : public SButton
{
public:
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override 
	{
		bIsFocusedByController = true;
#if UE_VERSION_OLDER_THAN(5, 0, 0)
		FSlateApplication::Get().PlaySound(Style->HoveredSlateSound);
#else
		// Implement hovered sounds for UE5
#endif
		return FReply::Handled();
	}
	
#if UE_VERSION_OLDER_THAN(5, 0, 0)
	virtual bool IsHovered() const override
	{
		return bIsFocusedByController || SButton::IsHovered();
	}
#endif

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override {
		bIsFocusedByController = false;
	}

private:
	bool bIsFocusedByController;
};
