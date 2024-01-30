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
#include "UI/Foundation/Base/Image/ModioCommonImageBase.h"
#include "ModioCommonImage.generated.h"

class UModioCommonImageStyle;

/**
 * Image that displays an image with an optional frame
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonImage : public UModioCommonImageBase
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Image within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonImageStyle> InStyle);

protected:
	/** The style of the Image within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonImageStyle> ModioStyle;

	/**
	 * The frame of the image. Optional
	 * It lies on the bottom layer of the image
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> ImageFrame;

	/** The image */
	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image;

public:
	//~ Begin UModioCommonImageBase Interface
	virtual void SetImageTexture_Implementation(UTexture2DDynamic* Texture) override;
	//~ End UModioCommonImageBase Interface

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//~ End UUserWidget Interface

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface
protected:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	/**
	 * An alternative to SynchronizeProperties that is BlueprintImplementableEvent
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Synchronize Properties"))
	void BP_SynchronizeProperties();

protected:
	/** The loaded texture of the image */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	UTexture2DDynamic* LoadedImageTexture;
};
