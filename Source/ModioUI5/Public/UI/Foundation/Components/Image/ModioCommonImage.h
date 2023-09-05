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
 * @brief Image that displays an image with an optional frame
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonImage : public UModioCommonImageBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Image within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonImageStyle> InStyle);

protected:
	/** @brief The style of the Image within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonImageStyle> ModioStyle;

	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> ImageFrame;

	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image;

public:
	virtual void SetImageTexture_Implementation(UTexture2DDynamic* Texture) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Synchronize Properties"))
	void BP_SynchronizeProperties();

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	UTexture2DDynamic* LoadedImageTexture;
};
