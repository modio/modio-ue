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

#include "Misc/EnumClassFlags.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "UObject/Interface.h"

#include "IModioUIModInfoReceiver.generated.h"

/**
* Enumerator of the information envents
**/
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIModInfoEventType : uint8
{
	/** Event to list all mods **/
	ListAllMods,

	/** Event to retrieve the mod information **/
	GetModInfo
};

ENUM_CLASS_FLAGS(EModioUIModInfoEventType);

UINTERFACE(BlueprintType)
class MODIOUICORE_API UModioUIModInfoReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModInfoReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUIModInfoReceiver = false;

	void ModInfoRequestHandler(FModioModID ModID, FModioErrorCode ec, TOptional<FModioModInfo> Info);
	void ListAllModsRequestHandler(FString RequestIdentifier, FModioErrorCode ec, TOptional<FModioModInfoList> List);

protected:
	template<typename ImplementingClass>
	void Register(EModioUIModInfoEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::ListAllMods))
			{
				Subsystem->OnListAllModsRequestCompleted.AddUObject(
					Cast<ImplementingClass>(this), &IModioUIModInfoReceiver::ListAllModsRequestHandler);
			}
			if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::GetModInfo))
			{
				Subsystem->OnModInfoRequestCompleted.AddUObject(Cast<ImplementingClass>(this),
																&IModioUIModInfoReceiver::ModInfoRequestHandler);
			}
		}
	}

	virtual void NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ec, TOptional<FModioModInfo> Info);

	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
													 TOptional<FModioModInfoList> List);

	UFUNCTION(BlueprintImplementableEvent)
	void OnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ec, FModioOptionalModInfo Info);

	UFUNCTION(BlueprintImplementableEvent)
	void OnListAllModsRequestCompleted(const FString& RequestIdentifier, FModioErrorCode ec,
									   FModioOptionalModInfoList List);
};
