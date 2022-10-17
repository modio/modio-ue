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

#include "IModioUIDataSourceWidget.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIDataSourceWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIDataSourceWidget : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeSetDataSource(UObject* InDataSource) {};

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIDataSourceWidget")
	void SetDataSource(UObject* InDataSource);

	void SetDataSource_Implementation(UObject* InDataSource);
};