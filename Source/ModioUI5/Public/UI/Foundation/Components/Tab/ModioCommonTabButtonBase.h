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

USTRUCT(BlueprintType)
struct FModioCommonTabDescriptor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI")
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonTabButtonBase> TabButtonType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonTabButtonStyle> TabButtonStyle;
};

UINTERFACE(BlueprintType)
class UModioCommonTabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

class IModioCommonTabButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabLabelInfo(const FModioCommonTabDescriptor& InTabLabelInfo);
};

/**
 * @brief Tab Button that is displayed in a Tab List
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonTabButtonBase : public UModioCommonButtonBase, public IModioCommonTabButtonInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Tab Button within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	void SetStyle(TSubclassOf<UModioCommonTabButtonStyle> InStyle);

protected:
	virtual void SetTabLabelInfo_Implementation(const FModioCommonTabDescriptor& InTabLabelInfo) override;

private:
	FModioCommonTabDescriptor TabLabelInfo;
};
