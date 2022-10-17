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

#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioDrawerContentBase.generated.h"

/**
* Modio UI element that wraps what content is displayed using background images
* either within its header or main content
**/
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

	UFUNCTION(BlueprintImplementableEvent, Category = "ModioDrawerContentBase")
	UWidget* GetHeaderWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = "ModioDrawerContentBase")
	UWidget* GetContentWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = "ModioDrawerContentBase")
	UWidget* GetOptionalContentWidget();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	UNamedSlot* HeaderSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	UNamedSlot* ContentSlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	UNamedSlot* OptionalContentSlot;

public:
};
