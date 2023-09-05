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
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "UObject/Interface.h"
/*
#include "IModioInputMappingAccessor.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioInputMappingAccessor : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioInputMappingAccessor : public IInterface
{
	GENERATED_BODY()


protected:

	UFUNCTION()
	virtual TArray<FString> GetInputMappingNames();
};

*/

class MODIOUI_API IModioInputMappingAccessor
{
public:

protected:
	TArray<FString> GetModioInputMappingNames();
	TArray<FString> GetAllProjectInputNames();
	TArray<FKey> GetModioInputKeys();
	TArray<FKey> GetKeyForModioInputName(const FName Name);
	TOptional<FKey> GetCommandKeyForEvent(const FKeyEvent& Event) const;
	TOptional<FKey> GetCommandKeyForPointerEvent(const FPointerEvent& Event) const;
	TOptional<FKey> GetCommandKeyForAnalogEvent(const FAnalogInputEvent& Event) const;
};

class MODIOUI_API IModioUIInputHandler : public IModioInputMappingAccessor
{
protected:
	virtual TSharedRef<const class FUICommandList> GetCommandList();
	virtual void BuildCommandList(TSharedRef<FUICommandList> InCommandList) {}

	virtual bool ProcessCommandForEvent(const struct FKeyEvent& Event);
	virtual bool ProcessCommandForEvent(const struct FPointerEvent& Event);
};