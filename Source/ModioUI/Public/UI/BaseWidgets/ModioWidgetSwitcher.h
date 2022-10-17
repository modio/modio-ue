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

#include "Components/WidgetSwitcher.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"

#include "ModioWidgetSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIndexChanged, int32, Index);

UCLASS()
class MODIOUI_API UModioWidgetSwitcher : public UWidgetSwitcher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Widgets")
	FOnIndexChanged OnActiveWidgetChanged;
	TArray<int32> ActiveWidgetIndexStack;

public:
	virtual void PushActiveWidgetIndex(int32 Index);
	virtual int32 PopActiveWidgetIndex();
	virtual void SetActiveWidgetIndex(int32 Index) override;

	virtual FOnIndexChanged& GetActiveWidgetChangedDelegate()
	{
		return OnActiveWidgetChanged;
	}
};
