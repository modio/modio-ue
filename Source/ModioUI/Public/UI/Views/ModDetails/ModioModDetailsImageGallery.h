// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "Styling/SlateTypes.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioImageGalleryBase.h"
#include "UI/CommonComponents/ModioListViewInteger.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "UI/Styles/ModioUIColorRef.h"

#include "ModioModDetailsImageGallery.generated.h"

// UStruct so that LoadedImage gets a hard reference
USTRUCT()
struct FModioModDetailsGalleryImageInfo
{
	GENERATED_BODY()
	TOptional<FModioErrorCode> Status;
	TOptional<FModioImageWrapper> DownloadedImageReference;
	UPROPERTY()
	UTexture2DDynamic* LoadedImage;
};

/**
 *
 */
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
												  TOptional<FModioImageWrapper> Image) override;
	virtual void NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ec, int32 ImageIndex,
														  TOptional<FModioImageWrapper> Image) override;
	virtual void BuildCommandList(TSharedRef<FUICommandList> CommandList) override;
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioImageGalleryBase* ImageGallery;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListViewInteger* NavButtons;
};

UCLASS()
class MODIOUI_API UModioModDetailsImageGalleryNavButtonBase : public UModioUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected);

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioImage* Image;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Appearance")
	TSoftObjectPtr<UMaterialInterface> SpecifiedMaterial;

	// TODO: @modio-ue4-ui the colors here should possibly be put into some kind of common style

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance",
			  meta = (EditCondition = "Image!=nullptr", EditConditionHides))
	FSlateColor UnselectedColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance",
			  meta = (EditCondition = "Image!=nullptr", EditConditionHides))
	FSlateColor SelectedColor;
};