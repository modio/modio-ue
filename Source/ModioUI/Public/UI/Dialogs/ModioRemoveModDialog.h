// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioTextBlock.h"
#include "UI/Dialogs/ModioDialogBase.h"

#include "ModioRemoveModDialog.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioRemoveModDialog : public UModioDialogBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioTextBlock* ModName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioTextBlock* PromptMessage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* BackButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioButton* ConfirmButton;
};
