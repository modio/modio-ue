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
#include "Blueprint/UserWidget.h"

#include "ModioPopupContainer.generated.h"


class UModioPopupBase;

/**
 * Caching widget for popup items. Tries to reuse existing widget to not tax the
 * garbage collector if one has already been created
 */
UCLASS()
class UModioPopupContainer : public UUserWidget
{
	GENERATED_BODY()
public:
	UModioPopupContainer(const FObjectInitializer& ObjectInitializer);

	/**
	 * Push a new popup on the stack and returns a new instance of it
	 * Override this function to do your custom handling of displaying the popup
	 * @param PopupClass - will warn on null and not do anything, else we will return a instance
	 * of a popup of that class
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|Widget",
			  meta = (DeterminesOutputType = PopupClass))
	UModioPopupBase* PushPopup(TSubclassOf<UModioPopupBase> PopupClass);

	/**
	 * @brief Pops a topmost item on the popup stack
	 * @param PopupClass - The class we want to pop, null if we just want to pop the topmost on the stack
	 * @return the next popup on the stack
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|Widget")
	UModioPopupBase* PopPopup(TSubclassOf<UModioPopupBase> PopupClass);

private:
	UPROPERTY(Transient)
	TArray<UModioPopupBase*> PopupStack;

	UPROPERTY(Transient)
	TArray<UModioPopupBase*> PopupCache;
};
