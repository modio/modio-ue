/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Settings/ModioUISettings.h"
#include "Core/Auth/ModioUIDefaultAuthProvider.h"
#include "Core/Input/ModioInputKeys.h"
#include "ModioModBrowserParams.h"
#include "UI/Input/ModioNavigationConfigFactoryBase.h"
#include "UI/ModioMenuDefaultBackgroundProvider.h"
#include "UI/ModioDefaultInputGlyphProvider.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/SoftObjectPtr.h"

UModioUISettings::UModioUISettings(const FObjectInitializer& Initializer) : Super(Initializer)
{
	for (const FKey& InputKey : FModioInputKeys::GetAll())
	{
		ModioToProjectInputMappings.Add(FModioInputMapping {InputKey});
	}

	InputHintGlyphProvider = UModioDefaultInputGlyphProvider::StaticClass();
	AuthenticationDataProvider = UModioUIDefaultAuthProvider::StaticClass();
	DefaultStyleSet = Cast<UModioUIStyleSet>(FSoftObjectPath("/Modio/UI/Styles/ModioUIDefaultStyle.ModioUIDefaultStyle").TryLoad());
	NavigationConfigOverride = TSoftObjectPtr<UModioNavigationConfigFactoryBase>(FSoftObjectPath("/Modio/UI/DefaultNavigationConfigFactory.DefaultNavigationConfigFactory"));
	BrowserCategoryConfiguration = TSoftObjectPtr<UModioModBrowserParams>(FSoftObjectPath("/Modio/UI/Browser/DefaultModBrowserParams.DefaultModBrowserParams"));
}

void UModioUISettings::PostInitProperties()
{
	// These are not always correctly fetched in the constructor, so this is to make sure they are

	Super::PostInitProperties();
	if (!InputHintGlyphProvider)
	{
		InputHintGlyphProvider = UModioDefaultInputGlyphProvider::StaticClass();
	}
	if (!AuthenticationDataProvider)
	{
		AuthenticationDataProvider = UModioUIDefaultAuthProvider::StaticClass();
	}
}
