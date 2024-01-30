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
#include "UI/Foundation/Base/Auth/ModioCommonTermsOfUseViewBase.h"
#include "ModioCommonTermsOfUseView.generated.h"

class UModioCommonButtonBase;
class UModioCommonTextBlock;

/**
 * Terms Of Use View that displays the terms of use and allows the user to accept or reject them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonTermsOfUseView : public UModioCommonTermsOfUseViewBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> ContentTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> TermsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> PrivacyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> SubmitButton;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
};
