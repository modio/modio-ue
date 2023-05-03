/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioUIStyleSet.h"
#include "Core/Input/ModioInputKeys.h"
#include "UI/Styles/ModioSpecifiedMaterialParams.h"
#include "UI/Styles/ModioRoundedRectangleMaterialParams.h"
#include "UI/Styles/ModioCheckBoxMaterialParams.h"
#include "UI/Styles/ModioWidgetStyleData.h"
#include "UI/Styles/ModioNamedGlyphsDataAsset.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "PropertyPathHelpers.h"
#include "SlateMaterialBrush.h"

#if UE_VERSION_NEWER_THAN(5, 0, 0)
const FSlateWidgetStyle* UModioUIStyleSet::GetWidgetStyleInternal(const FName DesiredTypeName, const FName StyleName,
																  const FSlateWidgetStyle* DefaultStyle,
																  bool bWarnIfNotFound) const
#else
const FSlateWidgetStyle* UModioUIStyleSet::GetWidgetStyleInternal(const FName DesiredTypeName,
																  const FName StyleName) const
#endif
{
	USlateWidgetStyleContainerBase* FoundStyle = nullptr;
	for (auto& asset : WidgetStyleAssets)
	{
		FoundStyle = asset->WidgetStyles.FindRef(StyleName);
		if (FoundStyle)
		{
			break;
		}
	}

	if (FoundStyle)
	{
		return FoundStyle->GetStyle();
	}
	return nullptr;
}

void UModioUIStyleSet::Log(EStyleMessageSeverity Severity, const FText& Message) const {}

void UModioUIStyleSet::PostLoad()
{
	Super::PostLoad();

	for (const auto& Prop : PropertyPathToColorPresetMap)
	{
		if (ColorPresetsData && ColorPresetsData->ColorPresets.Contains(Prop.Value))
		{
			FCachedPropertyPath PropertyPath = FCachedPropertyPath(Prop.Key);
			if (PropertyPath.Resolve(this))
			{
				FStructProperty* ResolvedProperty = CastFieldChecked<FStructProperty>(PropertyPath.GetFProperty());
				if (!ResolvedProperty)
				{
					return;
				}

				PropertyPathHelpers::SetPropertyValue(this, PropertyPath, ColorPresetsData->ColorPresets[Prop.Value]);
			}
		}
	}

	for (const TPair<FName, FModioUIColorRef>& SerializedColor : SerializedColors)
	{
		TArray<FString> PropertyPathElems;
		PropertyPathHelpers::SetPropertyValue(this, SerializedColor.Key.ToString(),
											  SerializedColor.Value.ResolveReference(this));
	}
	for (const TPair<FName, FModioUIMaterialRef>& SerializedBrush : SerializedMaterials)
	{
		UMaterialInterface* BrushMaterial = SerializedBrush.Value.ResolveReference(this);
		if (BrushMaterial)
		{
			if (!ModioNewPropertyHelpers::SetPropertyValue<UObject*>(this, SerializedBrush.Key.ToString(),
																	 Cast<UObject>(BrushMaterial)))
			{}
		}
	}
}

void UModioUIStyleSet::ClearMaterialCache()
{
	for (auto& material : MaterialInstanceCache)
	{
		material.Value->RemoveFromRoot();
	}

	MaterialInstanceCache.Empty();
}

TSet<FName> UModioUIStyleSet::GetStyleKeys() const
{
	return TSet<FName>(GetAllStyleNames());
}

FString UModioUIStyleSet::GetContentRootDir() const
{
	return "";
}

void UModioUIStyleSet::LogMissingResource(EStyleMessageSeverity Severity, const FText& Message,
										  const FName& MissingResource) const
{
	if (!MissingResources.Contains(MissingResource))
	{
		FText StyleAndMessage = FText::Format(
			NSLOCTEXT("SlateStyleSet", "SlateSetResourceMissing", "Missing Resource from '{0}' Style: '{1}'"),
			FText::FromName(GetStyleSetName()), Message);

		MissingResources.Add(MissingResource);
		Log(Severity, StyleAndMessage);
	}
}

UModioUIStyleSet::UModioUIStyleSet(const FObjectInitializer& Initializer) : UDataAsset(Initializer)
{

}

TArray<UObject*> UModioUIStyleSet::GetAssetReferences()
{
	TArray<UObject*> objects;
	objects.Add(MaterialData);
	objects.Add(NamedGlyphsAsset);
	objects.Add(ColorPresetsData);
	for (auto& style : WidgetStyleAssets)
	{
		objects.Add(style);
	}
	return objects;
}

