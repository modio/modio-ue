// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Components/Widget.h"
#include "Core/Input/ModioUIDirection.h"
#include "CoreMinimal.h"
#include "Misc/CoreMiscDefines.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"

#include "ModioImageGalleryBase.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioImageGalleryBase : public UWidget
{
	GENERATED_BODY()
public:
	// This should return a value so that the gallery knows if it it should keep polling
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDisplayImage, UWidget*, ImageWidget, int32, ImageIndex);

protected:
	TSharedPtr<SOverlay> MyGalleryRootWidget = nullptr;
	TSharedPtr<class SFxWidget> ForegroundImageWrapper;
	TSharedPtr<SFxWidget> BackgroundImageWrapper;

	UPROPERTY(Transient)
	UWidget* ForegroundContent;
	UPROPERTY(Transient)
	UWidget* BackgroundContent;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

	virtual UWidget* GenerateContentWidget();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (IsBindableEvent = "True"), Category="Widgets")
	FOnDisplayImage OnDisplayImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (IsBindableEvent = "True"), Category="Widgets")
	FGetWidget OnConstructWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (IsBindableEvent = "True"), Category="Widgets")
	FGetInt32 OnGetItemCount;

	/// @brief Widget class to wrap as the images in this gallery. Bind OnConstructWidget if you want to do something
	/// else
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (MustImplement = ModioUIImageDisplay), Category="Widgets")
	TSubclassOf<UUserWidget> ImageWidgetClass;
	int32 CurrentImageIndex = 0;

public:
	UFUNCTION(BlueprintCallable, Category="ModioImageGalleryBase")
	void DisplayGalleryImageAtIndex(EModioUIDirection Direction, int32 DirectIndex = -1);
	UFUNCTION(BlueprintCallable, Category="ModioImageGalleryBase")
	void ResetGallery();

	UFUNCTION(BlueprintCallable, Category="ModioImageGalleryBase")
	void RefreshCurrentImage();

	virtual FOnDisplayImage& GetImageDisplayDelegate()
	{
		return OnDisplayImage;
	}
	virtual FGetWidget& GetConstructContentWidgetDelegate()
	{
		return OnConstructWidget;
	}
	virtual FGetInt32& GetItemCountDelegate()
	{
		return OnGetItemCount;
	}
};
