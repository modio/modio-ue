#pragma once

#include "Misc/EnumClassFlags.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "UObject/Interface.h"

#include "IModioUIModInfoReceiver.generated.h"

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIModInfoEventType : uint8
{
	ListAllMods,
	GetModInfo
};

ENUM_CLASS_FLAGS(EModioUIModInfoEventType);

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIModInfoReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIModInfoReceiver : public IInterface
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
