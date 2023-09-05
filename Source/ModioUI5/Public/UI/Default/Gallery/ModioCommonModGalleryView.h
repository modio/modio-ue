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
#include "ModioCommonModGalleryView.generated.h"

class UHorizontalBox;
class UModioCommonModGalleryEntry;
class UModioCommonDynamicImage;
class UModioCommonImageStyle;
class UModioCommonButtonBase;
class UModioCommonModGalleryViewStyle;
class UModioCommonListView;

/**
 * @brief Mod Gallery View that displays a list of mod gallery entries to be navigated and switched between
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonModGalleryView : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UModioCommonModGalleryView();

	/**
	 * @brief Sets the style of the Mod Gallery View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModGalleryViewStyle> InStyle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModGalleryViewStyle> ModioStyle;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;

	virtual void BindInputActions() override;
	virtual void UnbindInputActions() override;

	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UModioCommonListView> ImageNavButtons;

	/** @brief Stored property to selected gallery image */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UModioCommonDynamicImage> SelectedGalleryImage;

	/** @brief Stored property to the previous button */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UModioCommonButtonBase> PreviousButton;
	
	/** @brief Stored property to the next button */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UModioCommonButtonBase> NextButton;

	/** @brief Updates the buttons visibility based on the CurrentImageIndex */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void RefreshGallery();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetPreviousButtonVisibility(bool bVisible);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetNextButtonVisibility(bool bVisible);

public:

	/** @brief Loads a list of Mod information into the ModList */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void LoadGallery(FModioModInfo InModInfo);

	/** @brief Go to next image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToNextImage();

	/** @brief Go to previous image */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void GoToPreviousImage();

	/** 
	* @brief Go to previous image
	* @returns Returns the image at the current index
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	int32 GetCurrentImageGalleryIndex();

	/** 
	* @brief Go to previous image
	* @returns Returns the number of gallery images
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	int32 GetNumGalleryImages();

	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddDefaultGalleryImage();

	UFUNCTION(BlueprintNativeEvent, Category = "Mod.io Common UI")
	void AddGalleryImage(int32 ImageGalleryIndex);

private:
	FModioModInfo ModInfo;
	int32 CurrentImageGalleryIndex = 0;
};