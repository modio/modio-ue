/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "ModioDialogInfoFactory.generated.h"

/**
 *
 */
UCLASS()
class MODIOUIEDITOR_API UModioDialogInfoFactory : public UFactory
{
	GENERATED_BODY()

public:
	UModioDialogInfoFactory();
	virtual FText GetDisplayName() const override;
	virtual bool ShouldShowInNewMenu() const override;

	bool DoesSupportClass(UClass* Class);
	virtual uint32 GetMenuCategories() const override;

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context,
							  FFeedbackContext* Warn) override;
};
