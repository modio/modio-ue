/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Objects/ModioOpenEditorWidgetAction.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Editor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioOpenEditorWidgetAction)

bool UModioOpenEditorWidgetAction::ExecuteAction(FString Args) const
{
	if (Args.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to execute Open Editor Widget action but no path to an editor widget was provided."));
		return false;
	}

	if(UEditorUtilityWidgetBlueprint* Blueprint = LoadObject<UEditorUtilityWidgetBlueprint>(GetTransientPackage(), *Args))
	{
		if (UEditorUtilitySubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
		{
			Subsystem->SpawnAndRegisterTab(Blueprint);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				   TEXT("Tried to execute Open Editor Widget action but could not get the EditorUtilitySubsystem."));
			return false;
		}
	}

	UE_LOG(LogTemp, Warning,
		   TEXT("Tried to execute Open Editor Widget action but cold not resolve the path to the widget."));
	return false;
}