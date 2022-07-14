// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/ModioUISettings.h"
#include "Core/Auth/ModioUIDefaultAuthProvider.h"
#include "Core/Input/ModioInputKeys.h"
#include "ModioModBrowserParams.h"
#include "UI/Input/ModioNavigationConfigFactoryBase.h"
#include "UI/ModioMenuDefaultBackgroundProvider.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/SoftObjectPtr.h"

UModioUISettings::UModioUISettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	for (const FKey& InputKey : FModioInputKeys::GetAll())
	{
		ModioToProjectInputMappings.Add(FModioInputMapping {InputKey});
	}
	AuthenticationDataProvider = UModioUIDefaultAuthProvider::StaticClass();
	BackgroundImageProvider = UModioMenuDefaultBackgroundProvider::StaticClass();
	DefaultStyleSet =
		TSoftObjectPtr<UModioUIStyleSet>(FSoftObjectPath("/Modio/UI/Styles/ModioUIDefaultStyle.ModioUIDefaultStyle"));
	NavigationConfigOverride = TSoftObjectPtr<UModioNavigationConfigFactoryBase>(
		FSoftObjectPath("/Modio/UI/DefaultNavigationConfigFactory.DefaultNavigationConfigFactory"));
	BrowserCategoryConfiguration = TSoftObjectPtr<UModioModBrowserParams>(
		FSoftObjectPath("/Modio/UI/Browser/DefaultModBrowserParams.DefaultModBrowserParams"));
}
