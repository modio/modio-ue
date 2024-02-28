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
#include "UI/Interfaces/IModioUIAsyncOperationWidget.h"
#include "Types/ModioCommonTypes.h"
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
	/** The style of the Mod Gallery View within the Mod.io Common UI styling system */
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

	/** The size of the gallery image to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	EModioGallerySize GallerySize = EModioGallerySize::Thumb1280;

	/** The size of the logo to display when there is no gallery image */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Properties")
	EModioLogoSize LogoSize = EModioLogoSize::Thumb1280;

	/** List of gallery image thumbnails used to navigate between images */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonListView> ImageNavButtons;

	/** Currently selected gallery image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonDynamicImage> SelectedGalleryImage;

	/** Button to go to the previous image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> PreviousButton;
	
	/** Button to go to the next image */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonButtonBase> NextButton;

	/** Loader widget to display while the mod gallery is loading */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, MustImplement = "/Script/ModioUICore.ModioUIAsyncHandlerWidget"), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UWidget> ModGalleryLoader;

	/** Widget to display when there is an error loading the mod gallery */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI|Widgets")
	TObjectPtr<UModioCommonErrorWithRetryWidget> ErrorWithRetryWidget;

	/** Updates the buttons visibility based on the CurrentImageIndex */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void RefreshGallery();

	/**
	 * Sets the visibility of the previous button
	 * @param bVisible Whether the previous button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetPreviousButtonVisibility(bool bIsVisible);

	/**
	 * Sets the visibility of the next button
	 * @param bVisible Whether the next button should be visible
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetNextButtonVisibility(bool bIsVisible);

public:
	/** Go to next image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToNextImage();

	/** Go to previous image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToPreviousImage();

	/**
	 * Gets the currently selected gallery image index
	 * @return The currently selected gallery image index
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetSelectedImageGalleryIndex();

	/**
	 * Go to previous image
	 * @returns Returns the number of gallery images
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	int32 GetNumGalleryImages();

	/**
	 * Adds a default gallery image to the gallery (if there is no gallery image)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddDefaultGalleryImage();

	/**
	 * Adds a gallery image to the gallery
	 * @param ImageGalleryIndex The index of the image to add (used to load via UModioSubsystem::GetModMediaAsync)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddGalleryImage(int32 ImageGalleryIndex);

private:
	/** Currently selected gallery image index */
	int32 CurrentImageGalleryIndex = 0;
};