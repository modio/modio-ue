#pragma once

#include "Core/ModioNewPropertyHelpers.h"
#include "Materials/MaterialInterface.h"
#include "ModioUI.h"
#include "PropertyPathHelpers.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "UI/Styles/ModioUIColorRef.h"

#include "ModioUIWidgetStyleContainer.generated.h"

PRAGMA_DISABLE_OPTIMIZATION

USTRUCT()
struct FModioSlateColorInspector : public FSlateColor
{
	GENERATED_BODY();
	FModioSlateColorInspector() : FSlateColor() {};
	FModioSlateColorInspector(FSlateColor InColor) : FSlateColor(InColor) {};

	bool IsUnlinked()
	{
		return ColorUseRule == ESlateColorStylingMode::UseColor_Specified;
	}
};

UCLASS(BlueprintType, EditInlineNew, collapseCategories, MinimalAPI,
	   meta = (MaterialSerialize = "SerializedMaterials", ColorSerialize = "SerializedColors"))
class UModioUIWidgetStyleContainer : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, meta = (ReadOnlyKeys, EditCondition = "true==false", EditConditionHides,
									ShowOnlyInnerProperties, EditFixedSize, EditFixedOrder))
	TMap<FName, FModioUIColorRef> SerializedColors;

	UPROPERTY(EditAnywhere, meta = (ReadOnlyKeys, EditCondition = "true==false", EditConditionHides,
									ShowOnlyInnerProperties, EditFixedSize, EditFixedOrder))
	TMap<FName, FModioUIMaterialRef> SerializedMaterials;

public:
	virtual void PreSave(const class ITargetPlatform* TargetPlatform) override
	{
		for (FPropertyValueIterator It = FPropertyValueIterator(FStructProperty::StaticClass(), GetClass(), this); It;
			 ++It)
		{
			const FStructProperty* Prop = CastField<FStructProperty>(It->Key);

			const void* Value = It->Value;
			if (Prop->Struct == FSlateColor::StaticStruct())
			{
				FSlateColor* ActualColor = const_cast<FSlateColor*>(reinterpret_cast<const FSlateColor*>(Value));
				if (!FModioSlateColorInspector(*ActualColor).IsUnlinked())
				{
					UE_LOG(LogModioUI, Display, TEXT("Unlinking color %s in %s"), *Prop->GetAuthoredName(),
						   *GetFName().ToString());
					ActualColor->Unlink();
				}
			}
		}

		Super::PreSave(TargetPlatform);
	}
	virtual void PostLoad() override
	{
		Super::PostLoad();

		for (FPropertyValueIterator It = FPropertyValueIterator(FStructProperty::StaticClass(), GetClass(), this); It;
			 ++It)
		{
			const FStructProperty* Prop = CastField<FStructProperty>(It->Key);

			const void* Value = It->Value;
			if (Prop->Struct == FSlateColor::StaticStruct())
			{
				FSlateColor* ActualColor = const_cast<FSlateColor*>(reinterpret_cast<const FSlateColor*>(Value));
				if (!FModioSlateColorInspector(*ActualColor).IsUnlinked())
				{
					UE_LOG(LogModioUI, Display, TEXT("Unlinking color %s in %s"), *Prop->GetAuthoredName(),
						   *GetFName().ToString());
					ActualColor->Unlink();
				}
			}
		}

		SerializedColors.Remove(NAME_None);

		for (const TPair<FName, FModioUIColorRef>& SerializedColor : SerializedColors)
		{
			TArray<FString> PropertyPathElems;
			if (!ModioNewPropertyHelpers::SetPropertyValue(this, SerializedColor.Key.ToString(),
														   SerializedColor.Value.ResolveReference()))
			{
				// UE_LOG(LogModioUI, Warning,
				//	   TEXT("Unable to restore serialized value for %s in %s"), *SerializedColor.Key.ToString(),
				//*GetFName().ToString());
			}
		}
		for (const TPair<FName, FModioUIMaterialRef>& SerializedBrush : SerializedMaterials)
		{
			UMaterialInterface* BrushMaterial = SerializedBrush.Value.ResolveReference();
			if (BrushMaterial)
			{
				if (!ModioNewPropertyHelpers::SetPropertyValue<UObject*>(this, SerializedBrush.Key.ToString(),
																		 Cast<UObject>(BrushMaterial)))
				{
					ModioNewPropertyHelpers::SetPropertyValue<UObject*>(
						this, SerializedBrush.Key.ToString() + ".ResourceObject", Cast<UObject>(BrushMaterial));
				}
			}
		}
	}
};

PRAGMA_ENABLE_OPTIMIZATION