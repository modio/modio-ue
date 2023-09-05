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
#include "UI/Default/ModEntry/ModioCommonGenericModEntry.h"
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"

#include "ModioCommonFeaturedPrimaryLargeModEntry.generated.h"

class UPanelWidget;
class UModioCommonRichTextBlock;
class UModioCommonDynamicImage;
class UModioCommonTextBlock;
class UModioCommonButtonBase;

/**
 * @brief Primary Large Mod Entry that can be used to display mod information and perform mod operations
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonFeaturedPrimaryLargeModEntry : public UModioCommonGenericModEntry
{
	GENERATED_BODY()

public:
	UModioCommonFeaturedPrimaryLargeModEntry();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLargeModEntryMouseChanged, bool);
	FOnLargeModEntryMouseChanged OnLargeModEntryMouseChanged;

protected:
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	//~ End IUserObjectListEntry Interface
	
	//~ Begin UModioCommonGenericModEntry Interface
	virtual bool IsModListItemValid_Implementation() const override;
	virtual bool IsModListItemSelected_Implementation() const override;
	//~ End UModioCommonGenericModEntry Interface

	//~ Begin UWidget Interface
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeConstruct() override;
	//~ End UWidget Interface

	virtual void NativeOnSetDataSource() override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Primary Large Mod Entry")
	void RefreshForegroundLayerVisibility();
	
	bool bSelected = false;
};