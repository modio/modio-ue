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

#include "Containers/Map.h"
#include "Editor/ModioDetailChildrenBuilderBase.h"
#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "Internationalization/Text.h"
#include "PropertyHandle.h"
#include "UObject/NameTypes.h"
#include "UObject/NoExportTypes.h"

class FModioDetailChildrenBuilder_PropertyRowHook
	: public FModioDetailChildrenBuilderBase<FModioDetailChildrenBuilder_PropertyRowHook>
{
	TMap<FString, IDetailPropertyRow*> Properties;

public:
	FModioDetailChildrenBuilder_PropertyRowHook(IDetailChildrenBuilder& ParentBuilder)
		: FModioDetailChildrenBuilderBase(ParentBuilder) {};

	class IDetailPropertyRow& AddProperty(TSharedRef<IPropertyHandle> PropertyHandle) override
	{
		IDetailPropertyRow& PropertyRow = ParentBuilder.AddProperty(PropertyHandle);
		Properties.Add(PropertyHandle->GeneratePathToProperty(), &PropertyRow);
		return PropertyRow;
	}

	class IDetailPropertyRow* GetExistingPropertyRow(FString PropertyPath)
	{
		if (Properties.Contains(PropertyPath))
		{
			return Properties[PropertyPath];
		}
		return nullptr;
	}
};