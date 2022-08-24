#include "UI/Styles/ModioUIStyleSet.h"
#include "Core/Input/ModioInputKeys.h"
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
	USlateWidgetStyleContainerBase* const* FoundStyle = WidgetStyles.Find(StyleName);
	if (FoundStyle)
	{
		return (*FoundStyle)->GetStyle();
	}
	else
	{
		return nullptr;
	}
}

void UModioUIStyleSet::Log(EStyleMessageSeverity Severity, const FText& Message) const {}

void UModioUIStyleSet::PostLoad()
{
	Super::PostLoad();
	for (const auto& Prop : PropertyPathToColorPresetMap)
	{
		if (OldColorPresets.Contains(Prop.Value))
		{
			FCachedPropertyPath PropertyPath = FCachedPropertyPath(Prop.Key);
			if (PropertyPath.Resolve(this))
			{
				FStructProperty* ResolvedProperty = CastFieldChecked<FStructProperty>(PropertyPath.GetFProperty());
				if (!ResolvedProperty)
				{
					return;
				}

				PropertyPathHelpers::SetPropertyValue(this, PropertyPath, OldColorPresets[Prop.Value]);
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

void UModioUIStyleSet::ResetInputMappingGlyphs()
{
	InputMappingGlyphs.Empty();
	for (const FKey& InputKey : FModioInputKeys::GetAll())
	{
		InputMappingGlyphs.Add(InputKey, FModioInputMappingGlyph {});
	}
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
	ResetInputMappingGlyphs();
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
	if (ColorPresets.Contains(ColorName))
	{
		return ColorPresets[ColorName];
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
	FName ActualName = PropertyName;
	if (Specifier.IsSet())
	{
		ActualName = FName(PropertyName.ToString() + Specifier.GetValue());
	}

	if (MaterialInstanceCache.Contains(ActualName))
	{
		return MaterialInstanceCache[ActualName];
	}
	else if (NamedBrushMaterialsNew.Contains(PropertyName))
	{
		if (UMaterialInterface* NamedMaterial = NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance())
		{
			if (UMaterialInstanceDynamic* AsDynamicMaterial = Cast<UMaterialInstanceDynamic>(NamedMaterial))
			{
				UMaterialInstanceDynamic* NewInstance =
					UMaterialInstanceDynamic::Create(AsDynamicMaterial->Parent, this);
				NewInstance->CopyInterpParameters(AsDynamicMaterial);
				MaterialInstanceCache.Add(ActualName, NewInstance);
				return NewInstance;
			}
			else
			{
				UMaterialInstanceDynamic* NewMaterialInstance = UMaterialInstanceDynamic::Create(NamedMaterial, this);
				MaterialInstanceCache.Add(ActualName, NewMaterialInstance);
				return NewMaterialInstance;
			}
		}
	}
	return nullptr;
}

UMaterialInterface* UModioUIStyleSet::GetGlyphMaterial(const FName PropertyName)
{
	UMaterialInterface* GlyphMaterial = GetNamedMaterial(FName("DefaultGlyphMaterial"), PropertyName.ToString());
	if (GlyphMaterial)
	{
		UMaterialInstanceDynamic* GlyphMaterialInstance = Cast<UMaterialInstanceDynamic>(GlyphMaterial);
		if (GlyphMaterialInstance)
		{
			if (NamedGlyphs.Contains(PropertyName))
			{
				GlyphMaterialInstance->SetTextureParameterValue(FName("Texture"), NamedGlyphs[PropertyName]);
			}
		}
	}

	return GlyphMaterial;
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
	if (NamedGlyphs.Contains(PropertyName) && DefaultGlyphMaterial)
	{
		TSharedPtr<FSlateBrush> MaterialBrush = GetMaterialBrushInternal(PropertyName, DefaultGlyphMaterial, Size);
		UMaterialInstanceDynamic* GlyphMaterialInstance =
			Cast<UMaterialInstanceDynamic>(MaterialBrush->GetResourceObject());
		checkf(GlyphMaterialInstance, TEXT("Could not create material instance for glyph %s for some reason!"),
			   *PropertyName.ToString());
		if (GlyphMaterialInstance)
		{
			GlyphMaterialInstance->SetTextureParameterValue(FName("Texture"), NamedGlyphs[PropertyName]);
			return MaterialBrush;
		}
	}
	return nullptr;
}

const TSharedPtr<FSlateBrush> UModioUIStyleSet::GetMaterialBrush(const FName PropertyName, TOptional<FString> Specifier,
																 const FVector2D& Size)
{
	if (NamedBrushMaterialsNew.Contains(PropertyName))
	{
		if (Specifier.IsSet())
		{
			if (UMaterialInterface* NamedMaterial = NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance())
			{
				return GetMaterialBrushInternal(FName(PropertyName.ToString() + Specifier.GetValue()), NamedMaterial,
												Size);
			}
		}
		else
		{
			if (UMaterialInterface* NamedMaterial = NamedBrushMaterialsNew[PropertyName]->GetMaterialInstance())
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
		UMaterialInstanceDynamic* NewMaterialInstance = UMaterialInstanceDynamic::Create(ParentMaterial, this);
		MaterialInstanceCache.Add(PropertyName, NewMaterialInstance);
		TSharedPtr<FModioSlateMaterialBrush> NewBrush = MakeShared<FModioSlateMaterialBrush>(NewMaterialInstance, Size);
		MaterialBrushCache.Add(PropertyName, NewBrush);

		return StaticCastSharedPtr<FSlateBrush>(NewBrush);
	}
}

const FSlateBrush* UModioUIStyleSet::GetSlateColorBrush(const FName PropertyName,
														const ANSICHAR* Specifier /*= nullptr*/,
														const FSlateBrush* const DefaultBrush)
{
	const FName ColorName = Join(PropertyName, Specifier);
	if (ColorPresets.Contains(ColorName))
	{
		return ColorPresets[ColorName].GetBrush();
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
	WidgetStyles.GetKeys(StyleNames);
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
	NamedBrushMaterialsNew.GetKeys(MaterialNames);
	return MaterialNames;
}

TArray<FName> UModioUIStyleSet::GetGlyphNames()
{
	TArray<FName> GlyphNames;
	NamedGlyphs.GetKeys(GlyphNames);
	return GlyphNames;
}

UMaterialInterface* UModioUIStyleSet::GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType)
{
	UMaterialInterface* GlyphMaterial = GetNamedMaterial(
		FName("DefaultGlyphMaterial"),
		FString::Printf(TEXT("%s%d"), *VirtualInput.GetFName().ToString(), static_cast<uint8>(InputType)));
	if (GlyphMaterial)
	{
		UMaterialInstanceDynamic* GlyphMaterialInstance = Cast<UMaterialInstanceDynamic>(GlyphMaterial);
		if (GlyphMaterialInstance)
		{
			if (UTexture2D* InputGlyphTexture = GetInputGlyphForInputType(VirtualInput, InputType))
			{
				GlyphMaterialInstance->SetTextureParameterValue(FName("Texture"), InputGlyphTexture);
			}
			return GlyphMaterialInstance;
		}
	}

	return nullptr;
}

UTexture2D* UModioUIStyleSet::GetInputGlyphForInputType(FKey VirtualInput, EModioUIInputMode InputType)
{
	if (InputMappingGlyphs.Contains(VirtualInput))
	{
		return InputMappingGlyphs[VirtualInput].GetInputGlyph(InputType);
	}
	else
	{
		return nullptr;
	}
}
