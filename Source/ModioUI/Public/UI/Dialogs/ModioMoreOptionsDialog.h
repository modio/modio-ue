// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioTileView.h"
#include "UI/Dialogs/ModioDialogBase.h"

#include "ModioMoreOptionsDialog.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioMoreOptionsDialog : public UModioDialogBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioTileView* OptionsList;
};
