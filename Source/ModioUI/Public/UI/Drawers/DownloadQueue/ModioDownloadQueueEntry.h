// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Border.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioDownloadQueueEntry.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDownloadQueueEntry : public UModioUserWidgetBase,
											 public IUserObjectListEntry,
											 public IModioUIMediaDownloadCompletedReceiver
{
	GENERATED_BODY()
protected:
	virtual void NativeOnSetDataSource() override;
	UFUNCTION()
	virtual void OnUnsubClicked();
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image) override;

	const FModioRichTextStyle& GetRichTextStyle();

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioImage* ModThumbnail;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModNameLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModSizeLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* UnsubscribeButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ModStatusLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UBorder* EntryBorder;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioDownloadQueueEntryStyle"), Category="Widgets")
	FModioUIStyleRef EntryStyle;
};
