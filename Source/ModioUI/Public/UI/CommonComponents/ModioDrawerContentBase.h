// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioDrawerContentBase.generated.h"
/**
 *
 */
UCLASS()
class MODIOUI_API UModioDrawerContentBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	TSharedPtr<SImage> HeaderBackgroundImage;
	TSharedPtr<SImage> ContentBackgroundImage;
	TSharedPtr<SImage> OptionalContentBackgroundImage;
	TSharedPtr<SVerticalBox> MyVerticalBox;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void NativeOnInitialized() override;
	// blueprint overridable functions
	// default implementation checks the delegate and executes it
	// Otherwise fall back to either a tsubclassof, or if that's null, the null widget

	// We want a delegate for close
	// that way the drawer content can tell the drawer to go

	bool bRoutedGetWidget = false;
	virtual TSharedRef<SWidget> NativeGetHeaderWidget();
	virtual TSharedRef<SWidget> NativeGetContentWidget();
	virtual TSharedRef<SWidget> NativeGetOptionalContentWidget();

	UFUNCTION(BlueprintImplementableEvent, Category="ModioDrawerContentBase")
	UWidget* GetHeaderWidget();

	UFUNCTION(BlueprintImplementableEvent, Category="ModioDrawerContentBase")
	UWidget* GetContentWidget();

	UFUNCTION(BlueprintImplementableEvent, Category="ModioDrawerContentBase")
	UWidget* GetOptionalContentWidget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	UNamedSlot* HeaderSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	UNamedSlot* ContentSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	UNamedSlot* OptionalContentSlot;

public:
};
