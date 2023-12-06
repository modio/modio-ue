/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ModioCommonActivatableWidgetStack.generated.h"

/**
 * @brief Widget Stack that can be used in Mod.io UI
 */
UCLASS()
class MODIOUI5_API UModioCommonActivatableWidgetStack : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:
	/** Adds an activatable widget to the container. See BP_AddWidgetSmart for more info. */
	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* AddWidgetSmart(TSubclassOf<ActivatableWidgetT> ActivatableWidgetClass)
	{
		for (UCommonActivatableWidget* Widget : WidgetList)
		{
			if (Widget->GetClass() == ActivatableWidgetClass)
			{
				if (Widget == GetActiveWidget())
				{
					return Cast<ActivatableWidgetT>(Widget);
				}

				RemoveWidget(*Widget);
				return AddWidget<ActivatableWidgetT>(ActivatableWidgetClass);

				// TODO: Do more things here if needed
			}
		}
		return AddWidget<ActivatableWidgetT>(ActivatableWidgetClass);
	}

	/**
	 * Alternative to BP_AddWidget, but is more optimized to avoid creating duplicate widgets
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Widget Stack", meta = (DeterminesOutputType = ActivatableWidgetClass, DisplayName = "Push Widget Smart"))
    UCommonActivatableWidget* BP_AddWidgetSmart(TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass);
};