void UModioUIStyleSet::RefreshStyleMaterials(FName StyleName) 
{
	for (auto& asset : WidgetStyleAssets)
	{
		UModioUIWidgetStyleContainer* style = Cast<UModioUIWidgetStyleContainer>(asset->WidgetStyles.FindRef(StyleName));
		if (style)
		{
			style->LoadBrushMaterials();
			return;
		}
	}
}

const FName& UModioUIStyleSet::GetStyleSetName() const
{
	const static FName Name = FName("ModioUIStyleSet");
	return Name;
}

void UModioUIStyleSet::GetResources(TArray<const FSlateBrush*>& OutResources) const {}

TArray<FName> UModioUIStyleSet::GetEntriesUsingBrush(const FName BrushName) const
{
	return {};
}

float UModioUIStyleSet::GetFloat(const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
								 float DefaultValue /*= FStyleDefaults::GetFloat()*/ MODIO_UE5_REQUESTING_STYLE) const
{
	return {};
}

FVector2D UModioUIStyleSet::GetVector(
	const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
	FVector2D DefaultValue /*= FStyleDefaults::GetVector2D()*/ MODIO_UE5_REQUESTING_STYLE) const
{
	return {};
}

const FLinearColor& UModioUIStyleSet::GetColor(
	const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
	const FLinearColor& DefaultValue /*= FStyleDefaults::GetColor()*/ MODIO_UE5_REQUESTING_STYLE) const
{
	static FLinearColor DefaultColor = FLinearColor::White;
	return DefaultColor;
}

const FSlateColor UModioUIStyleSet::GetSlateColor(
	const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
	const FSlateColor& DefaultValue /*= FStyleDefaults::GetSlateColor()*/ MODIO_UE5_REQUESTING_STYLE) const
{
	const FName ColorName = Join(PropertyName, Specifier);

	if (ColorPresetsData && ColorPresetsData->ColorPresets.Contains(ColorName))
	{
		return ColorPresetsData->ColorPresets[ColorName];
	}
	return DefaultValue;
}

const FMargin& UModioUIStyleSet::GetMargin(
	const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
	const FMargin& DefaultValue /*= FStyleDefaults::GetMargin()*/ MODIO_UE5_REQUESTING_STYLE) const
{
	static FMargin DefaultMargin;
	return DefaultMargin;
}

UMaterialInterface* UModioUIStyleSet::GetNamedMaterial(const FName PropertyName, TOptional<FString> Specifier)
{
	UMaterialInterface* material = nullptr; 
	if (MaterialData)
	{
		if (MaterialData->NamedBrushMaterialsNew.Contains(PropertyName))
		{
			material = MaterialData->NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance();
		}
		else if (MaterialData->RoundedRectangleParams.Contains(PropertyName))
		{
			material = MaterialData->RoundedRectangleParams[PropertyName]->GetMaterialInstance();
		}
		else if (MaterialData->CheckBoxMaterialParams.Contains(PropertyName))
		{
			material = MaterialData->CheckBoxMaterialParams[PropertyName]->GetMaterialInstance();
		}
	}

	if (material && !MaterialInstanceCache.Contains(PropertyName))
	{
		MaterialInstanceCache.Add(PropertyName, material);
		material->AddToRoot(); // Material instances get garbage collected without this 
	}

	if (MaterialInstanceCache.Contains(PropertyName))
	{
		return MaterialInstanceCache.FindRef(PropertyName);
	}

	return material;
}

UMaterialInterface* UModioUIStyleSet::GetDefaultRoundedRectangleMaterial()
{
	if (MaterialData)
	{
		return MaterialData->DefaultRoundedRectangleMaterial;
	}
	return nullptr;
}

UMaterialInterface* UModioUIStyleSet::GetDefaultCheckboxMaterial()
{
	if (MaterialData)
	{
		return MaterialData->DefaultCheckboxMaterial;
	}
	return nullptr;
}

