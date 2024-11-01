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
#include "SlateBasics.h"

class UModioBrowseModsObject;
class SSearchBox;
struct FModioModInfo;

class MODIOEDITOR_API ModioBrowseModsParamsDetails : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	void DrawEditMod(IDetailLayoutBuilder& DetailBuilder);

private:

	UModioBrowseModsObject* Target;
	TSharedPtr<SSearchBox> SearchBox;
	TSharedPtr<SListView<TSharedPtr<FModioModInfo>>> ListView;
	TArray<TSharedPtr<FModioModInfo>> OriginalSource;
	TArray<TSharedPtr<FModioModInfo>> Source;
	TArray<TSharedPtr<FModioModInfo>> SearchFilter;
};