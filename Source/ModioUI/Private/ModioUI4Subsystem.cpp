/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioUI4Subsystem.h"
#include "Core/Input/ModioInputKeys.h"
#include "Core/Input/ModioUIInputProcessor.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Math/IntPoint.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UI/Interfaces/IModioUIInputHintGlyphProvider.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/ModioDefaultInputGlyphProvider.h"
#include "UI/Styles/ModioUIStyleSet.h"

#include "ModioUICore.h"
#include "Blueprint/UserWidget.h"
#include "Loc/BeginModioLocNamespace.h"
#include "UI/CommonComponents/ModioMenu.h"
#include "UI/Interfaces/IModioModBrowser.h"

void UModioUI4Subsystem::GetPreloadDependencies(TArray<UObject*>& OutDeps)
{
	Super::GetPreloadDependencies(OutDeps);

	if (LoadedDefaultStyleSet)
	{
		OutDeps.Add(LoadedDefaultStyleSet);
		for (auto& object : LoadedDefaultStyleSet->GetAssetReferences())
		{
			OutDeps.Add(object);
		}
	}
}

void UModioUI4Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UModioSubsystem::StaticClass());
	// TODO: @modio-ue4 allow the input processor class to be loaded from project settings
	Processor = MakeShared<FModioUIInputProcessor>();
	// Bind to event on the processor
	Processor->OnInputDeviceChanged().AddUObject(this, &UModioUI4Subsystem::HandleInputModeChanged);
	if (FSlateApplication::IsInitialized())
	{
		// Register the input processor so we can detect input device changes
		FSlateApplication::Get().RegisterInputPreProcessor(Processor.ToSharedRef());
		// Retrieve the navigation config factory and set it on the application so we can map our virtual inputs to
		// navigation directions and actions
		if (const UModioUISettings* CurrentUISettings =
				UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
		{
			if (!CurrentUISettings->NavigationConfigOverride.IsNull())
			{
				if (UModioNavigationConfigFactoryBase* FactoryInstance =
						CurrentUISettings->NavigationConfigOverride.LoadSynchronous())
				{
					FSlateApplication::Get().SetNavigationConfig(FactoryInstance->CreateNavigationConfig());
				}
			}
		}
	}

	GetDefaultStyleSet();
}

UModioUIStyleSet* UModioUI4Subsystem::GetDefaultStyleSet()
{
	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
	if (!LoadedDefaultStyleSet)
	{
		// Could we load the user's override?
		if (Settings && !Settings->DefaultStyleSet.IsNull())
		{
			LoadedDefaultStyleSet = Settings->DefaultStyleSet.LoadSynchronous();
		}
		else
		{
			FSoftObjectPath Fallback("/Modio/UI/Styles/ModioUIDefaultStyle.ModioUIDefaultStyle");
			LoadedDefaultStyleSet = Cast<UModioUIStyleSet>(Fallback.TryLoad());
		}
	}

	checkf(LoadedDefaultStyleSet, TEXT("Unable to load default style set for the Modio UI module. Please ensure this "
									   "is set in your project settings window."));
	if (Settings)
	{
		LoadedDefaultStyleSet->bOverridePlatformMaterials = Settings->bOverridePlatformMaterials;
	}
	return LoadedDefaultStyleSet;
}

FText UModioUI4Subsystem::GetLocalizedTag(FString InTag)
{
	UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();
	if (!Settings)
	{
		return FText();
	}

	FText localizedTag = Settings->LocalizedTags.FindRef(InTag);
	if (localizedTag.IsEmpty())
	{
		// Workaround for NSLOCTEXT not accepting variables as values.
		// This is required for created text to be gathered for localization,
		// otherwise users will have to manually edit every FText variable to have them gathered for localization.
		Settings->LocalizedTags.Add(InTag,
									FInternationalization::ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(
										*InTag, TEXT("ModioLocalizedTags"), *InTag));
		Settings->SaveConfig();
		return Settings->LocalizedTags.FindRef(InTag);
	}
	return localizedTag;
}

UMaterialInterface* UModioUI4Subsystem::GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType)
{
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		#if WITH_EDITOR
		// This if for switching the glyphsets while testing. If it's set to Unknown in the settings,
		// this will have no effect, otherwise it will override the glyphset
		if (Settings->InputDevicesForTesting != EModioUIInputMode::Unknown && Processor &&
			!Processor->HasControllerOverrideType())
		{
			InputType = Settings->InputDevicesForTesting;
		}
		#endif
		if (UClass* GlyphProviderClass = Settings->InputHintGlyphProvider.Get())
		{
			if (UObject* TmpProvider = NewObject<UObject>(this, GlyphProviderClass))
			{
				return IModioUIInputHintGlyphProvider::Execute_GetInputGlyphMaterialForInputType(
					TmpProvider, VirtualInput, InputType);
			}
		}
		else if (UClass* testClass = UModioDefaultInputGlyphProvider::StaticClass())
		{
			if (UObject* testProvider = NewObject<UObject>(this, testClass))
			{
				return IModioUIInputHintGlyphProvider::Execute_GetInputGlyphMaterialForInputType(
					testProvider, VirtualInput, InputType);
			}
		}
	}

	return nullptr;
}

