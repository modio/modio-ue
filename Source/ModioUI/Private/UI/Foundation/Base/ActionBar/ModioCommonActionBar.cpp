/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/ActionBar/ModioCommonActionBar.h"

#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "CommonUI/Private/Input/UIActionRouterTypes.h"
#endif
#include "Input/CommonUIActionRouterBase.h"
#include "Misc/EngineVersionComparison.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#if UE_VERSION_NEWER_THAN(5, 1, 0)
#include "Input/UIActionBinding.h"
#endif

void UModioCommonActionBar::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (GameInstance->GetGameViewportClient())
		{
			GameInstance->GetGameViewportClient()->OnPlayerAdded().AddUObject(this, &UModioCommonActionBar::HandlePlayerAdded);
		}

		for (const ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
		{
			MonitorPlayerActions(LocalPlayer);
		}

		// Establish entries (as needed) immediately upon construction
		HandleDeferredDisplayUpdate();
	}
}

void UModioCommonActionBar::HandleDeferredDisplayUpdate_Implementation()
{
	
}

int32 UModioCommonActionBar::GetNumInputActions() const
{
	int32 NumActions = 0;
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		TArray<ULocalPlayer*> Players = GameInstance->GetLocalPlayers();
		for (const ULocalPlayer* LocalPlayer : Players)
		{
			if (const UCommonUIActionRouterBase* ActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(LocalPlayer))
			{
				TSet<FName> AcceptedBindings;
				TArray<FUIActionBindingHandle> FilteredBindings = ActionRouter->GatherActiveBindings().FilterByPredicate([ActionRouter, &AcceptedBindings](const FUIActionBindingHandle& Handle) mutable
				{
					if (TSharedPtr<FUIActionBinding> Binding = FUIActionBinding::FindBinding(Handle))
					{
						if (!Binding->bDisplayInActionBar)
						{
							return false;
						}
#if UE_VERSION_NEWER_THAN(5, 2, 0)
						if (CommonUI::IsEnhancedInputSupportEnabled())
						{
							if (TObjectPtr<const UInputAction> InputAction = Binding->InputAction.Get())
							{
								const UCommonInputSubsystem& InputSubsystem = ActionRouter->GetInputSubsystem();
								bool bAccept = CommonUI::ActionValidForInputType(ActionRouter->GetLocalPlayer(), InputSubsystem.GetCurrentInputType(), InputAction);
								AcceptedBindings.Add(Binding->ActionName, &bAccept);
								return bAccept;
							}
						}
#endif

						bool bAlreadyAccepted = false;
						AcceptedBindings.Add(Binding->ActionName, &bAlreadyAccepted);
						return !bAlreadyAccepted;
					}
					return false;
				});
				NumActions += AcceptedBindings.Num();
			}
		}
	}
	return NumActions;
}

void UModioCommonActionBar::HandleBoundActionsUpdated_Implementation(bool bFromOwningPlayer)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameInstance->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UModioCommonActionBar::HandleDeferredDisplayUpdate));
	}
}

void UModioCommonActionBar::HandlePlayerAdded_Implementation(int32 PlayerIdx)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (const ULocalPlayer* NewPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIdx))
		{
			MonitorPlayerActions(NewPlayer);
			HandleBoundActionsUpdated(NewPlayer == GetOwningLocalPlayer());
		}
	}
}

void UModioCommonActionBar::MonitorPlayerActions_Implementation(const ULocalPlayer* NewPlayer)
{
	if (const UCommonUIActionRouterBase* ActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(NewPlayer))
	{
		ActionRouter->OnBoundActionsUpdated().AddUObject(this, &UModioCommonActionBar::HandleBoundActionsUpdated, NewPlayer == GetOwningLocalPlayer());
	}
}

