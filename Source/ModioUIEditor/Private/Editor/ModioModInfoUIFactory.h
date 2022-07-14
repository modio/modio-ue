// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#if WITH_EDITOR

	#include "CoreMinimal.h"
	#include "Factories/Factory.h"
	#include "AssetTypeActions_Base.h"
	#include "Types/ModioErrorCode.h"
	#include "Misc/Optional.h"
	#include "Types/ModioModInfoList.h"
	#include "Containers/Array.h"
	#include "Types/SlateEnums.h"
	#include "ModioModInfoUIFactory.generated.h"

class FModioModInfoUIAssetActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return FText::FromString("Preview ModioModInfoUI");
	}
	virtual FColor GetTypeColor() const override
	{
		return FColor(149, 70, 255);
	}
	virtual UClass* GetSupportedClass() const override;

	virtual uint32 GetCategories() override;
};

/**
 *
 */
UCLASS()
class UModioModInfoUIFactory : public UFactory
{
	GENERATED_BODY()
protected:
	bool bOperationInProgress = true;
	bool bRequiresSDKShutdown = false;
	TArray<TSharedPtr<FModioModInfo>> ModInfos;
	TSharedPtr<SWindow> ImportWindow;
	TSharedPtr<SListView<TSharedPtr<FModioModInfo> > > AvailableModInfoList;
	TSharedPtr<FModioModInfo> SelectedItem;
	bool bConfirmClicked = false;
public:
	UModioModInfoUIFactory(const FObjectInitializer& ObjectInitializer);

	bool CanCreateNew() const override;

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context,
							  FFeedbackContext* Warn) override;

	FText GetDisplayName() const override;

	bool DoesSupportClass(UClass* Class) override;

	bool ConfigureProperties() override;

	void OnModListRetrieved(FModioErrorCode ec, TOptional<FModioModInfoList> ModList);
	FReply OnConfirmClicked();
	EVisibility IsListVisible() const;
	bool IsModInfoSelected() const;
	void OnSelectionChanged(TSharedPtr<FModioModInfo> Item, ESelectInfo::Type);
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FModioModInfo> Item, const TSharedRef<class STableViewBase>& OwnerTable);
};

#endif