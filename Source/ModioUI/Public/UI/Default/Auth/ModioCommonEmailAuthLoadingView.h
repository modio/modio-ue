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
#include "UI/Foundation/Base/Auth/ModioCommonEmailAuthLoadingViewBase.h"
#include "ModioCommonEmailAuthLoadingView.generated.h"

class UModioCommonTextBlock;
class UModioCommonButtonBase;

/**
 * Email Auth Loading View that displays a loading state while the email auth process is ongoing
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonEmailAuthLoadingView : public UModioCommonEmailAuthLoadingViewBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> DescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonButtonBase> CancelButton;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
};
