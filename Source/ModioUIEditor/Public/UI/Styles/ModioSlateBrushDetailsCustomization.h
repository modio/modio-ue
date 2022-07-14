#pragma once

#include "Containers/UnrealString.h"
#include "Customizations/SlateBrushCustomization.h"
#include "DetailWidgetRow.h"
#include "Editor/ModioDetailChildrenBuilder.h"
#include "Editor/ModioEditorHelpers.h"
#include "IDetailChildrenBuilder.h"
#include "Internationalization/Text.h"
#include "PropertyHandle.h"
#include "Styling/SlateBrush.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "UObject/Field.h"
#include "UObject/UnrealType.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SWidget.h"

class FModioSlateBrushStructCustomization : public FSlateBrushStructCustomization
{
public:
	FModioSlateBrushStructCustomization() : FSlateBrushStructCustomization(true) {};

	FSlateBrush* BrushBeingEdited = nullptr;

	FText GetBrushTypeText() const
	{
		if (BrushBeingEdited)
		{
			switch (BrushBeingEdited->GetImageType())
			{
				case ESlateBrushImageType::NoImage:
					return FText::FromString("Brush Type: Color");
					break;
				case ESlateBrushImageType::Linear:
					return FText::FromString("Brush Type: Image(Linear)");
					break;
				case ESlateBrushImageType::FullColor:
					return FText::FromString("Brush Type: Image(sRGB)");
					break;
			}
		}
		return FText::FromString("Unable to determine brush type!");
	}
	FReply ChangeBrushType()
	{
		if (BrushBeingEdited)
		{
			SlateBrushPropHandle->NotifyPreChange();
			switch (BrushBeingEdited->GetImageType())
			{
				case ESlateBrushImageType::NoImage:
					BrushBeingEdited->ImageType = ESlateBrushImageType::Linear;
					break;
				case ESlateBrushImageType::Linear:
					BrushBeingEdited->ImageType = ESlateBrushImageType::FullColor;
					break;
				case ESlateBrushImageType::FullColor:
					BrushBeingEdited->ImageType = ESlateBrushImageType::NoImage;
					break;
			}
			SlateBrushPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		}
		return FReply::Handled();
	}
	TSharedPtr<IPropertyHandle> SlateBrushPropHandle;
	TSharedPtr<IPropertyHandle> BrushRefPropHandle;
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		SlateBrushPropHandle = StructPropertyHandle;
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(StructPropertyHandle->GetProperty());
		void* ValuePtr = (void*) BrushBeingEdited;
		StructPropertyHandle->GetValueData(ValuePtr);
		BrushBeingEdited = static_cast<FSlateBrush*>(ValuePtr);
		FSlateBrushStructCustomization::CustomizeHeader(StructPropertyHandle, HeaderRow, StructCustomizationUtils);
		TSharedRef<SWidget> OriginalHeaderValueContent = HeaderRow.ValueContent().Widget;
		// StructPropertyHandle->GetChildHandle(TEXT("ResourceObject"))
		TSharedPtr<IPropertyHandle> ResourceObjectHandle =
			StructPropertyHandle->GetChildHandle(FName("ResourceObject"));
		if (ResourceObjectHandle)
		{
			BrushRefPropHandle = FModioEditorHelpers::FindSerializedProxyHandle<FModioUIMaterialRef>(
				ResourceObjectHandle, "MaterialSerialize");
		}

