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
#include "CommonTabListWidgetBase.h"
#include "ModioCommonTabButtonBase.h"
#include "ModioCommonTabListWidgetBase.generated.h"

/**
 * @brief Tab List that displays a list of tabs
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTabSelectedFast, FName);
	FOnTabSelectedFast OnTabSelectedFast;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabButtonClicked, UCommonButtonBase*, TabButton);
	UPROPERTY(BlueprintAssignable, Category = "Mod.io Common UI")
	FOnTabButtonClicked OnTabButtonClicked;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTabButtonClickedFast, UCommonButtonBase*);
	FOnTabButtonClickedFast OnTabButtonClickedFast;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActionWidget> PreviousTabAction;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "Mod.io Common UI")
	TObjectPtr<UCommonActionWidget> NextTabAction;

public:
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void SetPreviousTabInputActionData(FDataTableRowHandle InPreviousTabInputActionData);

	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI")
	void SetNextTabInputActionData(FDataTableRowHandle InNextTabInputActionData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	bool RegisterDynamicTab(const FModioCommonTabDescriptor& TabDescriptor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void RemoveAllDynamicTabs();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleNavigateToNextTab();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void HandleNavigateToPreviousTab();

public:
	virtual void SynchronizeProperties() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	virtual void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;

public:
	virtual void SetListeningForInput(bool bShouldListen) override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mod.io Common UI")
	void OnTabSelectedInternal(FName TabNameID);

private:
	TMap<FName, FModioCommonTabDescriptor> PendingTabsMap;
};
