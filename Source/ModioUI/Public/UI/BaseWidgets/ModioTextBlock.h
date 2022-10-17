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

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioTextBlock.generated.h"

/**
* Class definition for a Modio text block with simple static string
**/
UCLASS()
class MODIOUI_API UModioTextBlock : public UTextBlock
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ShowOnlyInnerProperties), Category = "Appearance")
	FModioUIStyleRef StyleRef = {FName("DefaultTextStyle")};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	bool bOverrideGlobalStyle = false;

	void ApplyStyleOverrides();
	virtual void SynchronizeProperties() override;
};
