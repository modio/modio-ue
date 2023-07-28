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

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "UI/CommonComponents/ModioImageGalleryBase.h"
#include "UI/CommonComponents/ModioListViewInteger.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioDynamicImageStyle.h"

#include "ModioModDetailsImageGallery.generated.h"

/**
 * Struct so that LoadedImage gets a hard reference
 **/
USTRUCT()
struct FModioModDetailsGalleryImageInfo
{
	GENERATED_BODY()

	/**
	 * Stored optional property of the error code
	 **/
	TOptional<FModioErrorCode> Status;

	/**
	 * Stored optional property of the downloaded image reference
	 **/
	TOptional<FModioImageWrapper> DownloadedImageReference;

	/**
	 * Stored property of the loaded dynamic texture image
	 **/
	UPROPERTY()
	UTexture2DDynamic* LoadedImage {};
};

/**
 * Base class that stores details of a mod image gallery in an array of
 * ModioModDetailsGalleryImageInfo.
 **/
UCLASS()
class MODIOUI_API UModioModDetailsImageGallery : public UModioUserWidgetBase,
												 public IModioUIMediaDownloadCompletedReceiver
{
	GENERATED_BODY()

protected:
	int32 CurrentImageIndex = 0;
	int32 ImageCount = 0;

	UFUNCTION()
	void OnGalleryImageLoad(class UTexture2DDynamic* LoadedImage, FModioModID ModID, int32 Index);

	UPROPERTY()
	TArray<FModioModDetailsGalleryImageInfo> ImageInfo;

	virtual void NativeOnInitialized() override;
	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
												  TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize) override;
	virtual void NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec, int32 ImageIndex,
														  TOptional<FModioImageWrapper> Image) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	virtual void PrevImage();
	UFUNCTION()
	virtual void NextImage();

	UFUNCTION()
	virtual void ApplyImageToWidget(UWidget* Widget, int32 ImageIndex);
	UFUNCTION()
	virtual int32 GetImageCount();

	virtual void OnNavButtonClicked(TSharedPtr<int64> Item);

	void ShowImageInternal(int32 RawIndex);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioImageGalleryBase* ImageGallery;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioListViewInteger* NavButtons;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioDynamicImageStyle"), Category = "Widgets")
	FModioUIStyleRef Style = FModioUIStyleRef {"DefaultDynamicImageStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRoundedImage* ActiveBackground;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	class UModioGridPanel* NavButtonGridPanel;

public:
	UPROPERTY(BlueprintReadWrite, Category="Widgets")
	bool bIsFocused = false;

	// function for changing image on controller/keyboard
	void ChangeImage(const FKey& InputKey);
};

/**
 * Base class that stores details of a mod image gallery navigation button
 **/
UCLASS()
class MODIOUI_API UModioModDetailsImageGalleryNavButtonBase : public UModioUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioImage* Image;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	TSoftObjectPtr<UMaterialInterface> SpecifiedMaterial;

	// TODO: @modio-ue4-ui the colors here should possibly be put into some kind of common style

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance",
			  meta = (EditCondition = "Image!=nullptr", EditConditionHides))
	FSlateColor UnselectedColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance",
			  meta = (EditCondition = "Image!=nullptr", EditConditionHides))
	FSlateColor SelectedColor;
};