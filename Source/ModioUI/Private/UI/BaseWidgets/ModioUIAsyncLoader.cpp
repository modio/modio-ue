// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/Interfaces/IModioUIAsyncRetryWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"

void UModioUIAsyncLoader::NativeLinkAsyncOperationWidget(const TScriptInterface<IModioUIAsyncOperationWidget>& Widget)
{
	FOnSetOperationState Delegate;
	Delegate.BindDynamic(this, &UModioUIAsyncLoader::NativeHandleAsyncOperationStateChange);
	IModioUIAsyncOperationWidget::Execute_SetOperationStateDelegate(Widget.GetObject(), Delegate);
}

TSharedRef<SWidget> UModioUIAsyncLoader::RebuildWidget()
{
	UWidget* ContentWidget = GetContentWidget();

	return SAssignNew(MyBox, SBox).Content()[ContentWidget ? ContentWidget->TakeWidget() : SNullWidget::NullWidget];
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
	MyBox.Reset();
}

void UModioUIAsyncLoader::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (UWidget* RetryWidget = GetContentForSlot("Error Widget"))
	{
		FOnRetryRequested Delegate;
		Delegate.BindDynamic(this, &UModioUIAsyncLoader::OnRetryRequested);
		IModioUIAsyncRetryWidget::Execute_SetRetryRequestedDelegate(RetryWidget, Delegate);
	}
}

void UModioUIAsyncLoader::OnRetryRequested()
{
	if (UWidget* ContentWidget = GetContentForSlot("Widget"))
	{
		IModioUIAsyncOperationWidget::Execute_RequestOperationRetry(ContentWidget);
	}
}

void UModioUIAsyncLoader::NativeHandleAsyncOperationStateChange(EModioUIAsyncOperationWidgetState NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
		if (MyBox.IsValid())
		{
			if (MyBox->GetChildren()->Num())
			{
				if (MyBox->GetChildren()->GetChildAt(0) != SNullWidget::NullWidget)
				{
					MyBox->GetChildren()->GetChildAt(0)->ConditionallyDetatchParentWidget(MyBox.Get());
				}
			}
			UWidget* ContentWidget = GetContentWidget();
			MyBox->SetContent(ContentWidget ? ContentWidget->TakeWidget() : SNullWidget::NullWidget);
		}
	}
}
