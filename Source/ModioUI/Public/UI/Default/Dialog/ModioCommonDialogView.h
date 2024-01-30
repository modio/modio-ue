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
#include "ModioSubsystem.h"
#include "UI/Foundation/Base/Dialog/ModioCommonDialogViewBase.h"
#include "UObject/ObjectPtr.h"
#include "ModioCommonDialogView.generated.h"

class UModioCommonWidgetSwitcher;
class UModioCommonDialogMessageViewBase;
struct FModioErrorCode;
struct FModioReportParams;

/**
 * Report View that displays a report reason view, email view and message view, with automatic switching between them
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonDialogView : public UModioCommonDialogViewBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonWidgetSwitcher> DialogSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonDialogMessageViewBase> DialogMessageViewClass;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeOnSetDataSource() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:

	void OnDialogMessageViewSubmitClicked();

private:

	UPROPERTY(Transient)
	TObjectPtr<UModioCommonDialogMessageViewBase> DialogMessageView;
};
