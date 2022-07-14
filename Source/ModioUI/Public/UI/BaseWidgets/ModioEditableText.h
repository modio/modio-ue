// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/EditableText.h"
#include "CoreMinimal.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioEditableText.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioEditableText : public UEditableText
{
	GENERATED_BODY()
	protected:
	virtual void SynchronizeProperties() override;
	UFUNCTION()
	TArray<FString> GetStyleNames() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance", meta = (StyleClass = "ModioRichTextStyle"))
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance", meta = (GetOptions = "GetStyleNames"))
	FName DefaultStyleName = FName("default");
};
