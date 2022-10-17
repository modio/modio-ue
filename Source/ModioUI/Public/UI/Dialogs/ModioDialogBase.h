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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioDialogBase.generated.h"

/**
* Modio UI element that provides the basic functionality for a dialog
* element, in other words, a widget that displays constrained content
* to authentication, reports or uninstall operations for example
**/
UCLASS()
class MODIOUI_API UModioDialogBase : public UModioUserWidgetBase
{
	GENERATED_BODY()

public:
	/**
	* Function to initialize the properties in this class
	* @return Bool True when intialization performed successfully, False otherwise
	**/
	bool Initialize() override;

	/**
	* Stored property of the dialog UI style to use in this class, with "DefaultDialogStyle" by default
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FModioUIStyleRef DialogStyle = {FName("DefaultDialogStyle")};
};
