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
#include "ModioModBrowserParams.h"
#include "Templates/SubclassOf.h"
#include "UI/Input/ModioNavigationConfigFactoryBase.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UI/Interfaces/IModioUIInputHintGlyphProvider.h"
#include "UI/Views/CategoryBrowser/ModioFeaturedCategoryParams.h"
#include "UObject/NoExportTypes.h"

#include "ModioUISettings.generated.h"

/**
* Helper struct to match a virtual key with inputs or actions associated with it
**/
USTRUCT(BlueprintType)
struct MODIOUI_API FModioInputMapping
{
	GENERATED_BODY()

	/**
	* Default Modio Input Mapping constructor without any parameters
	**/
	FModioInputMapping() = default;

	/**
	* Convenience constructor with a default virtual key parameter
	* @param VirtualKey The key to use when creating an instance
	**/
	FModioInputMapping(FKey VirtualKey)
		: VirtualKey(VirtualKey) {}

	/**
	* Stored property of a single FKey
	**/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (EditCondition = "true==false"),
		Category = "ModioInputMapping")
	FKey VirtualKey;

	/// @brief Array of FNames corresponding to project input axes or actions that are mapped to this virtual key
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioInputMapping")
	TArray<FName> MappedProjectInputs;
};

/**
* Class definition for UI Modio settings that keeps references to navigation, authentication, 
* and background image provider
**/
UCLASS(Config = Game, defaultconfig)
class MODIOUI_API UModioUISettings : public UObject, public IModioInputMappingAccessor
{
	GENERATED_BODY()
public:
	/**
	* Default Modio UI settings constructor that uses an object initializer
	* @param Initializer Related class that contains the parameters needed to create an instance
	**/
	UModioUISettings(const FObjectInitializer& Initializer);

	/**
	 * Used to fetch properties that weren't properly set in the constructor function 
	 **/
	virtual void PostInitProperties() override;

	/**
	* Map your project 's input axes and actions to mod.io UI ' virtual keys' here
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, EditFixedSize, Category = "ModioUISettings")
	TArray<FModioInputMapping> ModioToProjectInputMappings;

	/**
	* The default style set to use for all mod.io UI widgets
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<class UModioUIStyleSet> DefaultStyleSet;

	/** 
	* Optional factory class to instantiate a FNavigationConfig to use for handling navigation instead of
	* ModioToProjectInputMappings. Due to limitations with FSlateApplication, this FNavigationConfig will only support
	* a single user
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<UModioNavigationConfigFactoryBase> NavigationConfigOverride;

	/**
	* Stored property of the data provider for authentication actions
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (MustImplement = "ModioUIAuthenticationDataProvider"),
		Category = "ModioUISettings")
	TSubclassOf<UObject> AuthenticationDataProvider;

	/**
	* Stored property of the glyph provider for input actions
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (MustImplement = "ModioUIInputHintGlyphProvider"),
			  Category = "ModioUISettings")
	TSubclassOf<UObject> InputHintGlyphProvider;

	/**
	* Stored soft pointer property to the browser configuration
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<UModioModBrowserParams> BrowserCategoryConfiguration;

	/**
	 * Stored property of input devices for testing
	 **/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	EModioUIInputMode InputDevicesForTesting;

	/**
	* Turn on if target platform does not support MSDF materials or there are some other issues with MSDF materials.
	* This is turned on by default for XboxOne and Playstation 4.
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	bool bOverridePlatformMaterials = false;

	#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		if (PropertyChangedEvent.GetPropertyName() ==
		    GET_MEMBER_NAME_CHECKED(UModioUISettings, NavigationConfigOverride)) {}
	}
	#endif
};
