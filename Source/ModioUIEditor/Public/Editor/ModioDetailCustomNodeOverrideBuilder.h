#pragma once
#include "Delegates/Delegate.h"
#include "DetailWidgetRow.h"
#include "Editor/ModioDetailChildrenBuilder_EditConditionHook.h"
#include "IDetailCustomNodeBuilder.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"

class FModioDetailCustomNodeOverrideBuilder : public IDetailCustomNodeBuilder,
											  public TSharedFromThis<FModioDetailCustomNodeOverrideBuilder>
{
	virtual bool IsEditConditionEnabled(TSharedPtr<IPropertyHandle> PropertyHandle) const
	{
		if (BackingProperty)
		{
			if (BackingProperty->Contains(FName(PropertyHandle->GeneratePathToProperty())))
			{
				return *BackingProperty->Find(FName(PropertyHandle->GeneratePathToProperty()));
			}
		}
		return false;
	}
	virtual void UpdateEditConditionValue(bool NewValue, TSharedPtr<IPropertyHandle> PropertyHandle) 
	{
		if (BackingProperty)
		{
			BackingProperty->FindOrAdd(FName(PropertyHandle->GeneratePathToProperty())) = NewValue;
		}
	}

	TSharedPtr<FModioDetailChildrenBuilder_EditConditionHook> BuilderWrapper;
	TSharedPtr<IPropertyHandle> TargetProperty;
	TMap<FName, bool>* BackingProperty = nullptr;
	TArray<TSharedPtr<FModioDetailCustomNodeOverrideBuilder>> ChildPropertyBuilders;
public:
	FModioDetailCustomNodeOverrideBuilder(TSharedRef<IPropertyHandle> PropertyToBuildFor,
										  TMap<FName, bool>* BackingProperty)
		: TargetProperty(PropertyToBuildFor),
		  BackingProperty(BackingProperty) {};

	void SetOnRebuildChildren(FSimpleDelegate InOnRegenerateChildren) override {}

	void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override
	{
		NodeRow.NameContent()[TargetProperty->CreatePropertyNameWidget()];
		NodeRow.ValueContent()[TargetProperty->CreatePropertyValueWidget(false)];
	}

	void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override
	{
		ChildPropertyBuilders.Empty();
		BuilderWrapper = MakeShared<FModioDetailChildrenBuilder_EditConditionHook>(
			FGetEditConditionValue::CreateSP(AsShared(),
											 &FModioDetailCustomNodeOverrideBuilder::IsEditConditionEnabled),
			FUpdateEditConditionValue::CreateSP(AsShared(),
												&FModioDetailCustomNodeOverrideBuilder::UpdateEditConditionValue),
			ChildrenBuilder);
		uint32 NumChildren = 0;
		TargetProperty->GetNumChildren(NumChildren);
		for (uint32 ChildPropertyIndex = 0; ChildPropertyIndex < NumChildren; ChildPropertyIndex++)
		{
			if (TSharedPtr<IPropertyHandle> ChildProp = TargetProperty->GetChildHandle(ChildPropertyIndex))
			{
				BuilderWrapper->AddCustomBuilder(
					MakeShared<FModioDetailCustomNodeOverrideBuilder>(ChildProp.ToSharedRef(), BackingProperty));
			}

		}
	}

	void Tick(float DeltaTime) override {}

	bool RequiresTick() const override
	{
		return false;
	}

	bool InitiallyCollapsed() const override
	{
		return true;
	}

	FName GetName() const override
	{
		static const FName Name = FName("ModioDetailCustomNodeOverrideBuilder");
		return Name;
	}

	TSharedPtr<IPropertyHandle> GetPropertyHandle() const override
	{
		return TargetProperty;
	}
};