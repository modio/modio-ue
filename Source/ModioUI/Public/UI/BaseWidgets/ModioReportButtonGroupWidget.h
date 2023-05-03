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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UObject/Object.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SButton.h"

#include "ModioReportButtonGroupWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioReportButtonGroupWidget : public UWidget,
												  public IModioUIDialogButtonWidget,
												  public IModioUIDataSourceWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UObject* DataSource;

protected:
	virtual void NativeSetDataSource(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	TWeakObjectPtr<UModioDialogController> DialogController;

	virtual void NativeSetDialogController(class UModioDialogController* Controller) override
	{
		DialogController = Controller;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "DefaultDialogButtonStyle"), Category = "Widgets")
	FModioUIStyleRef ButtonStyle {"DefaultDialogButtonStyle"};

	const FModioUIStyleRef* GetButtonTextStyle() const;

	UFUNCTION()
	void CancelClicked();

	TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeSetStyle(const FModioDialogStyle& Style);

	FReply OnReportButtonClicked(EModioReportType ModioReport);

	TSharedRef<SButton> CreateButton(EModioReportType ReportType, FText ButtonTitle);

	TSharedPtr<SVerticalBox> ButtonList;
};
