// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "UI/BaseWidgets/ModioLoadingSpinner.h"
#include "UI/Dialogs/ModioDialogInfo.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Views/STileView.h"

#include "ModioDialogBaseInternal.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioDialogBaseInternal : public UWidget, public IModioUIInputHandler
{
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<struct FModioDialogButtonInfo>> ButtonParams;
	bool bBeingPreviewed = false;
	bool bUsingCustomButtons = false;
	SOverlay::FOverlaySlot* DialogContentOverlaySlot;
	SGridPanel::FSlot* InputWidgetSlot;
	SGridPanel::FSlot* ButtonWidgetSlot;
	SGridPanel::FSlot* SubHeaderWidgetSlot;
	SGridPanel::FSlot* DialogTextBlockSlot;
	SOverlay::FOverlaySlot* LoadingSpinnerOverlaySlot;
	TSharedPtr<SOverlay> DialogInternalRoot;
	TSharedPtr<class SImage> BackgroundImage;
	TSharedPtr<class SGridPanel> ContentPanel;
	TSharedPtr<class SModioRichTextBlock> HeadingTextBlock;
	TSharedPtr<class SModioRichTextBlock> DialogTextBlock;
	TSharedPtr<class STileView<TSharedPtr<FModioDialogButtonInfo>>> DialogButtons;

	UPROPERTY()
	UWidget* LoadingSpinner;

	UPROPERTY()
	UObject* DataSource;

	bool IsInputValid();

	virtual void BuildCommandList(TSharedRef<FUICommandList> InCommandList) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioDialogStyle"), Category="Widgets")
	FModioUIStyleRef DialogStyle = {FName("DefaultDialogStyle")};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FMargin ButtonSpacing;

	UPROPERTY()
	UWidget* SubHeaderWidget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	FReply OnButtonClicked(TSharedPtr<FModioDialogButtonInfo> Button);
	void ApplyStyling();
	void UpdateBoundValues(class UModioDialogInfo* DialogDescriptor);
	EVisibility GetButtonListVisibility() const;
	TSharedRef<class ITableRow> OnGenerateButton(TSharedPtr<struct FModioDialogButtonInfo> ButtonInfo,
												 const TSharedRef<class STableViewBase>& OwnerTableView);
	const FModioUIStyleRef* GetButtonTextStyle() const;

	UPROPERTY()
	TWeakObjectPtr<class UModioDialogController> Controller;

public:
	UPROPERTY()
	UWidget* InputWidget;

	void ShowLoadingSpinner();
	void HideLoadingSpinner();

	void SetDialogController(UModioDialogController* DialogController);
	void InitializeFromDialogInfo(class UModioDialogInfo* DialogDescriptor, bool bIsPreview = false,
								  UObject* DialogDataSource = nullptr);
	TOptional<FString> GetInputWidgetString();
	void SetInputWidgetString(FString Input);
};
