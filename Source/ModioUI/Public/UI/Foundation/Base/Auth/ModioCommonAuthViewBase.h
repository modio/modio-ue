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
 * Auth View Base class for authentication views
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonAuthViewBase : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	/**
	 * The terms of use view class
	 * Used to display the terms of use
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonTermsOfUseViewBase> TermsOfUseViewClass;

	/**
	 * The email auth view class
	 * Used to display the email auth view, where the user can enter their email address
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthViewBase> EmailAuthViewClass;

	/**
	 * The email auth loading view class
	 * Used to display the email auth loading view after the user has entered their email address
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthLoadingViewBase> EmailAuthLoadingViewClass;

	/**
	 * The email auth code view class
	 * Used to display the email auth code view after the server has sent the email auth code
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "Mod.io Common UI|Auth View")
	TSubclassOf<UModioCommonEmailAuthCodeViewBase> EmailAuthCodeViewBaseClass;
};
