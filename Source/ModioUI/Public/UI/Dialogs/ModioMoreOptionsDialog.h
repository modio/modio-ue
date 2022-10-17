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
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/Dialogs/ModioDialogBase.h"

#include "ModioMoreOptionsDialog.generated.h"

/**
* Base class that displays a dialog view when more options are available
**/
UCLASS()
class MODIOUI_API UModioMoreOptionsDialog : public UModioDialogBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioTileView* OptionsList;
};
