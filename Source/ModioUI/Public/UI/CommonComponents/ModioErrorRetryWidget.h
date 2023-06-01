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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "UObject/Object.h"

#include "ModioErrorRetryWidget.generated.h"

DECLARE_DELEGATE(FOnRetryClickedDelegate);

/**
* Modio UI element that represents a Modio Error retry widget, it displays
* the error text and has a retry button whem available
**/
UCLASS()
class MODIOUI_API UModioErrorRetryWidget : public UModioUserWidgetBase, public IModioUIAsyncRetryWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION()
	void OnRetryClicked();

	virtual void SynchronizeProperties() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ErrorText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FString ErrorTextValue;

public:
	/**
	* Change the text error in this widget
	* @param Error The updated string to store in this view
	**/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* RetryButton;

	UFUNCTION()
	void SetErrorText(FString Error)
	{
		ErrorTextValue = Error;
	}
};
