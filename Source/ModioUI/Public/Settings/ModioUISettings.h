// Fill out your copyright notice in the Description page of Project Settings.

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

USTRUCT(BlueprintType)
struct MODIOUI_API FModioInputMapping
{
	GENERATED_BODY()

	FModioInputMapping() = default;
	FModioInputMapping(FKey VirtualKey) : VirtualKey(VirtualKey) {}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (EditCondition = "true==false"),
			  Category = "ModioInputMapping")
	FKey VirtualKey;

	/// @brief Array of FNames corresponding to project input axes or actions that are mapped to this virtual key
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ModioInputMapping")
	TArray<FName> MappedProjectInputs;
};

/**
 *
 */
UCLASS(Config = Modio, defaultconfig)
class MODIOUI_API UModioUISettings : public UObject, public IModioInputMappingAccessor
{
	GENERATED_BODY()
public:
	UModioUISettings(const FObjectInitializer& Initializer);

	/// @brief Map your project's input axes and actions to mod.io UI 'virtual keys' here
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, EditFixedSize, Category = "ModioUISettings")
	TArray<FModioInputMapping> ModioToProjectInputMappings;

	/// @brief The default style set to use for all mod.io UI widgets
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<class UModioUIStyleSet> DefaultStyleSet;

	/// @brief Optional factory class to instantiate a FNavigationConfig to use for handling navigation instead of
	/// ModioToProjectInputMappings. Due to limitations with FSlateApplication, this FNavigationConfig will only support
	/// a single user
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<UModioNavigationConfigFactoryBase> NavigationConfigOverride;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (MustImplement = "ModioUIAuthenticationDataProvider"),
			  Category = "ModioUISettings")
	TSubclassOf<UObject> AuthenticationDataProvider;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (MustImplement = "ModioUIInputHintGlyphProvider"),
			  Category = "ModioUISettings")
	TSubclassOf<UObject> InputHintGlyphProvider;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, meta = (MustImplement = "ModioMenuBackgroundProvider"),
			  Category = "ModioUISettings")
	TSubclassOf<UObject> BackgroundImageProvider;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "ModioUISettings")
	TSoftObjectPtr<UModioModBrowserParams> BrowserCategoryConfiguration;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		if (PropertyChangedEvent.GetPropertyName() ==
			GET_MEMBER_NAME_CHECKED(UModioUISettings, NavigationConfigOverride))
		{}
	}
#endif
};
