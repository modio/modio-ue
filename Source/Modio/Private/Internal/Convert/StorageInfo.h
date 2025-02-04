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
#include "Types/ModioStorageInfo.h"

FORCEINLINE EModioStorageLocation ToUnreal(const Modio::StorageLocation& In)
{
	switch (In)
	{
		case Modio::StorageLocation::Local:
			return EModioStorageLocation::Local;
	}
	UE_LOG(LogModio, Error, TEXT("Unhandled case in ToUnreal(EModioStorageLocation)"));
	return EModioStorageLocation::Local;
}

FORCEINLINE EModioStorageUsage ToUnreal(const Modio::StorageUsage& In)
{
	switch (In)
	{
		case Modio::StorageUsage::Consumed:
			return EModioStorageUsage::Consumed;
		case Modio::StorageUsage::Available:
			return EModioStorageUsage::Available;
	}
	UE_LOG(LogModio, Error, TEXT("Unhandled case in ToUnreal(EModioStorageUsage)"));
	return EModioStorageUsage::Consumed;
}

FORCEINLINE FModioStorageInfo ToUnreal(const Modio::StorageInfo& In)
{
	FModioStorageInfo Out;

	for (const EModioStorageLocation Location : TEnumRange<EModioStorageLocation>())
	{
		for (const EModioStorageUsage Usage : TEnumRange<EModioStorageUsage>())
		{
			Modio::FileSize Space = In.GetSpace(ToModio(Location), ToModio(Usage));
			SetSpace(Out, Location, Usage, ToUnreal(Space));
		}
	}

	return Out;
}

FORCEINLINE void SetSpace(FModioStorageInfo& Info, EModioStorageLocation Location, EModioStorageUsage Usage,
						  FModioUnsigned64 Space)
{
	Info.StorageData.FindOrAdd(FStorageInfoKey {Location, Usage}) = Space;
}

Modio::StorageLocation ToModio(EModioStorageLocation Location)
{
	switch (Location)
	{
		case EModioStorageLocation::Local:
			return Modio::StorageLocation::Local;
	}

	checkf(false, TEXT("Missed a case in ToModio(EStorageLocation Location)"));
	return Modio::StorageLocation::Local;
}

Modio::StorageUsage ToModio(EModioStorageUsage Usage)
{
	switch (Usage)
	{
		case EModioStorageUsage::Available:
			return Modio::StorageUsage::Available;
		case EModioStorageUsage::Consumed:
			return Modio::StorageUsage::Consumed;
	}

	checkf(false, TEXT("Missed a case in ToModio(EStorageUsage Usage)"));
	return Modio::StorageUsage::Available;
}