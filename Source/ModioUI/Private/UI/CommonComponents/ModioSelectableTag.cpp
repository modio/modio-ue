/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioSelectableTag.h"
#include "Components/ListView.h"
#include "Components/ListViewBase.h"
#include "UI/CommonComponents/ModioListViewString.h"

bool UModioSelectableTag::IsCheckboxChecked()
{
	return TagSelectedCheckbox->IsChecked();
}

FString UModioSelectableTag::GetTagString()
{
	return TagSelectedCheckbox->GetLabelText().ToString();
}

void UModioSelectableTag::SetTagLabel(FString InLabel)
{
	SearchString = InLabel;
	if (TagSelectedCheckbox)
	{
		UModioUISubsystem* subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (!subsystem)
		{
			TagSelectedCheckbox->SetLabelText(FText::FromString(SearchString));
		}
		else
		{
			TagSelectedCheckbox->SetLabelText(subsystem->GetLocalizedTag(SearchString));
		}
	}
}

void UModioSelectableTag::OnCheckboxCheckStateChanged(bool bIsChecked)
{
	OnTagStateChanged.Broadcast(this, bIsChecked);
}


void UModioSelectableTag::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UModioSelectableTag::NativeConstruct() 
{
	Super::NativeConstruct();
	FScriptDelegate onCheckboxStateChanged;
	onCheckboxStateChanged.BindUFunction(this, "OnCheckboxCheckStateChanged");
	TagSelectedCheckbox->OnCheckStateChanged.AddUnique(onCheckboxStateChanged);
}

void UModioSelectableTag::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) 
{
	if (const FModioCheckBoxStyle* ResolvedStyle = FocusedStyle.FindStyle<FModioCheckBoxStyle>())
	{
		TagSelectedCheckbox->SetStyle(ResolvedStyle);
		FSlateApplication::Get().PlaySound(ResolvedStyle->HoveredSlateSound);
	}
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UModioSelectableTag::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) 
{
	if (const FModioCheckBoxStyle* ResolvedStyle = NormalStyle.FindStyle<FModioCheckBoxStyle>())
	{
		TagSelectedCheckbox->SetStyle(ResolvedStyle);
	}
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}
