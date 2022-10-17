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

#include "Containers/UnrealString.h"
#include "UObject/Interface.h"
#include "UObject/NameTypes.h"

#include "IModioUIStyleRefSerializer.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIStyleRefSerializer : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIStyleRefSerializer : public IInterface
{
	GENERATED_BODY()
public:
	virtual void NativeSerializeStyleReference(FString PathToProperty, FName StyleElementReference);
};