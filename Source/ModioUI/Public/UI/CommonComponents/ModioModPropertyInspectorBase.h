// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Array.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UObject/FieldPath.h"

#include "ModioModPropertyInspectorBase.generated.h"

UENUM(BlueprintType)
enum class EModioModInfoPropertyType : uint8
{
	Name,
	Size,
	LastUpdated,
	ReleaseDate,
	Subscribers,
	CreatorName
};

USTRUCT(BlueprintType)
struct MODIOUI_API FModioModInfoPropertyDescriptor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Label;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EModioModInfoPropertyType Property;
};

/**
 *
 */
UCLASS()
class MODIOUI_API UModioModPropertyInspectorBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;

	virtual void NativeOnSetDataSource() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FModioModInfoPropertyDescriptor> PropertiesToDisplay;
	
	bool bRoutedUpdatePropertyDisplay = false;

	UFUNCTION(BlueprintCallable)
	void UpdatePropertyDisplay();

	virtual void NativeOnUpdatePropertyDisplay();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatePropertyDisplay();
};
