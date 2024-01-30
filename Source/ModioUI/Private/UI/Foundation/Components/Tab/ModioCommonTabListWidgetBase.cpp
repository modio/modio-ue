/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/Tab/ModioCommonTabListWidgetBase.h"
#include "UI/Foundation/Components/Tab/ModioCommonTabButtonStyle.h"
#include "CommonActionWidget.h"
#include "Groups/CommonButtonGroupBase.h"

void UModioCommonTabListWidgetBase::SetPreviousTabInputActionData(FDataTableRowHandle InPreviousTabInputActionData)
{
	if (PreviousTabInputActionData == InPreviousTabInputActionData) 
	{
		// When the binding is the same, no need to call again
		return;
	}

	PreviousTabInputActionData = InPreviousTabInputActionData;
	UpdateBindings();
}

void UModioCommonTabListWidgetBase::SetNextTabInputActionData(FDataTableRowHandle InNextTabInputActionData)
{
	if (NextTabInputActionData == InNextTabInputActionData)
	{
		// When the binding is the same, no need to call again
		return;
	}

	NextTabInputActionData = InNextTabInputActionData;
	UpdateBindings();
}

bool UModioCommonTabListWidgetBase::RegisterDynamicTab_Implementation(const FModioCommonTabDescriptor& TabDescriptor)
{
	PendingTabsMap.Add(TabDescriptor.TabId, TabDescriptor);
	if (!GetOwningPlayer())
	{
#if WITH_EDITOR
		if (IsDesignTime())
		{
			UCommonButtonBase* const NewTabButton = CreateWidget<UCommonButtonBase>(GetWorld(), TabDescriptor.TabButtonType);
			if (TabButtonGroup)
			{
				TabButtonGroup->AddWidget(NewTabButton);
			}
			HandleTabCreation(TabDescriptor.TabId, NewTabButton);
			OnTabButtonCreation.Broadcast(TabDescriptor.TabId, NewTabButton);
			return true;
		}
#endif
		return false;
	}
	else
	{
		return RegisterTab(TabDescriptor.TabId, TabDescriptor.TabButtonType, nullptr);
	}
}

void UModioCommonTabListWidgetBase::RemoveAllDynamicTabs_Implementation()
{
	RemoveAllTabs();
#if WITH_EDITOR
	if (IsDesignTime())
	{
		if (TabButtonGroup)
		{
			TabButtonGroup->RemoveAll();
		}
	}
#endif
}

void UModioCommonTabListWidgetBase::HandleNavigateToNextTab_Implementation()
{
	if (TabButtonGroup)
	{
		TabButtonGroup->SelectNextButton();
	}
}

void UModioCommonTabListWidgetBase::HandleNavigateToPreviousTab_Implementation()
{
	if (TabButtonGroup)
	{
		TabButtonGroup->SelectPreviousButton();
	}
}

bool UModioCommonTabListWidgetBase::SelectTabByIDExtended(FName TabNameID, bool bSuppressClickFeedback, bool bSuppressOnTabSelectedIfAlreadySelected)
{
	if (GetSelectedTabId() == TabNameID && !bSuppressOnTabSelectedIfAlreadySelected)
	{
		OnTabSelected.Broadcast(TabNameID);
	}
	return SelectTabByID(TabNameID, bSuppressClickFeedback);
}

void UModioCommonTabListWidgetBase::SynchronizeProperties()
{
	if (!GetInputSubsystem())
	{
		Super::SynchronizeProperties();
		return;
	}

	//UpdateBindings();
	
	const bool bShowNextAndPrevious = bIsListeningForInput && GetTabCount() > 1;
	if (PreviousTabAction)
	{
		PreviousTabAction->SetVisibility(bShowNextAndPrevious ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		PreviousTabAction->SetInputAction(bShowNextAndPrevious ? PreviousTabInputActionData : FDataTableRowHandle());
	}
	if (NextTabAction)
	{
		NextTabAction->SetVisibility(bShowNextAndPrevious ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		NextTabAction->SetInputAction(bShowNextAndPrevious ? NextTabInputActionData : FDataTableRowHandle());
	}

	Super::SynchronizeProperties();
}

void UModioCommonTabListWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OnTabSelected.AddDynamic(this, &UModioCommonTabListWidgetBase::OnTabSelectedInternal);
}

void UModioCommonTabListWidgetBase::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	if (TabButton)
	{
		TabButton->SetIsFocusable(false);
		TabButton->OnClicked().AddWeakLambda(this, [this, TabButton]() {
			if (IsValid(TabButton))
			{
				OnTabButtonClickedFast.Broadcast(TabButton);
				OnTabButtonClicked.Broadcast(TabButton);
			}
		});
	}
	Super::HandleTabCreation_Implementation(TabNameID, TabButton);

	if (TabButton)
	{
		FModioCommonTabDescriptor* TabInfoPtr = PendingTabsMap.Find(TabNameID);
		if (TabButton->GetClass()->ImplementsInterface(UModioCommonTabButtonInterface::StaticClass()))
		{
			if (ensureMsgf(TabInfoPtr, TEXT("A tab button was created with id %s but no label info was specified. RegisterDynamicTab should be used over RegisterTab to provide label info."), *TabNameID.ToString()))
			{
				IModioCommonTabButtonInterface::Execute_SetTabLabelInfo(TabButton, *TabInfoPtr);
				if (TabInfoPtr->TabButtonStyle)
				{
					TabButton->SetStyle(TabInfoPtr->TabButtonStyle);
				}
			}
		}
	}

	PendingTabsMap.Remove(TabNameID);
	SynchronizeProperties();
}

void UModioCommonTabListWidgetBase::HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	Super::HandleTabRemoval_Implementation(TabNameID, TabButton);
	PendingTabsMap.Remove(TabNameID);
}

void UModioCommonTabListWidgetBase::SetListeningForInput(bool bShouldListen)
{
	Super::SetListeningForInput(bShouldListen);
	SynchronizeProperties();
}

void UModioCommonTabListWidgetBase::OnTabSelectedInternal_Implementation(FName TabNameID)
{
	SynchronizeProperties();
	OnTabSelectedFast.Broadcast(TabNameID);
}
