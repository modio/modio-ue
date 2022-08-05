// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UObject/Object.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "Widgets/Layout/SGridPanel.h"
#include "ModioReportButtonGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUI_API UModioReportButtonGroupWidget : public UWidget, public IModioUIDialogButtonWidget, public IModioUIDataSourceWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UObject *DataSource;

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
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "DefaultDialogButtonStyle"), Category="Widgets")
	FModioUIStyleRef ButtonStyle {"DefaultDialogButtonStyle"};
	
	TArray<TSharedPtr<SButton>> Buttons;

	const FModioUIStyleRef* GetButtonTextStyle() const;
	
	UFUNCTION()
	void CancelClicked();
	
	TSharedRef<SWidget> RebuildWidget() override;
	
	virtual void NativeSetStyle(const FModioDialogStyle& Style);
	
	FReply OnReportButtonClicked(EModioReportType ModioReport);

	TSharedRef<SButton> CreateButton(EModioReportType ReportType, FText ButtonTitle);
	
	TSharedPtr<SVerticalBox> ButtonList;
};
