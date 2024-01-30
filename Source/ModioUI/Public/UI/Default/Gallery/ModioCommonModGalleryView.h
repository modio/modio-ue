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
#include "Types/ModioModInfo.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "ModioCommonModGalleryView.generated.h"

class UHorizontalBox;
class UModioCommonModGalleryEntry;
class UModioCommonDynamicImage;
class UModioCommonImageStyle;
class UModioCommonButtonBase;
class UModioCommonModGalleryViewStyle;
class UModioCommonListView;
class UModioCommonErrorWithRetryWidget;;

/**
 * Mod Gallery View that displays a list of mod gallery entries to be navigated and switched between
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModGalleryView
	: public UModioCommonActivatableWidget,
	  public IModioUIAsyncOperationWidget
{
	GENERATED_BODY()

public:
	UModioCommonModGalleryView();

	/**
	 * Sets the style of the Mod Gallery View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModGalleryViewStyle> InStyle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModGalleryViewStyle> ModioStyle;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	//~ Begin UModioCommonActivatableWidget Interface
	virtual void NativeOnSetDataSource() override;
	virtual void BindInputActions() override;
	virtual void UnbindInputActions() override;
	//~ End UModioCommonActivatableWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonListView> ImageNavButtons;

	/** Stored property to selected gallery image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonDynamicImage> SelectedGalleryImage;

	/** Stored property to the previous button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> PreviousButton;
	
	/** Stored property to the next button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> NextButton;

	/** Loader widget to display while the mod gallery is loading */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "ModioUIAsyncHandlerWidget"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UWidget> ModGalleryLoader;

	/** Widget to display when there is an error loading the mod gallery */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonErrorWithRetryWidget> ErrorWithRetryWidget;

	/** Updates the buttons visibility based on the CurrentImageIndex */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void RefreshGallery();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetPreviousButtonVisibility(bool bVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetNextButtonVisibility(bool bVisible);

public:
	/** Go to next image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToNextImage();

	/** Go to previous image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToPreviousImage();

	/** 
	* Go to previous image
	* @returns Returns the image at the current index
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetCurrentImageGalleryIndex();

	/** 
	* Go to previous image
	* @returns Returns the number of gallery images
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	int32 GetNumGalleryImages();

	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddDefaultGalleryImage();

	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddGalleryImage(int32 ImageGalleryIndex);

private:
	int32 CurrentImageGalleryIndex = 0;
};