UMaterialInterface* UModioUIStyleSet::GetGlyphMaterial(const FName PropertyName)
{
	UMaterialInterface* GlyphMaterial = nullptr;
	if (!MaterialData || !MaterialData->GetGlyphMaterial(bOverridePlatformMaterials))
	{
		return nullptr;
	}
	if (NamedGlyphsAsset)
	{
		GlyphMaterial = MaterialData->GetGlyphMaterial(bOverridePlatformMaterials);
	}

	if (GlyphMaterial)
	{
		UMaterialInstanceDynamic* GlyphMaterialInstance;
		GlyphMaterialInstance = UMaterialInstanceDynamic::Create(GlyphMaterial, NULL);

		if (GlyphMaterialInstance && NamedGlyphsAsset)
		{
			if (NamedGlyphsAsset->GetNamedGlyph(PropertyName))
			{
				GlyphMaterialInstance->SetTextureParameterValue(FName("Texture"),
																NamedGlyphsAsset->GetNamedGlyph(PropertyName, bOverridePlatformMaterials));
			}
			return GlyphMaterialInstance;
		}

		return GlyphMaterial;
		
	}
	return nullptr;
}

const FSlateBrush* UModioUIStyleSet::GetBrush(const FName PropertyName,
											  const ANSICHAR* Specifier /*= nullptr*/ MODIO_UE5_REQUESTING_STYLE) const
{
	const FName StyleName = Join(PropertyName, Specifier);
	const FSlateBrush* Result = NamedBrushes.Find(StyleName);

	return Result;
}

const TSharedPtr<FSlateBrush> UModioUIStyleSet::GetGlyphBrush(const FName PropertyName, const FVector2D& Size)
{
	UMaterialInterface* material = nullptr;
	if (!MaterialData)
	{
		return nullptr;
	}
	if (NamedGlyphsAsset)
	{
		material = MaterialData->GetGlyphMaterial(bOverridePlatformMaterials);
		TSharedPtr<FSlateBrush> MaterialBrush = GetMaterialBrushInternal(PropertyName, material, Size);
		UMaterialInstanceDynamic* GlyphMaterialInstance =
			Cast<UMaterialInstanceDynamic>(MaterialBrush->GetResourceObject());
		checkf(GlyphMaterialInstance, TEXT("Could not create material instance for glyph %s for some reason!"),
			   *PropertyName.ToString());
		if (GlyphMaterialInstance && NamedGlyphsAsset->GetNamedGlyph(PropertyName, bOverridePlatformMaterials))
		{
			GlyphMaterialInstance->SetTextureParameterValue(FName("Texture"), NamedGlyphsAsset->GetNamedGlyph(PropertyName, bOverridePlatformMaterials));
			return MaterialBrush;
		}
	}
	return nullptr;
}

const TSharedPtr<FSlateBrush> UModioUIStyleSet::GetMaterialBrush(const FName PropertyName, TOptional<FString> Specifier,
																 const FVector2D& Size)
{
	if (MaterialData && MaterialData->NamedBrushMaterialsNew.Contains(PropertyName))
	{
		if (Specifier.IsSet())
		{
			if (UMaterialInterface* NamedMaterial =
					MaterialData->NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance())
			{
				return GetMaterialBrushInternal(FName(PropertyName.ToString() + Specifier.GetValue()), NamedMaterial,
												Size);
			}
		}
		else
		{
			if (UMaterialInterface* NamedMaterial =
					MaterialData->NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance())
			{
				return GetMaterialBrushInternal(PropertyName, NamedMaterial, Size);
			}
		}
	}
	return nullptr;
}

const TSharedPtr<FSlateBrush> UModioUIStyleSet::GetMaterialBrushInternal(const FName PropertyName,
																		 UMaterialInterface* ParentMaterial,
																		 const FVector2D& Size)
{
	if (MaterialBrushCache.Contains(PropertyName))
	{
		return StaticCastSharedPtr<FSlateBrush>(MaterialBrushCache[PropertyName]);
	}
	else
	{
		UMaterialInstanceDynamic* NewMaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, NULL);
		if (!MaterialInstanceCache.Contains(PropertyName))
		{
			NewMaterialInstance->AddToRoot();
			MaterialInstanceCache.Add(PropertyName, NewMaterialInstance);
		}
		TSharedPtr<FModioSlateMaterialBrush> NewBrush =
			MakeShared<FModioSlateMaterialBrush>(MaterialInstanceCache.FindRef(PropertyName), Size);
		MaterialBrushCache.Add(PropertyName, NewBrush);

		return StaticCastSharedPtr<FSlateBrush>(NewBrush);
	}
}

