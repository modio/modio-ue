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

#include "Components/NamedSlotInterface.h"
#include "Components/Widget.h"
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Images/SImage.h"

#include "ModioUIAsyncLoadingOverlay.generated.h"

/**
* Modio UI element that represents an asynchronous loading overlay.
* It has text, an image, a dialog, and a button to display
**/
UCLASS()
class UModioUIAsyncLoadingOverlay : public UModioUserWidgetBase
{
	GENERATED_BODY()

public:
	/**
	* Replace the internal base dialog
	* @param Dialog Pointer to the dialog replacement
	**/
	void SetOwningDialog(UModioDialogBaseInternal* Dialog)
	{
		OwningDialog = Dialog;
	}

	void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	void SetDialogFocus();

private:
	UModioDialogBaseInternal* OwningDialog;

protected:
	UFUNCTION()
	void Close();
	TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

	void ApplyStyling();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioDialogStyle"), Category = "Widgets")
	FModioUIStyleRef DialogStyle = {FName("DefaultDialogStyle")};

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* CancelButton;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* LoadingText;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	UImage* BackgroundImage;
};
