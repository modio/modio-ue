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

#include "UObject/Interface.h"

#include "IModioUIUserStringListEntry.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIUserStringListEntry : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIUserStringListEntry : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeSetListItemString(const FString& InString) {};

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIUserStringListEntry")
	void SetListItemString(const FString& InString);

	void SetListItemString_Implementation(const FString& InString)
	{
		NativeSetListItemString(InString);
	}
};