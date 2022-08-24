#pragma once
#include "Containers/Map.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "Internationalization/Text.h"
#include "Misc/EngineVersionComparison.h"
#include "PropertyHandle.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"
#include "UObject/NoExportTypes.h"

/// @brief base class for children builders that hook or otherwise modify the behaviour of an existing builder
template<typename Derived>
class FModioDetailChildrenBuilderBase : public IDetailChildrenBuilder, public TSharedFromThis<Derived>
{
protected:
	IDetailChildrenBuilder& ParentBuilder;

public:
	FModioDetailChildrenBuilderBase(IDetailChildrenBuilder& ParentBuilder) : ParentBuilder(ParentBuilder) {};

	virtual IDetailChildrenBuilder& AddCustomBuilder(TSharedRef<IDetailCustomNodeBuilder> InCustomBuilder) override
	{
		return ParentBuilder.AddCustomBuilder(InCustomBuilder);
	}

	virtual class IDetailGroup& AddGroup(FName GroupName, const FText& LocalizedDisplayName) override
	{
		return ParentBuilder.AddGroup(GroupName, LocalizedDisplayName);
	}

	virtual class FDetailWidgetRow& AddCustomRow(const FText& SearchString) override
	{
		return ParentBuilder.AddCustomRow(SearchString);
	}

	virtual class IDetailPropertyRow& AddProperty(TSharedRef<IPropertyHandle> PropertyHandle) override
	{
		return ParentBuilder.AddProperty(PropertyHandle);
	}

	virtual class IDetailPropertyRow* AddExternalObjects(const TArray<UObject*>& Objects,
														 FName UniqueIdName = NAME_None) override
	{
		return ParentBuilder.AddExternalObjects(Objects, UniqueIdName);
	}

	virtual IDetailPropertyRow* AddExternalObjectProperty(
		const TArray<UObject*>& Objects, FName PropertyName,
		const FAddPropertyParams& Params = FAddPropertyParams()) override
	{
		return ParentBuilder.AddExternalObjectProperty(Objects, PropertyName, Params);
	}

	virtual IDetailPropertyRow* AddExternalStructureProperty(
		TSharedRef<FStructOnScope> ChildStructure, FName PropertyName,
		const FAddPropertyParams& Params = FAddPropertyParams()) override
	{
		return ParentBuilder.AddExternalStructureProperty(ChildStructure, PropertyName, Params);
	}

	virtual class IDetailPropertyRow* AddExternalStructure(TSharedRef<FStructOnScope> ChildStructure,
														   FName UniqueIdName = NAME_None) override
	{
		return ParentBuilder.AddExternalStructure(ChildStructure, UniqueIdName);
	}

	virtual TArray<TSharedPtr<IPropertyHandle>> AddAllExternalStructureProperties(
		TSharedRef<FStructOnScope> ChildStructure) override
	{
		return ParentBuilder.AddAllExternalStructureProperties(ChildStructure);
	}

	virtual TSharedRef<SWidget> GenerateStructValueWidget(TSharedRef<IPropertyHandle> StructPropertyHandle) override
	{
		return ParentBuilder.GenerateStructValueWidget(StructPropertyHandle);
	}

	virtual IDetailCategoryBuilder& GetParentCategory() const override
	{
		return ParentBuilder.GetParentCategory();
	}

	virtual IDetailGroup* GetParentGroup() const override
	{
		return ParentBuilder.GetParentGroup();
	}
};