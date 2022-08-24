/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Factories/ModioDialogInfoFactory.h"
#include "AssetRegistryModule.h"
#include "AssetTypeCategories.h"
#include "Core/ModioUIHelpers.h"
#include "UI/Dialogs/ModioDialogInfo.h"

UModioDialogInfoFactory::UModioDialogInfoFactory()
{
	SupportedClass = UModioDialogInfo::StaticClass();
}

FText UModioDialogInfoFactory::GetDisplayName() const
{
	return FText::FromString("Modio Dialog Info");
}

bool UModioDialogInfoFactory::ShouldShowInNewMenu() const
{
	return true;
}
bool UModioDialogInfoFactory::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(UModioDialogInfo::StaticClass());
}
uint32 UModioDialogInfoFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::UI;
}

UObject* UModioDialogInfoFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
												   UObject* Context, FFeedbackContext* Warn)
{
	UModioDialogInfo* NewDialogInfo = NewObject<UModioDialogInfo>(InParent, InClass, InName, Flags);
	FAssetRegistryModule::AssetCreated(NewDialogInfo);
	UPackage* InPackage = Cast<UPackage>(InParent);

	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	ModioUIHelpers::SavePackage(InPackage, NewDialogInfo, RF_Standalone | RF_Public, *InPackagePath);
	return NewDialogInfo;
}
