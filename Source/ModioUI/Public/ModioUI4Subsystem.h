/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Misc/EngineVersionComparison.h"
#include "Core/Input/InputDeviceTypes.h"
#include "Core/Input/ModioUIInputProcessor.h"
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Engine/Engine.h"
#include "Framework/Commands/UICommandList.h"
#include "ModioSubsystem.h"
#include "Subsystems/EngineSubsystem.h"

#include "ModioUI4Subsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuTabIndexChanged, int, TabIndex);


/**
 *
 */
UCLASS() class MODIOUI_API UModioUI4Subsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	virtual void GetPreloadDependencies(TArray<UObject*>& OutDeps) override;

	friend class IModioUIMediaDownloadCompletedReceiver;
	friend class IModioUIModInfoReceiver;
	friend class IModioUIModManagementEventReceiver;
	friend class IModioUIInputDeviceChangedReceiver;
	friend class IModioUIUserChangedReceiver;
	friend class IModioUIUserAvatarDownloadCompletedReceiver;
	friend class IModioUIAuthenticationChangedReceiver;
	friend class IModioUIModEnableWidget;
	friend class IModioUINotificationController;
	friend class IModioUIModDetailsDisplay;

	TSharedPtr<FUICommandList> CommonUICommands = nullptr;
	TSharedPtr<class FModioUIInputProcessor> Processor;

	UPROPERTY()
	UModioUIStyleSet* LoadedDefaultStyleSet = nullptr;

	EModioUIInputMode LastInputMode;

	UPROPERTY()
	UWidget* CurrentFocusTarget;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void HandleInputModeChanged(EModioUIInputMode NewDevice);

	int32 GetActiveTabIndex();

	TSharedPtr<FUICommandList> GetCommandList()
	{
		if (!CommonUICommands)
		{
			CommonUICommands = MakeShared<FUICommandList>();
		}
		return CommonUICommands;
	}

	void SetActiveTabIndex(int32 TabIndex);
	void CloseSearchDrawer();
	void CloseDownloadDrawer();
	void ShowReportEmailDialog(UObject* DialogDataSource);
	void ShowUninstallConfirmationDialog(UObject* DialogDataSource);
	bool IsAnyDialogOpen();
	bool IsDownloadDrawerOpen();

	EModioUIInputMode GetLastInputDevice()
	{
		return LastInputMode;
	}

	FOnMenuTabIndexChanged OnMenuTabIndexChanged;
	FOnInputDeviceChanged OnInputDeviceChanged;

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	class UModioUIStyleSet* GetDefaultStyleSet();

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	FText GetLocalizedTag(FString InTag);

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	UMaterialInterface* GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType);

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	TArray<FName> GetAllNamedStyleNames();

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	void SetCurrentFocusTarget(UWidget* currentTarget);

	UFUNCTION(BlueprintCallable, Category = "ModioUI4Subsystem")
	UWidget* GetCurrentFocusTarget();

	void SetControllerOverrideType(EModioUIInputMode NewOverride);
	void ClearControllerOverride();

	void SendCommandToBrowserUI(FKey CommandKey, int32 UserIndex = 0);
};
