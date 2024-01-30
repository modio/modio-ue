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
 * Widget Stack that can be used in Mod.io UI
 * It provides some extra functionality to avoid creating duplicate widgets
 */
UCLASS(Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonActivatableWidgetStack : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	/**
	 * Optional widget to auto-generate as the preview root element of the stack
	 */
	UPROPERTY(EditAnywhere, Category = "Mod.io Common UI")
	TSubclassOf<UCommonActivatableWidget> PreviewContentWidgetClass;

	/**
	 * The preview widget that was generated
	 */
	UPROPERTY(Transient)
	TObjectPtr<UCommonActivatableWidget> PreviewContentWidget;
#endif

	//~ Begin UUserWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UUserWidget Interface

	/**
	 * Adds a widget to the stack, but is more optimized to avoid creating duplicate widgets. See BP_AddWidgetSmart for more info
	 * 
	 * @tparam ActivatableWidgetT The type of widget to add
	 * @param ActivatableWidgetClass The class of widget to add
	 * @param bSkipIfAlreadyActive If true, will not add the widget if it is already active, and will return the active widget
	 * @param bRemoveIfInStack If true, will remove the widget from the stack and re-add it
	 * @return The widget that was added
	 */
	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* AddWidgetSmart(TSubclassOf<ActivatableWidgetT> ActivatableWidgetClass, bool bSkipIfAlreadyActive, bool bRemoveIfInStack)
	{
		if (bSkipIfAlreadyActive || bRemoveIfInStack)
		{
			for (UCommonActivatableWidget* Widget : WidgetList)
			{
				if (Widget->GetClass() == ActivatableWidgetClass)
				{
					if (bSkipIfAlreadyActive && Widget == GetActiveWidget())
					{
						return Cast<ActivatableWidgetT>(Widget);
					}

					if (bRemoveIfInStack)
					{
						RemoveWidget(*Widget);
						return AddWidget<ActivatableWidgetT>(ActivatableWidgetClass);
					}

					// TODO: Do more things here if needed
				}
			}
		}
		return AddWidget<ActivatableWidgetT>(ActivatableWidgetClass);
	}

	/**
	 * Alternative to BP_AddWidget, but is more optimized to avoid creating duplicate widgets
	 * 
	 * @param ActivatableWidgetClass The class of widget to add
	 * @param bSkipIfAlreadyActive If true, will not add the widget if it is already active, and will return the active widget
	 * @param bRemoveIfInStack If true, will remove the widget from the stack and re-add it
	 * @return The widget that was added
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Widget Stack", meta = (DeterminesOutputType = ActivatableWidgetClass, DisplayName = "Push Widget Smart"))
    UCommonActivatableWidget* BP_AddWidgetSmart(TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass, bool bSkipIfAlreadyActive, bool bRemoveIfInStack);
};