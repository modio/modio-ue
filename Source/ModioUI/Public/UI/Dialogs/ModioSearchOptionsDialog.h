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
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/CommonComponents/ModioTabController.h"
#include "UI/Dialogs/ModioDialogBase.h"

#include "ModioSearchOptionsDialog.generated.h"

/**
* Modio UI element that represents options in a dialog for mod search, it
* provides a tag selector and two buttons to interact with this instance
**/
UCLASS()
class MODIOUI_API UModioSearchOptionsDialog : public UModioDialogBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioTabController* TagCategorySelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioListView* TagList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioButton* ApplyButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioButton* MoreOptionsButton;
};
