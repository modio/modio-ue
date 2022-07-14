
#pragma once

#if WITH_EDITOR

	#include "Framework/MultiBox/MultiBoxExtender.h"
	#include "Framework/MultiBox/MultiBoxBuilder.h"
	#include "Framework/Commands/UIAction.h"
	#include "AssetData.h"
	#include "ContentBrowserModule.h"
	#include "EditorReimportHandler.h"
	#include "Engine/Texture2D.h"

class FModioUIContentBrowserExtensions
{
public:
	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		// Run thru the assets to determine if any meet our criteria
		bool bAnyTextures = false;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& Asset = *AssetIt;
			bAnyTextures = bAnyTextures || (Asset.AssetClass == UTexture2D::StaticClass()->GetFName());
		}

		if (bAnyTextures)
		{
			// Add the sprite actions sub-menu extender
			Extender->AddMenuExtension("GetAssetActions", EExtensionHook::After, nullptr,
									   FMenuExtensionDelegate::CreateStatic(
										   &FModioUIContentBrowserExtensions::CreateMSDFMenuEntry, SelectedAssets));
		}

		return Extender;
	}
	static void CreateMSDFMenuEntry(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		FUIAction Action_ApplyMSDFSettings(
			FExecuteAction::CreateStatic(&FModioUIContentBrowserExtensions::ApplyMSDFSettings, SelectedAssets));

		MenuBuilder.AddMenuEntry(FText::FromString("Apply MSDF Import Settings"),
								 FText::FromString("Sets flags on this texture to make it suitable for use in UI "
												   "materials as a Multi Channel Signed Distance Field"),
								 FSlateIcon(), Action_ApplyMSDFSettings, NAME_None, EUserInterfaceActionType::Button);
	}

	static void ApplyMSDFSettings(TArray<FAssetData> SelectedAssets)
	{
		TArray<UTexture2D*> Textures;
		for (auto AssetIt = SelectedAssets.CreateConstIterator(); AssetIt; ++AssetIt)
		{
			const FAssetData& AssetData = *AssetIt;
			if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
			{
				Textures.Add(Texture);
			}
		}

		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		FContentBrowserModule& ContentBrowserModule =
			FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		for (auto TextureIt = Textures.CreateConstIterator(); TextureIt; ++TextureIt)
		{
			UTexture2D* Texture = *TextureIt;
			Texture->CompressionSettings = TextureCompressionSettings::TC_EditorIcon;
			Texture->SRGB = false;
			Texture->AddressX = TextureAddress::TA_Clamp;
			Texture->AddressY = TextureAddress::TA_Clamp;
			Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
			FReimportManager::Instance()->Reimport(Texture, true);
		}
	}
};

#endif