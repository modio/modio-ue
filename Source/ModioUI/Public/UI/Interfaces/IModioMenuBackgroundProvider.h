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
#include "Materials/MaterialInterface.h"
#include "UObject/Interface.h"

#include "IModioMenuBackgroundProvider.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioMenuBackgroundProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioMenuBackgroundProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual UMaterialInterface* NativeGetBackgroundMaterial()
		PURE_VIRTUAL(IModioMenuBackgroundProvider::NativeGetBackgroundMaterial, return {};);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioMenuBackgroundProvider")
	UMaterialInterface* GetBackgroundMaterial();

	UMaterialInterface* GetBackgroundMaterial_Implementation()
	{
		return NativeGetBackgroundMaterial();
	}
};