const FSlateBrush* UModioUIStyleSet::GetSlateColorBrush(const FName PropertyName,
														const ANSICHAR* Specifier /*= nullptr*/,
														const FSlateBrush* const DefaultBrush)
{
	const FName ColorName = Join(PropertyName, Specifier);

	if (ColorPresetsData->ColorPresets.Contains(ColorName))
	{
		return ColorPresetsData->ColorPresets[ColorName].GetBrush();
	}
	return DefaultBrush;
}

const FSlateBrush* UModioUIStyleSet::GetOptionalBrush(
	const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/,
	const FSlateBrush* const DefaultBrush /*= FStyleDefaults::GetNoBrush()*/) const
{
	return nullptr;
}

const TSharedPtr<FSlateDynamicImageBrush> UModioUIStyleSet::GetDynamicImageBrush(
	const FName BrushTemplate, const FName TextureName,
	const ANSICHAR* Specifier /*= nullptr*/ MODIO_UE5_REQUESTING_STYLE) MODIO_UE5_CONST
{
	return nullptr;
}

const TSharedPtr<FSlateDynamicImageBrush> UModioUIStyleSet::GetDynamicImageBrush(
	const FName BrushTemplate, const ANSICHAR* Specifier, UTexture2D* TextureResource,
	const FName TextureName MODIO_UE5_REQUESTING_STYLE) MODIO_UE5_CONST
{
	return nullptr;
}

const TSharedPtr<FSlateDynamicImageBrush> UModioUIStyleSet::GetDynamicImageBrush(
	const FName BrushTemplate, UTexture2D* TextureResource,
	const FName TextureName MODIO_UE5_REQUESTING_STYLE) MODIO_UE5_CONST
{
	return nullptr;
}

const TSharedPtr<FSlateDynamicImageBrush> UModioUIStyleSet::MakeDynamicImageBrush(const FName BrushTemplate,
																				  UTexture2D* TextureResource,
																				  const FName TextureName) const
{
	return nullptr;
}

FSlateBrush* UModioUIStyleSet::GetDefaultBrush() const
{
	return nullptr;
}

const FSlateSound& UModioUIStyleSet::GetSound(const FName PropertyName,
											  const ANSICHAR* Specifier /*= nullptr*/ MODIO_UE5_REQUESTING_STYLE) const
{
	static FSlateSound DefaultSound;
	return DefaultSound;
}

FSlateFontInfo UModioUIStyleSet::GetFontStyle(const FName PropertyName, const ANSICHAR* Specifier /*= nullptr*/) const
{
	return {};
}

void UModioUIStyleSet::NativeSerializeStyleReference(FString PathToProperty, FName StyleElementReference)
{
	PropertyPathToColorPresetMap.Add(PathToProperty, StyleElementReference);
}

TArray<FName> UModioUIStyleSet::GetAllStyleNames() const
{
	TArray<FName> StyleNames;
	for (auto& asset : WidgetStyleAssets)
	{
		for (auto& style : asset->WidgetStyles)
		{
			StyleNames.Add(style.Key);
		}
	}
	return StyleNames;
}

TArray<FName> UModioUIStyleSet::GetNamedBrushNames()
{
	TArray<FName> BrushNames;
	NamedBrushes.GetKeys(BrushNames);
	return BrushNames;
}

TArray<FName> UModioUIStyleSet::GetMaterialNames()
{
	TArray<FName> MaterialNames;

	if (MaterialData)
	{
		MaterialData->NamedBrushMaterialsNew.GetKeys(MaterialNames);
		for (auto& value : MaterialData->RoundedRectangleParams)
		{
			MaterialNames.Add(value.Key);
		}
		for (auto& value : MaterialData->CheckBoxMaterialParams)
		{
			MaterialNames.Add(value.Key);
		}
	}
	return MaterialNames;
}

TArray<FName> UModioUIStyleSet::GetGlyphNames()
{
	TArray<FName> GlyphNames;
	if (!NamedGlyphsAsset)
	{
		return GlyphNames;
	}
	NamedGlyphsAsset->NamedGlyphs.GetKeys(GlyphNames);
	return GlyphNames;
}

void UModioUIStyleSet::ModifyRoundedRectangle(FName InName, UMaterialInstanceDynamic* MaterialInstance) 
{
	if (MaterialData && MaterialData->RoundedRectangleParams.Contains(InName))
	{
		MaterialData->RoundedRectangleParams[InName]->ModifyMaterial(MaterialInstance);
	}
}

UModioColorPresets* UModioUIStyleSet::GetColorPaletteData()
{
	return ColorPresetsData;
}
