// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioDialogBase.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDialogBase : public UModioUserWidgetBase
{
	GENERATED_BODY()

public:
	bool Initialize() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef DialogStyle = {FName("DefaultDialogStyle")};
};
