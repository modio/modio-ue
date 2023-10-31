// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UObject/Object.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioTermsLinksWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTermsLinksWidget : public UWidget,
										   public IModioUIDialogButtonWidget,
										   public IModioUIStringInputWidget,
										   public IModioUIDataSourceWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UObject* DataSource;
	TSharedPtr<SHorizontalBox> ButtonList;
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedRef<SButton> CreateButton(FString URL, FText ButtonTitle);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "DefaultDialogButtonStyle"), Category = "Widgets")
	FModioUIStyleRef ButtonStyle {"DefaultDialogButtonStyle"};

	FReply OnLinkClicked(FString Link);

	void NativeSetDataSource(UObject* InDataSource) override;

	void NativeSetStyle(const FModioDialogStyle& Style) override;

	void NativeSetDialogController(class UModioDialogController* Controller) override;

	const FModioUIStyleRef* GetButtonTextStyle() const;
};
