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

#include "CoreMinimal.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "ModioCommonErrorWithRetryWidget.generated.h"

class UModioCommonImage;
class UModioCommonTextBlock;
class UModioCommonButtonBase;
class UModioCommonErrorWithRetryWidgetStyle;

UCLASS(BlueprintType)
class MODIOUI_API UModioCommonErrorWithRetryParamsUI : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioCommonErrorWithRetryParamsUI")
	FText ErrorTitleTextLabel = NSLOCTEXT("Modio", "ErrorTitleText", "");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ModioCommonErrorWithRetryParamsUI")
	FText ErrorDescriptionTextLabel = NSLOCTEXT("Modio", "ErrorDescriptionText", "Couldn't fetch data");
};

/**
 * Error with retry widget that displays an error message and a retry button
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonErrorWithRetryWidget
	: public UModioCommonActivatableWidget,
	  public IModioUIAsyncRetryWidget
{
	GENERATED_BODY()

public:
	UModioCommonErrorWithRetryWidget();

	/**
	 * Sets the style of the Error With Retry widget within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonErrorWithRetryWidgetStyle> InStyle);

protected:
	/** The style of the Error With Retry widget within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonErrorWithRetryWidgetStyle> ModioStyle;

protected:
	/** Icon to display. The style can be set via SetStyle or in the ModioStyle (Style in Blueprints) property */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonImage> IconImage;

	/** Text block displaying the title for errors. The style can be set via SetStyle or in the ModioStyle (Style in Blueprints) property */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonTextBlock> ErrorTitleTextBlock;

	/** Text block displaying the description for errors. The style can be set via SetStyle or in the ModioStyle (Style in Blueprints) property */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonTextBlock> ErrorDescriptionTextBlock;

	/** Button to retry the operation. The style can be set via SetStyle or in the ModioStyle (Style in Blueprints) property */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI|Widgets", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonButtonBase> RetryButton;

	//~ Begin UUserWidget Interface
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	//~ End UModioCommonActivatableWidget Interface

public:
	DECLARE_MULTICAST_DELEGATE(FRetryClickedDelegate);
	/** Static delegate for when the retry button is clicked */
	FRetryClickedDelegate RetryClickedDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRetryClickedDynamicDelegate);
	/** Dynamic delegate for when the retry button is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FRetryClickedDynamicDelegate RetryClickedDynamicDelegate;
};