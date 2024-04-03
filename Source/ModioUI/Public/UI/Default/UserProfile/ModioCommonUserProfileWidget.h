/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "UI/Foundation/Base/UserProfile/ModioCommonUserProfileBase.h"

#include "ModioCommonUserProfileWidget.generated.h"

/**
 * User Profile Widget that displays the basic information about a user
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonUserProfileWidget : public UModioCommonUserProfileBase
{
	GENERATED_BODY()

public:
	UModioCommonUserProfileWidget();

protected:
	/** Button that allows the user to view the profile of the user (quick access view) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|User Profile|Widgets")
	TObjectPtr<UModioCommonButtonBase> ProfileButton;

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface
};