		// clang-format off
		HeaderRow
			.ValueContent()
			.MinDesiredWidth(260)
			.MaxDesiredWidth(600)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1)
				[
					OriginalHeaderValueContent
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(4,0,4,0)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(this, &FModioSlateBrushStructCustomization::GetBrushTypeText)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FModioSlateBrushStructCustomization::ChangeBrushType)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Change"))
					]
				]
			];
		// clang-format on
	}

	void OnSourceReferenceUpdated()
	{
		checkf(BrushRefPropHandle.IsValid(), TEXT("Property Handle somehow went out of scope!"));
		TArray<void*> MaterialData;
		BrushRefPropHandle->AccessRawData(MaterialData);
		if (MaterialData.Num())
		{
			FModioUIMaterialRef* Material = reinterpret_cast<FModioUIMaterialRef*>(MaterialData[0]);
			// TSharedPtr<FName> MaterialName = MakeShared<FName>(Material->GetMaterialName());
			// UpdateUnderlyingSlateColor(MaterialName, ESelectInfo::Direct);
			TSharedPtr<IPropertyHandle> UnderlyingBrushResourcePropHandle =
				SlateBrushPropHandle->GetChildHandle(FName("ResourceObject"));
			if (UnderlyingBrushResourcePropHandle)
			{
				UMaterialInterface* ActualMaterial = Material->ResolveReference();
				if (ActualMaterial)
				{
					UnderlyingBrushResourcePropHandle->SetValue(ActualMaterial);
				}
				else
				{
					UnderlyingBrushResourcePropHandle->SetValue((UObject*) nullptr);
				}
			}
		}
		// checkf(ActualReference, TEXT("Color reference somehow went out of scope!"));
		// TSharedPtr<FName> ColorName = MakeShared<FName>(ActualReference->ColorName);
		// UpdateUnderlyingSlateColor(ColorName, ESelectInfo::Direct);
	}
	void OnMaterialRefResetToDefault(TSharedPtr<IPropertyHandle> PropertyBeingReset)
	{
		TArray<void*> MaterialData;
		PropertyBeingReset->AccessRawData(MaterialData);
		if (MaterialData.Num())
		{
			FModioUIMaterialRef* Material = reinterpret_cast<FModioUIMaterialRef*>(MaterialData[0]);
			Material->SetMaterialName(NAME_None);
		}

		TSharedPtr<IPropertyHandle> UnderlyingBrushResourcePropHandle =
			SlateBrushPropHandle->GetChildHandle(FName("ResourceObject"));
		if (UnderlyingBrushResourcePropHandle)
		{
			UnderlyingBrushResourcePropHandle->SetValue((UObject*) nullptr);
		}
	}
	bool IsMaterialRefResettable(TSharedPtr<IPropertyHandle> MaterialRefProperty)
	{
		TArray<void*> MaterialData;
		MaterialRefProperty->AccessRawData(MaterialData);
		if (MaterialData.Num())
		{
			FModioUIMaterialRef* Material = reinterpret_cast<FModioUIMaterialRef*>(MaterialData[0]);
			return Material->GetMaterialName() != NAME_None;
		}
		return false;
	}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
								   class IDetailChildrenBuilder& StructBuilder,
								   IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		IDetailPropertyRow* TmpBrushRefRow = nullptr;
		if (StructPropertyHandle->GetProperty()->GetFName() != FName("NamedBrushes"))
		{
			if (BrushRefPropHandle)
			{
				TmpBrushRefRow = &StructBuilder.AddProperty(BrushRefPropHandle.ToSharedRef());
				TmpBrushRefRow->OverrideResetToDefault(FResetToDefaultOverride::Create(
					FIsResetToDefaultVisible::CreateSP(this,
													   &FModioSlateBrushStructCustomization::IsMaterialRefResettable),
					FResetToDefaultHandler::CreateSP(
						this, &FModioSlateBrushStructCustomization::OnMaterialRefResetToDefault)));
				BrushRefPropHandle->SetOnPropertyValueChanged(
					FSimpleDelegate::CreateSP(this, &FModioSlateBrushStructCustomization::OnSourceReferenceUpdated));
			}
		}

		FModioDetailChildrenBuilder_PropertyRowHook BuilderWrapper(StructBuilder);

		FSlateBrushStructCustomization::CustomizeChildren(StructPropertyHandle, BuilderWrapper,
														  StructCustomizationUtils);
		if (TmpBrushRefRow)
		{
			TSharedPtr<SWidget> NameWidget;
			TSharedPtr<SWidget> ValueWidget;
			TmpBrushRefRow->GetDefaultWidgets(NameWidget, ValueWidget, true);
			TmpBrushRefRow->Visibility(EVisibility::Collapsed);

			IDetailPropertyRow* ResourceRow = BuilderWrapper.GetExistingPropertyRow(
				StructPropertyHandle->GetChildHandle(TEXT("ResourceObject"))->GeneratePathToProperty());
			if (ResourceRow)
			{
				ResourceRow->ShowPropertyButtons(false);
				ResourceRow->CustomWidget()
					.NameContent()[StructPropertyHandle->GetChildHandle(TEXT("ResourceObject"))
									   ->CreatePropertyNameWidget()]
					.ValueContent()[ValueWidget.ToSharedRef()];
			}
		}
	}
	// override customizechildren to be able to swap out the material for a materialref if necessary
};
