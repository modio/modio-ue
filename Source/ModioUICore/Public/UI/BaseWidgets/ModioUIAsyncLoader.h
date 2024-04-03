/*
*  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/NamedSlotInterface.h"
#include "Components/Widget.h"
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "ModioUIAsyncLoader.generated.h"

/**
 * Async loader widget which can switch between three states: InProgress, Success, and Failure
 */
UCLASS(Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io UI"))
class MODIOUICORE_API UModioUIAsyncLoader : public UWidget, public IModioUIAsyncHandlerWidget, public INamedSlotInterface
{
	GENERATED_BODY()
protected:
	/**
	 * Current state of the async operation
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	EModioUIAsyncOperationWidgetState CurrentState = EModioUIAsyncOperationWidgetState::InProgress;

	/**
	 * Mapping of slot names ("Error Widget", "Loading Widget", "Widget") to the widgets that are assigned to them
	 */
	UPROPERTY()
	TMap<FName, UWidget*> NamedWidgets;

	//~ Begin INamedSlotInterface Interface
	virtual void GetSlotNames(TArray<FName>& SlotNames) const override;
	virtual UWidget* GetContentForSlot(FName SlotName) const override;
	virtual void SetContentForSlot(FName SlotName, UWidget* Content) override;
	//~ End INamedSlotInterface Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface


	//~ Begin UWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	/**
	 * Called when the retry is requested
	 */
	UFUNCTION()
	virtual void OnRetryRequested();

	/**
	 * Called when the async operation state changes
	 */
	UFUNCTION()
	virtual void NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState);

	//~ Begin IModioUIAsyncHandlerWidget Interface
	virtual void NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget) override;
	virtual EModioUIAsyncOperationWidgetState NativeGetAsyncOperationState() const override;
	virtual void NativeSetOnOperationStateDelegate(const FOnChangeAsyncHandlerOperationState& Delegate) override;
	//~ End IModioUIAsyncHandlerWidget Interface

	FOnChangeAsyncHandlerOperationState OnOperationStateChangeDelegate;

	/**
	 * Gets the content widget for the current state
	 * @return The content widget
	 */
	virtual UWidget* GetContentWidget();

public:
	/**
	 * Retries the async operation
	 */
	UFUNCTION()
	void Retry();

protected:
	/** Switcher that contains the three widgets for each state */
	TSharedPtr<class SWidgetSwitcher> MyWidgetSwitcher;
};
