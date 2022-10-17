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

#include "Components/SizeBox.h"
#include "CoreMinimal.h"

#include "ModioSizeBox.generated.h"

/**
* Class definition for a Modio size box which contains horizontal and vertical alignment
**/
UCLASS()
class MODIOUI_API UModioSizeBox : public USizeBox
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Content")
	TEnumAsByte<EHorizontalAlignment> HAlign;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Content")
	TEnumAsByte<EVerticalAlignment> VAlign;
};
