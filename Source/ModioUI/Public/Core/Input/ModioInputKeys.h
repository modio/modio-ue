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

#include "Algo/Transform.h"
#include "InputCoreTypes.h"
#include "UObject/NameTypes.h"

const FName NAME_ModioInputKeys(TEXT("ModioInputKeys"));

// NOTE: If you add an entry here, you need to update IModioInputMappingAccessor::GetModioInputMappingNames, *and* make
// sure you add the key in FModioUIModule::StartupModule

struct MODIOUI_API FModioInputNames
{
	static const FName Up;
	static const FName Down;
	static const FName Left;
	static const FName Right;
	static const FName Previous;
	static const FName Next;
	static const FName MoreOptions;
	static const FName RefineSearch;
	static const FName Confirm;
	static const FName Back;
	static const FName Subscribe;
	static const FName DownloadQueue;
	static const FName EnableDisableMod;
	static const FName Collapse;

	static TArray<FName> GetAll()
	{
		return {Up,          Down,         Left,    Right, Previous,  Next,
				MoreOptions, RefineSearch, Confirm, Back,  Subscribe, DownloadQueue, Collapse, EnableDisableMod};
	}
	static TArray<FString> GetAllStrings()
	{
		TArray<FString> Strings;
		Algo::Transform(GetAll(), Strings, [](const FName& InName) { return InName.ToString(); });
		return Strings;
	}
};

struct MODIOUI_API FModioInputKeys
{
	static const FKey Up;
	static const FKey Down;
	static const FKey Left;
	static const FKey Right;
	static const FKey Previous;
	static const FKey Next;
	static const FKey MoreOptions;
	static const FKey RefineSearch;
	static const FKey Confirm;
	static const FKey Back;
	static const FKey Subscribe;
	static const FKey DownloadQueue;
	static const FKey EnableDisableMod;
	static const FKey Collapse;

	static TArray<FKey> GetAll()
	{
		return {Up,          Down,         Left,    Right, Previous,  Next,
				MoreOptions, RefineSearch, Confirm, Back,  Subscribe, DownloadQueue, Collapse, EnableDisableMod};
	}
};