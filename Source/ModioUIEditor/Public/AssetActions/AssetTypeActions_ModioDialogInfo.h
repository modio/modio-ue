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

class FAssetTypeActions_ModioDialogInfo : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return FText::FromString("Modio Dialog Info");
	}
	virtual FColor GetTypeColor() const override
	{
		return FColor(149, 70, 255);
	}
	virtual UClass* GetSupportedClass() const override;

	virtual uint32 GetCategories() override;

	void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;


};

#endif