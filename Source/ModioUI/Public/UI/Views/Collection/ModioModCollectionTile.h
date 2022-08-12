// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/MenuAnchor.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioPopupMenu.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioSizeBox.h"
#include "UI/CommonComponents/ModioModTileBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/Interfaces/IModioUIModEnableWidget.h"
#include "UI/Views/Collection/ModioCollectionTileStatus.h"

#include "ModioModCollectionTile.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioModCollectionTile : public UModioModTileBase,
											public IModioUIModEnableWidget,
											public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()
protected:
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnSetExpandedState(bool bExpanded) override;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating);
	UFUNCTION()
	void SubmitNegativeRating();
	UFUNCTION()
	void SubmitPositiveRating();
	UFUNCTION()
	void SubmitModReport();
	UFUNCTION()
	void ForceUninstall();
	UFUNCTION()
	void NativeMoreOptionsClicked();
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* SizeOnDiskLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioPopupMenu* MoreOptionsMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioCollectionTileStatus* StatusWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* StatusLine;

	// We don't need a text for any other status because all mods are either subscribed for the local user or installed
	// for another user

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Localization)
	FText SubscribedStatusText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Localization)
	FText InstalledStatusText;

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image) override;

	virtual void OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;

	virtual UModioModInfoUI* GetAsModInfoUIObject() override;

	virtual void BuildCommandList(TSharedRef<FUICommandList> CommandList) override;
};
