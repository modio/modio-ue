/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Foundation/Components/Tag/ModioCommonModTagEntry.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "UI/Foundation/Components/CheckBox/ModioCommonCheckBox.h"
#include "UI/Interfaces/IModioModBrowser.h"
#include "UI/Settings/Params/ModioCommonSearchParams.h"
#include "ModioUISubsystem.h"

void UModioCommonModTagEntry::SetTag_Implementation(const FString& InTag)
{
	Tag = InTag;
	if (TagButton)
	{
		TagButton->SetLabel(FText::FromString(Tag));
		TagButton->OnClicked().AddWeakLambda(this, [this]() 
		{
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				FModioModCategoryParams FilterParams;
				FilterParams.Tags = {Tag};
				Subsystem->ShowSearchResults(FilterParams, false);
				DeactivateWidget();
			}
		});
	}
	if (TagCheckbox)
	{
		TagCheckbox->SetLabel(FText::FromString(Tag));
	}
}

FString UModioCommonModTagEntry::GetTag() const
{
	return Tag;
}

bool UModioCommonModTagEntry::GetAllowMultipleSelection() const
{
	return bAllowMultipleSelection;
}

bool UModioCommonModTagEntry::GetSelected() const
{
	if (TagCheckbox)
	{
		return TagCheckbox->GetCheckedState() == ECheckBoxState::Checked;
	}
	return false;
}

void UModioCommonModTagEntry::SetSelected_Implementation(bool bIsSelected)
{
	if (TagCheckbox)
	{
		TagCheckbox->SetCheckedState(bIsSelected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}
}

void UModioCommonModTagEntry::SetAllowMultipleSelection_Implementation(bool bInAllowMultipleSelection)
{
	bAllowMultipleSelection = bInAllowMultipleSelection;
}

void UModioCommonModTagEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TagCheckbox)
	{
		TagCheckbox->OnModioCommonCheckBoxStateChanged.AddWeakLambda(this, [this](bool bIsChecked) {
			OnModTagEntrySelectionStateChanged.Broadcast(this, bIsChecked);
		});
	}
}

UWidget* UModioCommonModTagEntry::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}
	if (TagButton)
	{
		return TagButton;
	}
	if (TagCheckbox)
	{
		return TagCheckbox;
	}
	return Super::NativeGetDesiredFocusTarget();
}
