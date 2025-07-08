/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

class UModioCreateModParamsObject;
struct FModioModInfo;

class MODIOEDITOR_API ModioCreateModParamsDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	void DrawCreateMod(IDetailLayoutBuilder& DetailBuider);

private:
	UModioCreateModParamsObject* Target;
	TSharedPtr<SEditableTextBox> PathToLogoFileEditableTextBox;
	TSharedPtr<SEditableTextBox> NameEditableTextBox;
	TSharedPtr<SMultiLineEditableTextBox> SummaryMultiLineEditableTextBox;
};