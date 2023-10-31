/*
 *  Copyright (C) 2022 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */


#include "ModioInputGlyphSet.h"

#include "ModioUI4Subsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "ModioUISubsystem.h"
#include "GameFramework/InputSettings.h"
#include "Settings/ModioUISettings.h"

UMaterialInterface* UModioInputGlyphSet::GetGlyphForKey(const FKey& Key)
{
	if (!Key.IsValid())
	{
		return nullptr;
	}

	FKey RawKey;
	UTexture2D* GlyphTexture = nullptr;
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (!Subsystem)
	{
		return nullptr;
	}

	UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
	if (!DefaultStyleSet)
	{
		return nullptr;
	}

	if (UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
	{
		if (UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings())
		{
			FModioInputMapping MappedKey;

			for(int i = 0; i < CurrentUISettings->ModioToProjectInputMappings.Num(); i++)
			{
				if (CurrentUISettings->ModioToProjectInputMappings[i].VirtualKey == Key)
				{
					MappedKey = CurrentUISettings->ModioToProjectInputMappings[i];
					break;
				}
			}
			
			if (!MappedKey.MappedProjectInputs.IsValidIndex(0)) 
			{
				return nullptr;
			}

			TArray<FInputActionKeyMapping> ActionMappings;
			CurrentInputSettings->GetActionMappingByName(MappedKey.MappedProjectInputs[0], ActionMappings);

			for (auto& ActionMapping : ActionMappings)
			{ 
				#if PLATFORM_XBOXONE || PLATFORM_PS4 || PLATFORM_PS5
				if (PngGlyphs.Contains(ActionMapping.Key))
				{
					GlyphTexture = PngGlyphs[ActionMapping.Key];
				}
				#else
				if (DefaultStyleSet->bOverridePlatformMaterials && PngGlyphs.Contains(ActionMapping.Key))
				{
					GlyphTexture = PngGlyphs[ActionMapping.Key];
				}
				else if (Glyphs.Contains(ActionMapping.Key))
				{
					GlyphTexture = Glyphs[ActionMapping.Key];
				}
				#endif
			}
		}
	}

	UMaterialInstanceDynamic* GlyphMaterial = UMaterialInstanceDynamic::Create(
		DefaultStyleSet->MaterialData->GetGlyphMaterial(DefaultStyleSet->bOverridePlatformMaterials), NULL);

	if (GlyphMaterial)
	{
		if (GlyphTexture)
		{
			GlyphMaterial->SetTextureParameterValue(FName("Texture"), GlyphTexture);
		}
		return GlyphMaterial;
	}

	return nullptr;
}

UTexture2D* UModioInputGlyphSet::GetTextureForKey(const FKey& Key)
{
	if (UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
	{
		if (UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings())
		{
			FModioInputMapping MappedKey;

			for (int i = 0; i < CurrentUISettings->ModioToProjectInputMappings.Num(); i++)
			{
				if (CurrentUISettings->ModioToProjectInputMappings[i].VirtualKey == Key)
				{
					MappedKey = CurrentUISettings->ModioToProjectInputMappings[i];
					break;
				}
			}

			if (!MappedKey.MappedProjectInputs.IsValidIndex(0))
			{
				return nullptr;
			}

			TArray<FInputActionKeyMapping> ActionMappings;
			CurrentInputSettings->GetActionMappingByName(MappedKey.MappedProjectInputs[0], ActionMappings);

			for (auto& ActionMapping : ActionMappings)
			{
				if (PngGlyphs.Contains(ActionMapping.Key))
				{
					return PngGlyphs.FindRef(ActionMapping.Key);
				}
			}
		}
	}
	return nullptr;
}

