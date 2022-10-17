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

#include "Containers/Array.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UObject/FieldPath.h"

#include "ModioModPropertyInspectorBase.generated.h"

/**
* Enumerator for the types a ModInfo could have as properties
**/
UENUM(BlueprintType)
enum class EModioModInfoPropertyType : uint8
{
	/** ModInfo name **/
	Name,

	/** ModInfo size **/
	Size,

	/** ModInfo date when it was updated **/
	LastUpdated,

	/** ModInfo date when it was released **/
	ReleaseDate,

	/** How many subcribers this mod has **/
	Subscribers,

	/** The nick name of the creator **/
	CreatorName
};

/**
* Strong type struct to store the property metadata of a ModInfo
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioModInfoPropertyDescriptor
{
	GENERATED_BODY()
	
	/**
	* Stored property of the label for this descriptor
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioModInfoPropertyDescriptor")
	FText Label;

	/**
	* Stored property of the property type enumerated by EModioModInfoPropertyType
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioModInfoPropertyDescriptor")
	EModioModInfoPropertyType Property;
};

/**
* Base class definition that stores a list of property descriptors, then it can be used to
* update those properties on the widget to display
**/
UCLASS()
class MODIOUI_API UModioModPropertyInspectorBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void SynchronizeProperties() override;

	virtual void NativeOnSetDataSource() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioModPropertyInspectorBase")
	TArray<FModioModInfoPropertyDescriptor> PropertiesToDisplay;

	bool bRoutedUpdatePropertyDisplay = false;

	UFUNCTION(BlueprintCallable, Category = "ModioModPropertyInspectorBase")
	void UpdatePropertyDisplay();

	virtual void NativeOnUpdatePropertyDisplay();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatePropertyDisplay();
};
