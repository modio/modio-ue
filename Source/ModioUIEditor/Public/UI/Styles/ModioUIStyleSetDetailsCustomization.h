#pragma once

#include "Algo/Transform.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Core/ModioUIHelpers.h"
#include "InputCoreTypes.h"
#include "PropertyEditor/Public/DetailLayoutBuilder.h"
#include "PropertyEditor/Public/DetailWidgetRow.h"
#include "PropertyEditor/Public/IDetailChildrenBuilder.h"
#include "PropertyEditor/Public/IDetailCustomization.h"
#include "PropertyEditor/Public/PropertyHandle.h"
#include "PropertyPath.h"
#include "PropertyPathHelpers.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Input/ModioKeyDetailsCustomization.h"
#include "UI/Styles/IModioUIStyleRefSerializer.h"
#include "UI/Styles/ModioUIColorDetailsCustomization.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/NoExportTypes.h"
#include "UObject/UnrealNames.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

class FModioWidgetStyleDetailsCustomization : public IPropertyTypeCustomization
{
public:
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];
		HeaderRow.ValueContent()[PropertyHandle->CreatePropertyValueWidget(false)];
	}

	/**
	 * Called when the children of the property should be customized or extra rows added
	 *
	 * @param PropertyHandle			Handle to the property being customized
	 * @param StructBuilder				A builder for adding children
	 * @param StructCustomizationUtils	Utilities for customization
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		uint32 NumChildProperties = 0;
		PropertyHandle->GetNumChildren(NumChildProperties);

		if (NumChildProperties)
		{
			TSharedPtr<IPropertyHandle> ActualHandle = PropertyHandle->GetChildHandle(0);
			if (ActualHandle)
			{
				ActualHandle->GetNumChildren(NumChildProperties);

				for (uint32 CurrentChildPropIndex = 0; CurrentChildPropIndex < NumChildProperties;
					 CurrentChildPropIndex++)
				{
					ChildBuilder.AddProperty(ActualHandle->GetChildHandle(CurrentChildPropIndex).ToSharedRef());

					// check if the property has metadata here and do the custom reset to defaults
				}
			}
		}
		/* USlateWidgetStyleContainerBase* ActualContainer = static_cast<USlateWidgetStyleContainerBase*>(DataPointer);
		const FSlateWidgetStyle* Style = ActualContainer->GetStyle();
		PropertyHandle->CreatePropertyValueWidget()
		ChildBuilder.AddExternalStructure(MakeShared<FStructOnScope>(Style->GetStruct(), Style));*/
	};
};

class FModioSlateColorDetailsCustomization : public IPropertyTypeCustomization
{
	UModioUIStyleSet* StyleBeingEdited;
	TArray<TSharedPtr<FName>> ColorPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingSlateColorPropHandle;

public:
	void UpdateColorPresetList()
	{
		ColorPresetNames.Empty();

		Algo::Transform(StyleBeingEdited->OldColorPresets, ColorPresetNames,
						[](const TPair<FName, FLinearColor>& Preset) { return MakeShared<FName>(Preset.Key); });
	}
	void UpdateUnderlyingSlateColorOld(TSharedPtr<FName> ColorName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSlateColorPropHandle->GetProperty());

