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
#include "UI/Foundation/Base/ModBrowser/ModioCommonModListBase.h"
#include "ModioCommonFeaturedView.generated.h"

class UModioCommonFeaturedViewStyle;
class UModioCommonActivatableWidget;
class UModioCommonFeaturedPrimaryView;
class UModioCommonFeaturedAdditionalView;
class UCommonRichTextBlock;

/**
 * @brief Featured View that displays primary and additional featured mods
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedView : public UModioCommonModListBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Featured View within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonFeaturedViewStyle> InStyle);

protected:
	/** @brief The style of the Featured View within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonFeaturedViewStyle> ModioStyle;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonFeaturedPrimaryView> PrimaryView;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonFeaturedAdditionalView> AdditionalView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UCommonRichTextBlock> CategoryLabel;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UModioCommonActivatableWidget> LastFocusedView;
};
