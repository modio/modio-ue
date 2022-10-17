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

#include "Components/NamedSlotInterface.h"
#include "CoreMinimal.h"
#include "ModioUserWidgetBase.h"

#include "ModioUserWidgetNamedSlotBase.generated.h"

/**
 *
 */
UCLASS(abstract)
class MODIOUI_API UModioUserWidgetNamedSlotBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TMap<FName, UWidget*> NamedWidgets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TArray<FName> DesignerSpecifiedSlotNames;

	// UNamedSlotInterface Begin
	virtual void GetSlotNames(TArray<FName>& SlotNames) const override;
	virtual UWidget* GetContentForSlot(FName SlotName) const override;
	virtual void SetContentForSlot(FName SlotName, UWidget* Content) override;
	// UNamedSlotInterface End

	virtual void NativeGetSlotNames(TArray<FName>& SlotNames) const {};
};
