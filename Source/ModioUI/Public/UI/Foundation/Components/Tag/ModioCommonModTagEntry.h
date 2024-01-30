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
#include "Types/ModioModTag.h"

#include "ModioCommonModTagEntry.generated.h"

class UModioCommonCheckBox;
class UModioCommonButtonBase;

/**
 * Mod Tag Entry that represents a single mod tag entry
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonModTagEntry : public UModioCommonActivatableWidget
{
	GENERATED_BODY()

public:
	/** Delegate for when the selection state of the ModTagEntry changes */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModTagEntrySelectionStateChanged, UModioCommonModTagEntry* /** ModTagEntry */, bool /** bIsSelected */);
	FOnModTagEntrySelectionStateChanged OnModTagEntrySelectionStateChanged;

protected:
	/** The button that represents this tag */
	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonButtonBase> TagButton;

	/** The checkbox that represents this tag */
	UPROPERTY(BlueprintReadWrite, Category = "Mod.io Common UI", meta = (BindWidgetOptional))
	TObjectPtr<UModioCommonCheckBox> TagCheckbox;

	/** Whether this tag entry allows multiple selection */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	FString Tag;

	/** Whether this tag entry allows multiple selection */
	UPROPERTY(BlueprintReadOnly, Category = "Mod.io Common UI")
	bool bAllowMultipleSelection;

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	virtual UWidget* NativeGetDesiredFocusTarget() const override;

public:
	/**
	 * Sets the tag for this entry to the specified tag
	 * @param InTag The tag to set
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetTag(const FString& InTag);

	/**
	 * Gets the tag for this entry
	 * @return The tag for this entry
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mod.io Common UI")
	FString GetTag() const;

	/**
	 * Sets whether this tag entry allows multiple selection
	 * @param bInAllowMultipleSelection Whether this tag entry allows multiple selection
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetAllowMultipleSelection(bool bInAllowMultipleSelection);

	/**
	 * Gets whether this tag entry allows multiple selection
	 * @return Whether this tag entry allows multiple selection
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mod.io Common UI")
	bool GetAllowMultipleSelection() const;

	/**
	 * Sets whether this tag entry is selected
	 * @param bIsSelected Whether this tag entry is selected
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void SetSelected(bool bIsSelected);

	/**
	 * Gets whether this tag entry is selected
	 * @return Whether this tag entry is selected
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mod.io Common UI")
	bool GetSelected() const;
};
