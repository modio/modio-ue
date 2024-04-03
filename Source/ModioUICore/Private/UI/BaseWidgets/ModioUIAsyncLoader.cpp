/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "ModioUISubsystem.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void UModioUIAsyncLoader::NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget)
{
	FOnSetOperationState Delegate;
	Delegate.BindDynamic(this, &UModioUIAsyncLoader::NativeHandleAsyncOperationStateChange);
	IModioUIAsyncOperationWidget::Execute_SetOperationStateDelegate(Widget.GetObject(), Delegate);
}

EModioUIAsyncOperationWidgetState UModioUIAsyncLoader::NativeGetAsyncOperationState() const
{
	return CurrentState;
}

void UModioUIAsyncLoader::NativeSetOnOperationStateDelegate(const FOnChangeAsyncHandlerOperationState& Delegate)
{
	OnOperationStateChangeDelegate = Delegate;
}

TSharedRef<SWidget> UModioUIAsyncLoader::RebuildWidget()
{
	SAssignNew(MyWidgetSwitcher, SWidgetSwitcher);

	if (UWidget* Widget = GetContentForSlot("Error Widget"))
	{
		MyWidgetSwitcher->AddSlot()[Widget->TakeWidget()];
	}

	if (UWidget* Widget = GetContentForSlot("Loading Widget"))
	{
		MyWidgetSwitcher->AddSlot()[Widget->TakeWidget()];
	}

	if (UWidget* Widget = GetContentForSlot("Widget"))
	{
		MyWidgetSwitcher->AddSlot()[Widget->TakeWidget()];
	}

	if (UWidget* Widget = GetContentWidget())
	{
		MyWidgetSwitcher->SetActiveWidget(Widget->TakeWidget());
	}
	
	return MyWidgetSwitcher.ToSharedRef();
}

UWidget* UModioUIAsyncLoader::GetContentWidget()
{
	switch (CurrentState)
	{
		case EModioUIAsyncOperationWidgetState::Error:
		{
			return GetContentForSlot("Error Widget");
		}
		case EModioUIAsyncOperationWidgetState::InProgress:
		{
			return GetContentForSlot("Loading Widget");
		}
		case EModioUIAsyncOperationWidgetState::Success:
		{
			return GetContentForSlot("Widget");
		}
	}
	return nullptr;
}

void UModioUIAsyncLoader::GetSlotNames(TArray<FName>& SlotNames) const
{
	SlotNames.Add(FName("Error Widget"));
	SlotNames.Add(FName("Loading Widget"));
	SlotNames.Add(FName("Widget"));
}

UWidget* UModioUIAsyncLoader::GetContentForSlot(FName SlotName) const
{
	if (NamedWidgets.Contains(SlotName))
	{
		return NamedWidgets[SlotName];
	}
	return nullptr;
}

void UModioUIAsyncLoader::SetContentForSlot(FName SlotName, UWidget* Content)
{
	NamedWidgets.Add(SlotName, Content);
}

void UModioUIAsyncLoader::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	for (auto NamedWidget : NamedWidgets)
	{
		if (NamedWidget.Value)
		{
			NamedWidget.Value->ReleaseSlateResources(bReleaseChildren);
		}
	}
	MyWidgetSwitcher.Reset();
}

void UModioUIAsyncLoader::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (UWidget* RetryWidget = GetContentForSlot("Error Widget"))
	{
		if (RetryWidget->Implements<UModioUIAsyncRetryWidget>())
		{
			FOnRetryRequested Delegate;
			Delegate.BindDynamic(this, &UModioUIAsyncLoader::OnRetryRequested);
			IModioUIAsyncRetryWidget::Execute_SetRetryRequestedDelegate(RetryWidget, Delegate);

			UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			UISubsystem->OnRetryAllAsyncLoaders.AddUniqueDynamic(this, &UModioUIAsyncLoader::Retry);
		}
	}
}

void UModioUIAsyncLoader::OnRetryRequested()
{
	UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	UISubsystem->RetryAllAsyncLoaders();
}

void UModioUIAsyncLoader::Retry()
{
	if (UWidget* ContentWidget = GetContentForSlot("Widget"))
	{
		if (ContentWidget->Implements<UModioUIAsyncOperationWidget>())
		{
			IModioUIAsyncOperationWidget::Execute_RequestOperationRetry(ContentWidget);
		}
	}
}

void UModioUIAsyncLoader::NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState)
{
	if (NewState != CurrentState)
	{
		OnOperationStateChangeDelegate.ExecuteIfBound(NewState);
		CurrentState = NewState;
		if (MyWidgetSwitcher.IsValid())
		{
			if (UWidget* ContentWidget = GetContentWidget())
			{
				MyWidgetSwitcher->SetActiveWidget(ContentWidget->TakeWidget());
			}
		}
	}
}
