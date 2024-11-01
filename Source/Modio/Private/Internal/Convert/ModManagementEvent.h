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
#include "Internal/ModioConvert.h"
#include "Internal/ModioPrivateDefines.h"
#include "ModioSDK.h"
#include "Types/ModioModManagementEvent.h"

MODIO_BEGIN_CONVERT_SWITCHES
FORCEINLINE EModioModManagementEventType ToUnreal(Modio::ModManagementEvent::EventType Event)
{
	switch (Event)
	{
		case Modio::ModManagementEvent::EventType::BeginInstall:
			return EModioModManagementEventType::BeginInstall;
		case Modio::ModManagementEvent::EventType::Installed:
			return EModioModManagementEventType::Installed;
		case Modio::ModManagementEvent::EventType::BeginUninstall:
			return EModioModManagementEventType::BeginUninstall;
		case Modio::ModManagementEvent::EventType::Uninstalled:
			return EModioModManagementEventType::Uninstalled;
		case Modio::ModManagementEvent::EventType::BeginUpdate:
			return EModioModManagementEventType::BeginUpdate;
		case Modio::ModManagementEvent::EventType::Updated:
			return EModioModManagementEventType::Updated;
		case Modio::ModManagementEvent::EventType::BeginUpload:
			return EModioModManagementEventType::BeginUpload;
		case Modio::ModManagementEvent::EventType::Uploaded:
			return EModioModManagementEventType::Uploaded;
	}

	checkf(false, TEXT("Missed a case in ToUnreal(Modio::ModManagementEvent::EventType Event)"));
	return EModioModManagementEventType::Installed;
}
MODIO_END_CONVERT_SWITCHES

FORCEINLINE FModioModManagementEvent ToUnreal(const Modio::ModManagementEvent& In)
{
	FModioModManagementEvent Out;
	Out.ID = ToUnreal(In.ID);
	Out.Event = ToUnreal(In.Event);
	Out.Status = ToUnreal(In.Status);
	return Out;
}