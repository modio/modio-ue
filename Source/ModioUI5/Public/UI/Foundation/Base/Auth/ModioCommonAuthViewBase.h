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

#include "CoreMinimal.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "ModioCommonAuthViewBase.generated.h"

class UModioCommonTermsOfUseViewBase;
class UModioCommonEmailAuthViewBase;
class UModioCommonEmailAuthLoadingViewBase;
class UModioCommonEmailAuthCodeViewBase;

/**
 * @brief Auth View Base class for authentication views
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonAuthViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonTermsOfUseViewBase> TermsOfUseViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthViewBase> EmailAuthViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthLoadingViewBase> EmailAuthLoadingViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthCodeViewBase> EmailAuthCodeViewBaseClass;
};