TArray<FName> UModioUI4Subsystem::GetAllNamedStyleNames()
{
	TArray<FName> StyleNames;
	UModioUIStyleSet* DefaultStyleSet = GetDefaultStyleSet();
	if (DefaultStyleSet)
	{
		StyleNames = DefaultStyleSet->GetAllStyleNames();
	}
	return StyleNames;
}

void UModioUI4Subsystem::SetCurrentFocusTarget(UWidget* currentTarget)
{
	CurrentFocusTarget = currentTarget;
}

UWidget* UModioUI4Subsystem::GetCurrentFocusTarget()
{
	return CurrentFocusTarget;
}

void UModioUI4Subsystem::SetControllerOverrideType(EModioUIInputMode NewOverride)
{
	if (Processor)
	{
		Processor->SetControllerOverrideType(NewOverride);
	}
}

void UModioUI4Subsystem::ClearControllerOverride()
{
	if (Processor)
	{
		Processor->ClearControllerOverride();
	}
}

void UModioUI4Subsystem::SendCommandToBrowserUI(FKey CommandKey, int32 UserIndex)
{
	if (FModioInputKeys::GetAll().Contains(CommandKey))
	{
		FSlateApplication::Get().ProcessKeyDownEvent(FKeyEvent(CommandKey, FModifierKeysState {}, UserIndex, false, 0, 0));
	}
}

void UModioUI4Subsystem::HandleInputModeChanged(EModioUIInputMode NewDevice)
{
	UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (!ModioUISubsystem)
	{
		return;
	}
	
	OnInputDeviceChanged.Broadcast(NewDevice);
	LastInputMode = NewDevice;

	// If we're on a controller, then we want to disable Synthetic Cursor Moves in Slate.
	// This means that any element that the mouse cursor is hovering over won't re-take focus as you scroll through with
	// a keyboard or controller
	if (NewDevice == EModioUIInputMode::Controller || NewDevice == EModioUIInputMode::Keyboard)
	{
		UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance);
		if (IsValid(MenuInstance))
		{
			MenuInstance->SetShowCursor(false);
		}

		if (IsValid(GetCurrentFocusTarget()) && !GetCurrentFocusTarget()->HasFocusedDescendants())
		{
			if (IsValid(MenuInstance) && MenuInstance->GetDrawerController()->IsAnyDrawerExpanded())
			{
				MenuInstance->GetDrawerController()->SetFocusToActiveDrawer();
			}
			else
			{
				GetCurrentFocusTarget()->SetFocus();
			}
		}
		if (!IsValid(GetCurrentFocusTarget()))
		{
			if (IsValid(MenuInstance))
			{
				MenuInstance->RefreshView(GetActiveTabIndex());
			}
		}
	}
	else if (NewDevice == EModioUIInputMode::Mouse)
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			MenuInstance->SetShowCursor(true);
		}
	}
}

int32 UModioUI4Subsystem::GetActiveTabIndex()
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			return MenuInstance->GetPageIndex();
		}
	}

	return INDEX_NONE;
}

void UModioUI4Subsystem::SetActiveTabIndex(int32 TabIndex)
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			OnMenuTabIndexChanged.Broadcast(TabIndex);
			MenuInstance->SetPageIndex(TabIndex);
		}
	}
}

void UModioUI4Subsystem::CloseSearchDrawer()
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			MenuInstance->ForceCloseSearchDrawer();
		}
	}
}
void UModioUI4Subsystem::CloseDownloadDrawer()
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			MenuInstance->ForceCloseDownloadDrawer();
		}
	}
}
void UModioUI4Subsystem::ShowReportEmailDialog(UObject* DialogDataSource)
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			MenuInstance->ShowReportEmailDialog(DialogDataSource);
		}
	}
}
void UModioUI4Subsystem::ShowUninstallConfirmationDialog(UObject* DialogDataSource)
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			MenuInstance->ShowUninstallConfirmationDialog(DialogDataSource);
		}
	}
}

bool UModioUI4Subsystem::IsAnyDialogOpen()
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			return MenuInstance->GetDialogController()->DialogStack.Num() > 0;
		}
	}

	return false;
}

bool UModioUI4Subsystem::IsDownloadDrawerOpen()
{
	if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioMenu* MenuInstance = Cast<UModioMenu>(ModioUISubsystem->ModBrowserInstance))
		{
			return MenuInstance->Execute_IsDownloadDrawerOpen(MenuInstance);
		}
	}

	return false;
}

#include "Loc/EndModioLocNamespace.h"
