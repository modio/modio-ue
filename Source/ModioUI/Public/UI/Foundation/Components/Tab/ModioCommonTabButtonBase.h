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
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "ModioCommonTabButtonBase.generated.h"

class UModioCommonTabButtonStyle;
class UModioCommonTabButtonBase;
class UModioCommonActivatableWidget;

/**
 * Tab descriptor that contains information about the tab
 */
USTRUCT(BlueprintType)
struct FModioCommonTabDescriptor
{
	GENERATED_BODY()

public:
	/** The ID of the tab */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	FName TabId;

	/** The text to display on the tab */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	FText TabText;

	/** The widget to display when the tab is selected */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonTabButtonBase> TabButtonType;

	/** The widget to display when the tab is selected */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonTabButtonStyle> TabButtonStyle;
};

UINTERFACE(BlueprintType)
class UModioCommonTabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Tab Buttons
 */
class IModioCommonTabButtonInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets the tab label info
	 * @param InTabLabelInfo The tab label info
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabLabelInfo(const FModioCommonTabDescriptor& InTabLabelInfo);
};

/**
 * Tab Button that is displayed in a Tab List
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonTabButtonBase : public UModioCommonButtonBase, public IModioCommonTabButtonInterface
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Tab Button within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonTabButtonStyle> InStyle);

protected:
	//~ Begin IModioCommonTabButtonInterface Interface
	virtual void SetTabLabelInfo_Implementation(const FModioCommonTabDescriptor& InTabLabelInfo) override;
	//~ End IModioCommonTabButtonInterface Interface

private:
	/** Information about the tab label */
	FModioCommonTabDescriptor TabLabelInfo;
};
