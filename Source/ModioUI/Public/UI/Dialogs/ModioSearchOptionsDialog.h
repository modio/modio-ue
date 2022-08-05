// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/CommonComponents/ModioTabController.h"
#include "UI/Dialogs/ModioDialogBase.h"

#include "ModioSearchOptionsDialog.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioSearchOptionsDialog : public UModioDialogBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioTabController* TagCategorySelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* TagList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* ApplyButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* MoreOptionsButton;
};
