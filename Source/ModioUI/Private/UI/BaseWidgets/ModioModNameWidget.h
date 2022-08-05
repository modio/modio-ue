// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioModNameWidget.generated.h"

UCLASS()
class MODIOUI_API UModioModNameWidget : public UWidget, public IModioUIDataSourceWidget
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

	TSharedPtr<class SModioRichTextBlock> MyText;

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "DefaultDialogStyle"),
			  Category = "ModioModNameWidget")
	FModioUIStyleRef Style = FModioUIStyleRef {"DefaultDialogStyle"};
};
