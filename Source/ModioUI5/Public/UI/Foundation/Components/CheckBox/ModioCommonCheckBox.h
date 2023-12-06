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
#include "UObject/Object.h"
#include "Components/CheckBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "ModioCommonCheckBox.generated.h"

class UModioCommonTextBlock;
class UModioCommonCheckBoxStyle;

/**
 * @brief Check Box that can be used in Mod.io UI
 * It supports styling through the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonCheckBox : public UCheckBox
{
	GENERATED_BODY()

public:
	UModioCommonCheckBox();

	/**
	 * @brief Sets the style of the Check Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonCheckBoxStyle> InStyle);

protected:
	/** @brief The style of the Check Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonCheckBoxStyle> ModioStyle;

public:
	UFUNCTION(BlueprintCallable, Category = "Text")
	void SetLabel(const FText& InLabelText);

	UFUNCTION(BlueprintCallable, Category = "Text")
	FText GetLabel() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnModioCommonCheckBoxStateChanged, bool);
	FOnModioCommonCheckBoxStateChanged OnModioCommonCheckBoxStateChanged;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI")
	TObjectPtr<UModioCommonTextBlock> LabelTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	FText LabelText;

public:
	virtual void SynchronizeProperties() override;
protected:
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	FText GetLabelTextBlockText() const;

	virtual TSharedRef<SWidget> GetContentWidget();
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	FMargin GetCheckboxContentPadding() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleOnCheckStateChanged(bool bIsChecked);
};
