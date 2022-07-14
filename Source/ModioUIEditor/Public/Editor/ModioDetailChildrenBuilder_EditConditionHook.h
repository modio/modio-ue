#pragma once
#include "Containers/Map.h"
#include "Editor/ModioDetailChildrenBuilderBase.h"
#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "Internationalization/Text.h"
#include "PropertyHandle.h"
#include "UObject/NameTypes.h"
#include "UObject/NoExportTypes.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FGetEditConditionValue, TSharedPtr<IPropertyHandle>);
DECLARE_DELEGATE_TwoParams(FUpdateEditConditionValue, bool, TSharedPtr<IPropertyHandle>);

class FModioDetailChildrenBuilder_EditConditionHook : public FModioDetailChildrenBuilderBase<FModioDetailChildrenBuilder_EditConditionHook>
{
	FGetEditConditionValue GetEditConditionValue;
	FUpdateEditConditionValue UpdateEditConditionDelegate;

protected:
	virtual bool IsEditConditionEnabled(TSharedRef<IPropertyHandle> PropertyHandle) const
	{
		if (GetEditConditionValue.IsBound())
		{
			return GetEditConditionValue.Execute(PropertyHandle);
		}
		return false;
	}
	virtual void UpdateEditConditionValue(bool NewValue, TSharedRef<IPropertyHandle> PropertyHandle)
	{
		if (UpdateEditConditionDelegate.IsBound())
		{
			UpdateEditConditionDelegate.Execute(NewValue, PropertyHandle);
		}
	}

public:
	FModioDetailChildrenBuilder_EditConditionHook(FGetEditConditionValue EditConditionCheckDelegate,
												  FUpdateEditConditionValue UpdateEditConditionFunc,
												  IDetailChildrenBuilder& ParentBuilder)
		: FModioDetailChildrenBuilderBase(ParentBuilder),
		  GetEditConditionValue(EditConditionCheckDelegate),
		  UpdateEditConditionDelegate(UpdateEditConditionFunc) {};

	virtual class IDetailPropertyRow& AddProperty(TSharedRef<IPropertyHandle> PropertyHandle) override
	{
		IDetailPropertyRow& PropertyRow = ParentBuilder.AddProperty(PropertyHandle);
		TAttribute<bool> IsParamEnabled = TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(
			AsShared(), &FModioDetailChildrenBuilder_EditConditionHook::IsEditConditionEnabled, PropertyHandle));
		PropertyRow.EditCondition(
			IsParamEnabled,
			FOnBooleanValueChanged::CreateSP(
				AsShared(), &FModioDetailChildrenBuilder_EditConditionHook::UpdateEditConditionValue, PropertyHandle));
		return PropertyRow;
	}
};