		if (ColorName.IsValid() && UnderlyingStruct && StyleBeingEdited->OldColorPresets.Contains(*ColorName.Get()))
		{
			UnderlyingSlateColorPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingSlateColorPropHandle->GetValueData(DataPointer);
			FSlateColor* ActualColor = static_cast<FSlateColor*>(DataPointer);
			*ActualColor = FSlateColor(StyleBeingEdited->OldColorPresets[*ColorName.Get()]);
			UnderlyingSlateColorPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		}
	}
	void UpdateUnderlyingSlateColor(TSharedPtr<FName> ColorName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSlateColorPropHandle->GetProperty());

		if (ColorName.IsValid() && UnderlyingStruct && StyleBeingEdited->ColorPresets.Contains(*ColorName.Get()))
		{
			UnderlyingSlateColorPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingSlateColorPropHandle->GetValueData(DataPointer);
			FSlateColor* ActualColor = static_cast<FSlateColor*>(DataPointer);
			*ActualColor = FSlateColor(StyleBeingEdited->ColorPresets[*ColorName.Get()]);
			UnderlyingSlateColorPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		}
	}

	void OnSourceReferenceUpdated()
	{
		checkf(ActualColorHandle.IsValid(), TEXT("Color Handle somehow went out of scope!"));
		TArray<void*> ColorData;
		ActualColorHandle->AccessRawData(ColorData);
		if (ColorData.Num())
		{
			FModioUIColorRef* Color = reinterpret_cast<FModioUIColorRef*>(ColorData[0]);
			TSharedPtr<FName> ColorName = MakeShared<FName>(Color->ColorName);
			UpdateUnderlyingSlateColor(ColorName, ESelectInfo::Direct);
		}
		// checkf(ActualReference, TEXT("Color reference somehow went out of scope!"));
		// TSharedPtr<FName> ColorName = MakeShared<FName>(ActualReference->ColorName);
		// UpdateUnderlyingSlateColor(ColorName, ESelectInfo::Direct);
	}
	FLinearColor GetUnderlyingColor() const
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSlateColorPropHandle->GetProperty());

		if (UnderlyingStruct)
		{
			void* DataPointer;
			UnderlyingSlateColorPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FSlateColor* ActualColor = static_cast<FSlateColor*>(DataPointer);
				return ActualColor->GetSpecifiedColor();
			}
		}
		{
			return FLinearColor(0.25, 0.25, 0.25, 1);
		}
	}

	FModioUIColorRef* ActualReference = nullptr;
	TSharedPtr<IPropertyHandle> ActualColorHandle;
	TSharedPtr<FStructOnScope> ColorRefStruct;
	FString PropertyPath;
	FModioSlateColorDetailsCustomization(UModioUIStyleSet* StyleBeingEdited) : StyleBeingEdited(StyleBeingEdited) {};
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingSlateColorPropHandle = PropertyHandle;
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			// Generate the path to the real FSlateColor being customized - this path will be used as the map key later,
			// so we can easily reapply the resolved color to the FSlateColor during PostLoad
			PropertyPath = PropertyHandle->GeneratePathToProperty();

			TArray<UObject*> OuterObjects;
			PropertyHandle->GetOuterObjects(OuterObjects);

			// Loop through each parent handle of the FSlateColor's property handle
			TSharedPtr<IPropertyHandle> OuterPropHandle = PropertyHandle->GetParentHandle();

			while (OuterPropHandle)
			{
				// UE_LOG(LogTemp, Log, TEXT("%s"), *OuterPropHandle->GeneratePathToProperty());
				// Sanity check, if we somehow get a property handle with a null property??
				if (!OuterPropHandle->GetProperty())
				{
					break;
				}
				// Check if this handle's associated property is a UClass
				UClass* Test = OuterPropHandle->GetProperty()->GetOwnerClass();
				// If it was a UClass and has the correct metadata in its hierarchy
				if (Test && Test->HasMetaDataHierarchical("ColorSerialize"))
				{
					// Resolve the name of the map we're going to serialize color data in
					FString ColorSerializeFieldName;
					Test->GetStringMetaDataHierarchical("ColorSerialize", &ColorSerializeFieldName);
					// The parent of the current property handle should be the property handle representing the class
					// object proper
					OuterPropHandle = OuterPropHandle->GetParentHandle();
					if (OuterPropHandle)
					{
						// Look on the class object for the property with the name stored in metadata
						TSharedPtr<IPropertyHandle> SerializationFieldPropHandle =
							OuterPropHandle->GetChildHandle(FName(ColorSerializeFieldName));

						if (SerializationFieldPropHandle)
						{
							// UE_LOG(LogTemp, Log, TEXT("Serialized In %s"),
							//	   *SerializationFieldPropHandle->GeneratePathToProperty());
							FMapProperty* MapProp =
								CastField<FMapProperty>(SerializationFieldPropHandle->GetProperty());
							if (!MapProp)
							{
								break; // We could try to keep recursing up the tree, but safer for now to just fall
									   // back
							}
							TArray<const void*> PropertyRawData;
							SerializationFieldPropHandle->AccessRawData(PropertyRawData);
							if (PropertyRawData.Num())
							{
								// Search through the map to see if the value already exists
								int32 ValueIndex = INDEX_NONE;
								FScriptMapHelper Helper = FScriptMapHelper(MapProp, PropertyRawData[0]);
								for (int32 ChildIndex = 0; ChildIndex < Helper.Num(); ChildIndex++)
								{
									if (uint8* KeyPtr = Helper.GetKeyPtr(ChildIndex))
									{
										FName* KeyNamePtr = reinterpret_cast<FName*>(KeyPtr);
										if (*KeyNamePtr == FName(PropertyPath))
										{
											ValueIndex = ChildIndex;
										}
									}
								}
								if (ValueIndex == INDEX_NONE)
								{
									// The valye didn't exist, so we're going to add a new one to the map
									FName NewKey(PropertyPath);
									FModioUIColorRef NewValue {};
									TSharedPtr<IPropertyHandleMap> MapHandle = SerializationFieldPropHandle->AsMap();
									if (MapHandle)
									{
										MapHandle->AddItem();
										// We can't guarantee that the element will be added at the end of the map, so
										// iterate through it to find the handle of either a) the existing value or b)
										// the one we just added
										uint32 NumElements = 0;
										MapHandle->GetNumElements(NumElements);
										for (uint32 ChildIndex = 0; ChildIndex < NumElements; ChildIndex++)
										{
											TSharedPtr<IPropertyHandle> ElementHandle =
												SerializationFieldPropHandle->GetChildHandle(ChildIndex);
											if (ElementHandle.IsValid())
											{
												TSharedPtr<IPropertyHandle> ElementKeyHandle =
													ElementHandle->GetKeyHandle();
												if (ElementKeyHandle.IsValid())
												{
													FName KeyValue;
													if (ElementKeyHandle->GetValue(KeyValue) ==
															FPropertyAccess::Success &&
														KeyValue == NAME_None)
													{
														ElementKeyHandle->SetValue(NewKey);
														TArray<void*> ElementRawData;
														ElementHandle->AccessRawData(ElementRawData);
														if (ElementRawData.Num())
														{
															FModioUIColorRef* ValuePtr =
																reinterpret_cast<FModioUIColorRef*>(ElementRawData[0]);
															*ValuePtr = NewValue;
														}

														ValueIndex = ChildIndex;
														MapHandle->GetNumElements(NumElements);
														break;
													}
												}
											}
										}
									}
								}
								// This should always be correct, because we either found an existing value, or we added
								// one
								if (ValueIndex != INDEX_NONE)
								{
									uint32 NumChildren = 0;
									SerializationFieldPropHandle->GetNumChildren(NumChildren);
									// Get the handle of the specific entry in the map
									ActualColorHandle = SerializationFieldPropHandle->GetChildHandle(ValueIndex);
									if (ActualColorHandle)
									{
										// We found the handle we want to add as a child, so we can bail for now and add
										// it in CustomizeChildren
										return;
									}
								}
							}
						}
					}
				}
				OuterPropHandle = OuterPropHandle->GetParentHandle();
			}
			// Fallback
			void* DataPointer;
			PropertyHandle->GetValueData(DataPointer);
			FSlateColor* ActualColor = static_cast<FSlateColor*>(DataPointer);
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox) 
					+SHorizontalBox::Slot().FillWidth(1.0)
					[
						SNew(SColorBlock)
						.Color(this, &FModioSlateColorDetailsCustomization::GetUnderlyingColor)
					] 
					+SHorizontalBox::Slot().AutoWidth()
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Color Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioSlateColorDetailsCustomization::UpdateColorPresetList))
						.OptionsSource(&ColorPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioSlateColorDetailsCustomization::UpdateUnderlyingSlateColorOld))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateLambda([](TSharedPtr<FName> Name){ return SNew(STextBlock).Text(Name.IsValid()? FText::FromName(*Name) : FText());}))
					]
				];
			// clang-format on
		}
		else
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)];
		}
	}

	/**
	 * Called when the children of the property should be customized or extra rows added
	 *
	 * @param PropertyHandle			Handle to the property being customized
	 * @param StructBuilder				A builder for adding children
	 * @param StructCustomizationUtils	Utilities for customization
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		if (ActualColorHandle)
		{
			IDetailPropertyRow& ChildRow = ChildBuilder.AddProperty(ActualColorHandle.ToSharedRef());
			TSharedPtr<SWidget> ColorNameWidget;
			TSharedPtr<SWidget> ColorValueWidget;
			ChildRow.ShowPropertyButtons(false);
			ChildRow.GetDefaultWidgets(ColorNameWidget, ColorValueWidget, false);

			ChildRow.CustomWidget()
				.NameContent()[SNullWidget::NullWidget]
				.ValueContent()[ColorValueWidget.ToSharedRef()];

			ActualColorHandle->SetOnPropertyValueChanged(
				FSimpleDelegate::CreateSP(this, &FModioSlateColorDetailsCustomization::OnSourceReferenceUpdated));
		}
	};
};

class FModioLinearColorDetailsCustomization : public IPropertyTypeCustomization
{
	UModioUIStyleSet* StyleBeingEdited;
	TArray<TSharedPtr<FName>> ColorPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingSlateColorPropHandle;

public:
	void UpdateColorPresetList()
	{
		ColorPresetNames.Empty();

		Algo::Transform(StyleBeingEdited->OldColorPresets, ColorPresetNames,
						[](const TPair<FName, FLinearColor>& Preset) { return MakeShared<FName>(Preset.Key); });
	}
	void UpdateUnderlyingSlateColor(TSharedPtr<FName> ColorName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSlateColorPropHandle->GetProperty());

		if (ColorName.IsValid() && UnderlyingStruct && StyleBeingEdited->OldColorPresets.Contains(*ColorName.Get()))
		{
			void* DataPointer;
			if (UnderlyingSlateColorPropHandle->GetValueData(DataPointer) == FPropertyAccess::Success)
			{
				FString TestString = UnderlyingSlateColorPropHandle->GeneratePathToProperty();
				FString FullNameString = UnderlyingSlateColorPropHandle->GetProperty()->GetFullName();
				TArray<UObject*> Objs;
				UnderlyingSlateColorPropHandle->GetOuterObjects(Objs);
				if (Objs.Num())
				{
					/*	FCachedPropertyPath cp = FCachedPropertyPath(TestString);
						cp.Resolve(Objs[0]);
					FProperty* RoundTripProperty = cp.GetFProperty();
					*/

					// Non-UInterface function call on interface requires explicit cast and direct call
					IModioUIStyleRefSerializer* Serializer = Cast<IModioUIStyleRefSerializer>(Objs[0]);
					if (Serializer)
					{
						Serializer->NativeSerializeStyleReference(TestString, *ColorName.Get());
					}
				}

				FLinearColor* ActualColor = static_cast<FLinearColor*>(DataPointer);
				*ActualColor = FLinearColor(StyleBeingEdited->OldColorPresets[*ColorName.Get()]);
			}
		}
	}
	FLinearColor GetUnderlyingColor() const
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSlateColorPropHandle->GetProperty());

		if (UnderlyingStruct)
		{
			void* DataPointer;
			UnderlyingSlateColorPropHandle->GetValueData(DataPointer);
			FLinearColor* ActualColor = static_cast<FLinearColor*>(DataPointer);
			return *ActualColor;
		}
		else
		{
			return FLinearColor(0.25, 0.25, 0.25, 1);
		}
	}

	TArray<FLinearColor*> CurrentlyEditedColors;

	virtual FReply OnMouseButtonDownColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent,
											   TSharedRef<IPropertyHandle> PropertyHandle)
	{
		CurrentlyEditedColors.Empty();
		void* DataPointer;
		PropertyHandle->GetValueData(DataPointer);
		FLinearColor* ActualColor = static_cast<FLinearColor*>(DataPointer);
		CurrentlyEditedColors.Add(ActualColor);

		FColorPickerArgs Args;
		Args.bIsModal = true;
		Args.bUseAlpha = true;
		Args.LinearColorArray = &CurrentlyEditedColors;
		OpenColorPicker(Args);
		return FReply::Handled();
	}
	FModioLinearColorDetailsCustomization(UModioUIStyleSet* StyleBeingEdited) : StyleBeingEdited(StyleBeingEdited) {};
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingSlateColorPropHandle = PropertyHandle;
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			void* DataPointer;
			if (PropertyHandle->GetProperty()->GetFName() == FName("OldColorPresets"))
			{
				HeaderRow
					.ValueContent()[SNew(SColorBlock)
										.Color(this, &FModioLinearColorDetailsCustomization::GetUnderlyingColor)
										.ShowBackgroundForAlpha(false)
#if UE_VERSION_NEWER_THAN(5, 0, 0)
										.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
#else
										.IgnoreAlpha(true)
#endif

										.OnMouseButtonDown(
											this, &FModioLinearColorDetailsCustomization::OnMouseButtonDownColorBlock,
											PropertyHandle)
										.Size(FVector2D(35.0f, 12.0f))];
				return;
			}
			PropertyHandle->GetValueData(DataPointer);
			FLinearColor* ActualColor = static_cast<FLinearColor*>(DataPointer);
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox) 
					+SHorizontalBox::Slot().MaxWidth(64).AutoWidth()
					[
						SNew(SColorBlock)
						.Color(this, &FModioLinearColorDetailsCustomization::GetUnderlyingColor)
					] 
					+SHorizontalBox::Slot().MaxWidth(128).AutoWidth()
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Color Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioLinearColorDetailsCustomization::UpdateColorPresetList))
						.OptionsSource(&ColorPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioLinearColorDetailsCustomization::UpdateUnderlyingSlateColor))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateLambda([](TSharedPtr<FName> Name){ return SNew(STextBlock).Text(Name.IsValid()? FText::FromName(*Name) : FText());}))
					]
				];
			// clang-format on
		}
		else
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)];
		}
	}

	/**
	 * Called when the children of the property should be customized or extra rows added
	 *
	 * @param PropertyHandle			Handle to the property being customized
	 * @param StructBuilder				A builder for adding children
	 * @param StructCustomizationUtils	Utilities for customization
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override {

	};
};

class FInstancedModioUIColorCustomization : public IDetailCustomization
{
public:
	TSharedRef<class IPropertyTypeCustomization> GetSlateColorCustomization(UModioUIStyleSet* StyleBeingEdited)
	{
		return MakeShared<FModioSlateColorDetailsCustomization>(StyleBeingEdited);
	}

	TSharedRef<class IPropertyTypeCustomization> GetLinearColorCustomization(UModioUIStyleSet* StyleBeingEdited)
	{
		return MakeShared<FModioLinearColorDetailsCustomization>(StyleBeingEdited);
	}

	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
		// Could we load the default style set?
		if (Settings && !Settings->DefaultStyleSet.IsNull())
		{
			UModioUIStyleSet* DefaultStyle = Settings->DefaultStyleSet.LoadSynchronous();

			TArray<TWeakObjectPtr<UObject>> Objects;
			DetailBuilder.GetObjectsBeingCustomized(Objects);
			if (Objects.Num() > 0)
			{
				UObject* FirstObject = Objects[0].Get();
				if (FirstObject)
				{
					DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
						FSlateColor::StaticStruct()->GetFName(),
						FOnGetPropertyTypeCustomizationInstance::CreateSP(
							this, &FInstancedModioUIColorCustomization::GetSlateColorCustomization, DefaultStyle));
					DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
						NAME_LinearColor,
						FOnGetPropertyTypeCustomizationInstance::CreateSP(
							this, &FInstancedModioUIColorCustomization::GetLinearColorCustomization, DefaultStyle));
				}
			}
		}
		// FSlateColorBrush
		// FSlateColor
	}
};

class FModioUIStyleSetDetailsCustomization : public FInstancedModioUIColorCustomization
{
protected:
	TSharedRef<class IPropertyTypeCustomization> GetWidgetStyleCustomization()
	{
		return MakeShared<FModioWidgetStyleDetailsCustomization>();
	};

public:
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		if (Objects.Num() > 0)
		{
			UObject* FirstObject = Objects[0].Get();
			if (FirstObject)
			{
				DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					FSlateColor::StaticStruct()->GetFName(),
					FOnGetPropertyTypeCustomizationInstance::CreateSP(
						this, &FInstancedModioUIColorCustomization::GetSlateColorCustomization,
						Cast<UModioUIStyleSet>(FirstObject)));
				DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					NAME_LinearColor, FOnGetPropertyTypeCustomizationInstance::CreateSP(
										  this, &FInstancedModioUIColorCustomization::GetLinearColorCustomization,
										  Cast<UModioUIStyleSet>(FirstObject)));
				DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					FModioUIColor::StaticStruct()->GetFName(),
					FOnGetPropertyTypeCustomizationInstance::CreateLambda(
						[StyleSet = Cast<UModioUIStyleSet>(FirstObject)]() {
							return MakeShared<FModioUIColorDetailsCustomization>();
						}));
				DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					FKey::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(
														  &FModioEditorHelpers::MakePropertyCustomizationInstance<
															  FModioKeyReadOnlyDetailsCustomization>));

				/*DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					USlateWidgetStyleContainerBase::StaticClass()->GetFName(),
					FOnGetPropertyTypeCustomizationInstance::CreateSP(
						this, &FModioUIStyleSetDetailsCustomization::GetWidgetStyleCustomization));*/
			}
		}

		// instanced details customization for FSLateColors inside FSlateBrush, FSlateImageBrush, FslateColorBrush, etc
		// = looks to outer object (ie the FSlateBrush) then uses that for metadata to resolve on the parent object of
		// the FSlateBrush, to find a special object to load colors from
		// instanced details customization for fslateColors inside ModioUIStyleSet = use the metadata to locate the
		// property path of an FModioUIColorRef to populate from, during PostLoad().
	}
};

class FModioUserWidgetDetailsCustomization : public FInstancedModioUIColorCustomization
{
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FInstancedModioUIColorCustomization::CustomizeDetails(DetailBuilder);
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		if (Objects.Num() > 0)
		{
			UObject* FirstObject = Objects[0].Get();
			if (FirstObject)
			{
				DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(
					FKey::StaticStruct()->GetFName(),
					FOnGetPropertyTypeCustomizationInstance::CreateStatic(
						&FModioEditorHelpers::MakePropertyCustomizationInstance<FModioKeyEditDetailsCustomization>));
			}
		}
	};
};
