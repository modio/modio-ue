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

class UModioBrowseModFileCollectionObject;
class SSearchBox;
class SModFileRow;
struct FModioModInfo;
struct FModioBrowseModFileStruct;

class MODIOEDITOR_API ModioBrowseModFileDetails : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	void DrawBrowseModFile(IDetailLayoutBuilder& DetailBuilder);

private:

	UModioBrowseModFileCollectionObject* Target;
	TSharedPtr<SListView<TSharedPtr<FModioBrowseModFileStruct>>> ListView;
	TArray<TSharedPtr<FModioBrowseModFileStruct>> Source;
};