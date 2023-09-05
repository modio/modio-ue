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
 * @brief User Profile Widget that displays the basic information about a user
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonUserProfileWidget : public UModioCommonUserProfileBase
{
	GENERATED_BODY()

public:
	UModioCommonUserProfileWidget();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|User Profile|Widgets")
	TObjectPtr<UModioCommonButtonBase> ProfileButton;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
};
