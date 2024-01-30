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
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "Types/ModioModInfo.h"

#include "ModioCommonModGalleryEntry.generated.h"

class UModioCommonModGalleryView;
class UModioCommonImage;
class UModioCommonButtonBase;
class UModioCommonDynamicImage;
class UModioCommonModGalleryEntryStyle;

UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModGalleryItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	FModioModInfo ModInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	int32 ImageGalleryIndex = -1;
};

/**
 * Mod Gallery Entry that displays a mod image to be used in a Mod Gallery View
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModGalleryEntry : public UModioCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Mod Gallery Entry within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonModGalleryEntryStyle> InStyle);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonModGalleryEntryStyle> ModioStyle;
	
protected:
	/** Mod image to display */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonDynamicImage> ModImage;

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	/**
	 * Updates the styling of the widget based on the current selection state
	 * @param bIsListItemSelected Whether the entry is currently selected
	 */
	virtual void NativeUpdateStyling(bool bIsListItemSelected);
};
