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

#include "Styling/SlateBrush.h"
#include "UObject/Interface.h"

#include "IModioUIImageDisplay.generated.h"

UINTERFACE(BlueprintType)
class MODIOUI_API UModioUIImageDisplay : public UInterface
{
	GENERATED_BODY()
};

class MODIOUI_API IModioUIImageDisplay : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeDisplayImage(const FSlateBrush& Image);

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ModioUIImageDisplay")
	void DisplayImage(const FSlateBrush& Image);

	void DisplayImage_Implementation(const FSlateBrush& Image)
	{
		NativeDisplayImage(Image);
	}
};