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
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioTabController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentTabChanged, int64, TabIndex, UObject*, AssociatedItem);

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTabController : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	bool bRoutedOnCurrentTabChanged = false;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioInputBindingImage* NavigatePreviousHint;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioTileView* TabButtons;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioInputBindingImage* NavigateNextHint;

	virtual void NativeOnCurrentTabChanged(int64 TabIndex, UObject* AssociatedItem);

	virtual void HandleItemSelectionChanged(UObject* Item);

	// This handler may not be required
	virtual void HandleItemClicked(UObject* Item);

	virtual void NativeConstruct() override;

	UFUNCTION()
	ESlateVisibility GetInputHintVisibility(EModioUIInputMode InputMode);

public:
	UFUNCTION(BlueprintCallable, Category = "ModioTabController")
	void SetTabItems(TArray<UObject*> NewItems);

	UFUNCTION(BlueprintCallable, Category = "ModioTabController")
	void SetCurrentTab(int64 TabIndex);

	UFUNCTION(BlueprintCallable, Category = "ModioTabController")
	void NextTab();

	UFUNCTION(BlueprintCallable, Category = "ModioTabController")
	void PrevTab();

	UPROPERTY(BlueprintAssignable, Category = "ModioTabController")
	FOnCurrentTabChanged OnCurrentTabChanged;
};
