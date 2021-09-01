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
#include "Types/ModioModCollectionEntry.h"
#include "Internal/ModioPrivateDefines.h"
#include "Internal/ModioConvert.h"
#include "Internal/Convert/ModInfo.h"
#include "ModioSDK.h"


MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE EModioModState ToUnreal(Modio::ModState ModState)
{
	switch (ModState)
	{
		case Modio::ModState::InstallationPending:
			return EModioModState::InstallationPending;
		case Modio::ModState::Installed:
			return EModioModState::Installed;
		case Modio::ModState::UpdatePending:
			return EModioModState::UpdatePending;
		case Modio::ModState::Downloading:
			return EModioModState::Downloading;
		case Modio::ModState::Extracting:
			return EModioModState::Extracting;
		case Modio::ModState::UninstallPending:
			return EModioModState::UninstallPending;
	}

	checkf(false, TEXT("Missed a case in ToUnreal(Modio::ModState ModState)"));
	return EModioModState::InstallationPending;
}
MODIO_END_CONVERT_SWITCHES

FModioModCollectionEntry ToUnreal(const Modio::ModCollectionEntry& In)
{
	FModioModCollectionEntry Out;
	Out.ModState = ToUnreal(In.GetModState());
	Out.ModID = ToUnreal(In.GetID());
	Out.ModPath = ToUnreal(In.GetPath());
	Out.ModProfile = ToUnreal(In.GetModProfile());
	return Out;
}

