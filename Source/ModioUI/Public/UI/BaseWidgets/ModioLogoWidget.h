// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UObject/Object.h"
#include "ModioLogoWidget.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioLogoWidget : public UWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UObject *DataSource;
	
protected:
	TSharedPtr<class SImage> MyImage;

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widgets")
	FModioUIStyleRef StyleRef = {FName("DefaultLogoTextStyle") };
};
