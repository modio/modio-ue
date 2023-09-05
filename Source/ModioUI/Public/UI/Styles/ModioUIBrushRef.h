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

#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UObject/NameTypes.h"

#include "ModioUIBrushRef.generated.h"

/**
* Strong type struct to wrap a UI material reference
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioUIMaterialRef
{
	GENERATED_BODY()
protected:
	/**
	* Stored property of the material name
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FName MaterialName;

	friend class FModioSlateBrushStructCustomization;

public:
	/**
	* Default constructor without parameters
	**/
	FModioUIMaterialRef() : MaterialName(NAME_None) {};
	
	/**
	* Convenience constructor with a material name parameter
	* @param MaterialName Name of the material to store in this instance
	**/
	FModioUIMaterialRef(FName MaterialName) : MaterialName(MaterialName) {};

	/**
	* Update the material name stored
	* @param NewName The update to set the stored material name
	**/
	void SetMaterialName(FName NewName)
	{
		MaterialName = NewName;
	}

	/**
	* Retrieve the material name stored
	* @return FName stored in this instance
	**/
	FName GetMaterialName();
	
	/**
	* Query the engine about a UI style and retrieve the material reference
	* @param OverrideStyleSet By default the reference will be resolved against the default style set, this provides a
	* different styleset to reference. Not intended for normal use, currently used only to break a circular dependency
	* when setting a new UModioUIStyleSet.
	* @param AdditionalSpecifier Extra parameter use to find the material in the engine subsystem 
	* @return Material interface when the reference is found, otherwise a nullptr
	**/
	UMaterialInterface* ResolveReference(class UModioUIStyleSet* OverrideStyleSet = nullptr,
										 FName AdditionalSpecifier = NAME_None) const;
};

/*
template<>
struct TStructOpsTypeTraits<FModioUIBrushRef> : public TStructOpsTypeTraitsBase2<FModioUIBrushRef>
{
	enum
	{
		WithSerializer = true,
	};
};
*/