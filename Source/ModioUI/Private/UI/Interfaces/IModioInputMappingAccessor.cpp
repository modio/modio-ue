// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "Algo/Transform.h"
#include "Core/Input/ModioInputKeys.h"
#include "Engine/Engine.h"
#include "GameFramework/InputSettings.h"
#include "Input/Events.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"

bool operator==(const FInputActionKeyMapping& Mapping, const FKeyEvent& Event)
{
	FModifierKeysState ModifierKeys = Event.GetModifierKeys();
	return Mapping.Key == Event.GetKey() && (Mapping.bAlt == ModifierKeys.IsAltDown()) &&
		   (Mapping.bCmd == ModifierKeys.IsCommandDown()) && (Mapping.bCtrl == ModifierKeys.IsControlDown()) &&
		   (Mapping.bShift == ModifierKeys.IsShiftDown());
}

bool operator==(const FInputAxisKeyMapping& Mapping, const FKeyEvent& Event)
{
	return Mapping.Key == Event.GetKey();
}

TArray<FString> IModioInputMappingAccessor::GetModioInputMappingNames()
{
	return FModioInputNames::GetAllStrings();
}

TArray<FString> IModioInputMappingAccessor::GetAllProjectInputNames()
{
	UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings();
	if (!CurrentInputSettings)
	{
		return {"Could Not Get Input Settings"};
	}
	TArray<FString> InputNameStrings;
	TArray<FName> Names;
	CurrentInputSettings->GetAxisNames(Names);

	Algo::Transform(Names, InputNameStrings, [](FName CurrentName) { return CurrentName.ToString(); });
	CurrentInputSettings->GetActionNames(Names);
	Algo::Transform(Names, InputNameStrings, [](FName CurrentName) { return CurrentName.ToString(); });
	return InputNameStrings;
}

TArray<FKey> IModioInputMappingAccessor::GetModioInputKeys()
{
	TArray<FKey> Keys;
	EKeys::GetAllKeys(Keys);
	return Keys.FilterByPredicate([](const FKey& Key) { return Key.GetMenuCategory() == NAME_ModioInputKeys; });
}

TArray<FKey> IModioInputMappingAccessor::GetKeyForModioInputName(const FName Name)
{
	/*UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass());
	if (!CurrentUISettings)
	{
		return {};
	}
	FModioInputMapping* ModioInputMapping = CurrentUISettings->ModioToProjectInputMappings.FindByPredicate(
		[&Name](FModioInputMapping& Mapping) { return Mapping.ModioInputName == Name; });
	if (ModioInputMapping)
	{
		UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings();
		if (!CurrentInputSettings)
		{
			return {};
		}
		TArray<FKey> MappedKeys;
		TArray<FInputActionKeyMapping> Actions;
		TArray<FInputAxisKeyMapping> Axes;
		CurrentInputSettings->GetActionMappingByName(ModioInputMapping->ProjectInputName, Actions);
		CurrentInputSettings->GetAxisMappingByName(ModioInputMapping->ProjectInputName, Axes);
		for (FInputActionKeyMapping Mapping : Actions)
		{
			MappedKeys.Add(Mapping.Key);
		}
		for (FInputAxisKeyMapping Mapping : Axes)
		{
			MappedKeys.Add(Mapping.Key);
		}
		return MappedKeys;
	}*/
	return {};
}
PRAGMA_DISABLE_OPTIMIZATION
TOptional<FKey> IModioInputMappingAccessor::GetCommandKeyForEvent(const FKeyEvent& Event) const
{
	// If we're processing an event containing a modio command key, return the key directly and skip the lookup
	if (FModioInputKeys::GetAll().Contains(Event.GetKey()))
	{
		return Event.GetKey();
	}
	// Get our overall settings
	if (UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
	{
		// Get the global input settings
		if (UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings())
		{
			// Find the modio key which corresponds to the event
			FModioInputMapping* ModioInputMapping = CurrentUISettings->ModioToProjectInputMappings.FindByPredicate(
				[CurrentInputSettings, &Event](FModioInputMapping& Mapping) {
					// For each project action which is bound to the current modio key
					for (const auto& MappingName : Mapping.MappedProjectInputs)
					{
						TArray<FInputActionKeyMapping> Actions;
						// check if any of the action's input chords are equivalent to the specific chord in the event
						CurrentInputSettings->GetActionMappingByName(MappingName, Actions);
						for (const FInputActionKeyMapping& CurrentAction : Actions)
						{
							// If the action contains an input chord matching the event, select it
							if (CurrentAction == Event)
							{
								return true;
							};
						}

						TArray<FInputAxisKeyMapping> Axes;
						CurrentInputSettings->GetAxisMappingByName(MappingName, Axes);
						for (const FInputAxisKeyMapping& CurrentAxis : Axes)
						{
							return (CurrentAxis == Event);
						}
					}
					return false;
				});
			// If we found an action with a chord matching our event, and we know which modio key is associated with
			// that action...
			if (ModioInputMapping)
			{
				{
					return ModioInputMapping->VirtualKey;
				}
			}
		}
	}
	return {};
}

TOptional<FKey> IModioInputMappingAccessor::GetCommandKeyForAnalogEvent(const FAnalogInputEvent& Event) const
{
	return {};
}

PRAGMA_ENABLE_OPTIMIZATION

TSharedRef<const FUICommandList> IModioUIInputHandler::GetCommandList()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		TSharedRef<FUICommandList> NewCommandList = MakeShared<FUICommandList>(*Subsystem->GetCommandList().Get());
		BuildCommandList(NewCommandList);
		return NewCommandList;
	}
	return MakeShared<FUICommandList>();
}

bool IModioUIInputHandler::ProcessCommandForEvent(const FKeyEvent& Event)
{
	TOptional<FKey> CommandKey = IModioInputMappingAccessor::GetCommandKeyForEvent(Event);
	if (CommandKey.IsSet())
	{
		return GetCommandList()->ProcessCommandBindings(CommandKey.GetValue(), FModifierKeysState {}, false);
	}

	return false;
}
