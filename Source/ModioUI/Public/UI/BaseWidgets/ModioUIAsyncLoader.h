// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/NamedSlotInterface.h"
#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIAsyncHandlerWidget.h"
#include "Containers/Array.h"

#include "ModioUIAsyncLoader.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioUIAsyncLoader : public UWidget, public IModioUIAsyncHandlerWidget, public INamedSlotInterface
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	EModioUIAsyncOperationWidgetState CurrentState = EModioUIAsyncOperationWidgetState::InProgress;

	TSharedPtr<class SBox> MyBox;

	UPROPERTY()
	TMap<FName, UWidget*> NamedWidgets;

	virtual void GetSlotNames(TArray<FName>& SlotNames) const override;

	virtual UWidget* GetContentForSlot(FName SlotName) const override;

	virtual void SetContentForSlot(FName SlotName, UWidget* Content) override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;
	UFUNCTION()
	void OnRetryRequested();
	UFUNCTION()
	virtual void NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState);

	void NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget) override;

	TSharedRef<SWidget> RebuildWidget() override;

	virtual UWidget* GetContentWidget();

};
