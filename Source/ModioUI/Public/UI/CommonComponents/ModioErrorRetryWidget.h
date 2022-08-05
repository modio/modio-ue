// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "UObject/Object.h"

#include "ModioErrorRetryWidget.generated.h"

DECLARE_DELEGATE(FOnRetryClickedDelegate);

/**
 *
 */
UCLASS()
class MODIOUI_API UModioErrorRetryWidget : public UModioUserWidgetBase, public IModioUIAsyncRetryWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION()
	void OnRetryClicked();

	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* RetryButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ErrorText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Widgets")
	FString ErrorTextValue;

public:
	UFUNCTION()
	void SetErrorText(FString Error)
	{
		ErrorTextValue = Error;
	}
};
