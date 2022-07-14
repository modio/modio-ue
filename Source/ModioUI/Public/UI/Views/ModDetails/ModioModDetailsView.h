// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Types/ModioRating.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/CommonComponents/ModioModManagementWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/CommonComponents/ModioSubscriptionBadge.h"
#include "UI/CommonComponents/ModioTagList.h"
#include "UI/EventHandlers/IModioUIAuthenticationChangedReceiver.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "UI/Interfaces/IModioUIModDetailsDisplay.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"
#include "UI/Views/ModDetails/ModioModDetailsImageGallery.h"
#include "UI/Views/ModioMenuView.h"

#include "ModioModDetailsView.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioModDetailsView : public UModioMenuView,
										 public IModioUIModInfoReceiver,
										 public IModioUIModDetailsDisplay,
										 public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeDisplayModDetails(const TScriptInterface<class IModioModInfoUIDetails>& Details) override;
	virtual void NativeDisplayModDetailsForId(const FModioModID& ModID) override;
	virtual void NativeRequestOperationRetry() override;
	UFUNCTION()
	void RateUpClicked();
	UFUNCTION()
	void RateDownClicked();
	UFUNCTION()
	void ReportClicked();
	UFUNCTION()
	void OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating SubmittedRating);

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"))
	UWidget* DetailsLoader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioAsyncOpWrapperWidget* DetailsViewContent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextButton* SubscribeButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextButton* RateUpButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextButton* RateDownButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextButton* ReportButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioUserWidgetBase* ModPropertiesInspector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioModManagementWidgetBase* ProgressWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText SubscribeButtonFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RateUpTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RateDownTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ReportTextFormat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* ModTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* ModSummaryTextBlock;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* ModFullDescriptionTextBlock;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioTagList* ModTags;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioRichTextBlock* ModChangelog;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText SubscribeLabel;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText UnsubscribeLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioModDetailsImageGallery* ImageGallery;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UModioSubscriptionBadge* SubscriptionBadge;

#if WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Editor Preview Data")
	UModioModInfoUI* EditorPreviewDataSource;
#endif

	bool bCachedSubscriptionState;

	UFUNCTION()
	virtual void OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed);

	UFUNCTION()
	void NativeSubscribeClicked();

	void NativeOnSetDataSource() override;

	FModioModID CurrentModID;
	void NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ec, TOptional<FModioModInfo> Info);

public:
	void ShowDetailsForMod(FModioModID ID);
};
