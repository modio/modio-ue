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
#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"
#include "Types/ModioFilterParams.h"
#include "ModioCommonSortingView.generated.h"

class UModioCommonCheckBox;
class UModioCommonCheckBoxStyle;
class UPanelWidget;

/**
 * @brief Sorting View that displays a list of sorting options
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonSortingView : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UModioCommonSortingView();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Style")
	TSubclassOf<UModioCommonCheckBoxStyle> SortingRadioButtonStyle;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Mod.io Common UI|Sorting View|Widget")
	TObjectPtr<UPanelWidget> SortingRadioButtonsContainer;

public:
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Sorting View")
	void ResetSorting();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Sorting View")
	bool GetSortFieldTypeAndDirection(EModioSortFieldType& OutSortFieldType, EModioSortDirection& OutSortDirection) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Sorting View")
	void UpdateSelection(UModioCommonCheckBox* CheckedRadioButton, bool bIsChecked, EModioSortFieldType ByField, EModioSortDirection ByDirection);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Sorting View")
	void AddSortingRadioButton(const FString& LabelText, EModioSortFieldType ByField, EModioSortDirection ByDirection, bool bCheckedByDefault = false);

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	/**
	 * Get the number of sorting options that are checked
	 * @return The number of sorting options that are checked
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI|Sorting View")
	int32 GetNumSortingOptionsChecked() const;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Sorting View")
	EModioSortFieldType CurrentSortFieldType;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Mod.io Common UI|Sorting View")
	EModioSortDirection CurrentSortDirection;
};
