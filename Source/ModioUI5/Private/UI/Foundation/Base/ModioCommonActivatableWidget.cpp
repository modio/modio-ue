/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Base/ModioCommonActivatableWidget.h"

#include "ModioUI5.h"
#include "ModioUISubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/Foundation/Components/Button/ModioCommonButtonBase.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"
#include "Algo/RemoveIf.h"

#if WITH_EDITOR
#include "Blueprint/WidgetTree.h"
#include "Editor/WidgetCompilerLog.h"
#endif

void UModioCommonActivatableWidget::SetDataSource(UObject* NewDataSource)
{
	bRoutedSetDataSource = false;

	DataSource = NewDataSource;

	// Forward to overrideable native implementation
	NativeOnSetDataSource();

	// Verify that the derived implementation called our base implementation
	checkf(bRoutedSetDataSource, TEXT("Please ensure you call Super::NativeOnSetDataSource in your overridden "
									  "implementation to route notifications to Blueprint"));
}

FString UModioCommonActivatableWidget::GetRequestIdentifier_Implementation()
{
	return FString::FromInt(GetUniqueID());
}

void UModioCommonActivatableWidget::NativeOnSetDataSource()
{
	bRoutedSetDataSource = true;
	OnDataSourceUpdated();
	SynchronizeProperties();
}

void UModioCommonActivatableWidget::BindInputActions()
{
	for (FListeningInputActionStruct& ListeningInputAction : ListeningInputActions)
	{
		if (ListeningInputAction.InputAction.IsNull())
		{
			continue;
		}
		if (ListeningInputAction.Button.IsValid())
		{
			if (!ListeningInputAction.DisplayName.IsEmpty())
			{
				ListeningInputAction.Button->OverrideActionDisplayName = ListeningInputAction.DisplayName;
			}
			ListeningInputAction.Button->SetTriggeringInputAction(ListeningInputAction.InputAction);
			BoundInputActions.Add(FBoundInputActionStruct(ListeningInputAction.Button));
			UE_LOG(ModioUI5, Log, TEXT("Bound input action '%s' to button '%s' ('%s') in widget '%s'"), *ListeningInputAction.Button->GetName(), *ListeningInputAction.Button->GetName(), *ListeningInputAction.DisplayName.ToString(), *GetName());
		}
	}
}

void UModioCommonActivatableWidget::BP_BindInputActions()
{
	BindInputActions();
}

void UModioCommonActivatableWidget::UnbindInputActions()
{
	for (FBoundInputActionStruct& BoundInputAction : BoundInputActions)
	{
		if (BoundInputAction.Button.IsValid())
		{
			UE_LOG(ModioUI5, Log, TEXT("Unbound input action from button '%s' in widget '%s'"), *BoundInputAction.Button->GetName(), *GetName());
			BoundInputAction.Button->SetTriggeringInputAction(FDataTableRowHandle());
		}
		if (BoundInputAction.ActionHandle.IsValid())
		{
			UE_LOG(ModioUI5, Log, TEXT("Unbound input action '%s' in widget '%s'"), *BoundInputAction.ActionHandle.GetActionName().ToString(), *GetName());
			BoundInputAction.ActionHandle.Unregister();
		}
	}
	BoundInputActions.Empty();
}

void UModioCommonActivatableWidget::BP_UnbindInputActions()
{
	UnbindInputActions();
}

void UModioCommonActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAutoBindInputAction)
	{
		UnbindInputActions();
		BindInputActions();
	}
}

void UModioCommonActivatableWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (GetInputSubsystem())
	{
		UnbindInputActions();
	}
}

void UModioCommonActivatableWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	BP_SynchronizeProperties();
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
	{
		OnViewportResized(FVector2D(GEngine->GameViewport->Viewport->GetSizeXY()));
	}
}

void UModioCommonActivatableWidget::BP_ListenForInputAction(UModioCommonButtonBase* Button,	FDataTableRowHandle InputAction, const FText& DisplayName, const FOnModioCommonActivatableWidgetActionFired& OnActionFired)
{
	ListenForInputAction(Button, InputAction, DisplayName, FOnModioCommonActivatableWidgetActionFiredFast::CreateWeakLambda(this, [OnActionFired]() {
		OnActionFired.ExecuteIfBound();
	}));
}

void UModioCommonActivatableWidget::ListenForInputAction(TObjectPtr<UModioCommonButtonBase> Button, FDataTableRowHandle InputAction, const FText& DisplayName, const FOnModioCommonActivatableWidgetActionFiredFast& OnActionFired)
{
	if (!InputAction.IsNull())
	{
		if (Button)
		{
			ListeningInputActions.Add(FListeningInputActionStruct(InputAction, DisplayName, TWeakObjectPtr<UModioCommonButtonBase>(Button), OnActionFired));
		}
		else
		{
			UE_LOG(ModioUI5, Error, TEXT("Cannot listen for input action with a null button in widget '%s'"), *GetName());
		}
	}

	if (bAutoBindInputAction)
	{
		UnbindInputActions();
		BindInputActions();
	}

	for (FListeningInputActionStruct& ListeningInputAction : ListeningInputActions)
	{
		if (ListeningInputAction.InputAction.IsNull())
		{
			continue;
		}
		if (ListeningInputAction.Button.IsValid())
		{
			ListeningInputAction.Button->OnClicked().RemoveAll(this);
			ListeningInputAction.Button->OnClicked().AddWeakLambda(this, [this, ListeningInputAction]() {
				ListeningInputAction.OnActionFired.ExecuteIfBound();
			});
		}
	}
}

void UModioCommonActivatableWidget::ClearListeningInputActions()
{
	UnbindInputActions();
	ListeningInputActions.Empty();
}

void UModioCommonActivatableWidget::ClearListeningInputAction(UModioCommonButtonBase* Button)
{
	for (FBoundInputActionStruct& BoundInputAction : BoundInputActions)
	{
		if (BoundInputAction.Button.IsValid() && BoundInputAction.Button.Get() == Button)
		{
			UE_LOG(ModioUI5, Log, TEXT("Unbound input action from button '%s' in widget '%s'"), *BoundInputAction.Button->GetName(), *GetName());
			BoundInputAction.Button->SetTriggeringInputAction(FDataTableRowHandle());
		}
	}
	BoundInputActions.RemoveAll([Button](const FBoundInputActionStruct& BoundInputAction) {
		return BoundInputAction.Button.IsValid() && BoundInputAction.Button.Get() == Button;
	});
	ListeningInputActions.RemoveAll([Button](const FListeningInputActionStruct& ListeningInputAction) {
		return ListeningInputAction.Button.IsValid() && ListeningInputAction.Button.Get() == Button;
	});
}

void UModioCommonActivatableWidget::FocusOnDesiredWidget()
{
	if (GetOwningPlayer())
	{
		if (UWidget* WidgetToFocus = GetDesiredFocusTarget())
		{
			WidgetToFocus->SetFocus();
		}
	}
}

void UModioCommonActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FViewport::ViewportResizedEvent.AddWeakLambda(this, [this](FViewport* Viewport, uint32 Params) {
		if (GetOwningPlayer() && GetWorld() && GetWorld()->GetGameViewport() && Viewport == GetWorld()->GetGameViewport()->Viewport)
		{
			OnViewportResized(FVector2D(Viewport->GetSizeXY()));
		}
	});
}

void UModioCommonActivatableWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	OnFocusChangedFast.Broadcast(false);
	OnFocusChanged.Broadcast(false);
}

void UModioCommonActivatableWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnFocusChangedFast.Broadcast(true);
	OnFocusChanged.Broadcast(true);
}

void UModioCommonActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	if (bAutoFocusOnActivation)
	{
		FocusOnDesiredWidget();
	}
}

#if WITH_EDITOR

void UModioCommonActivatableWidget::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
                                                               class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);
	for (TFieldIterator<FProperty> PropIterator(GetClass()); PropIterator; ++PropIterator)
	{
		FObjectProperty* ObjectProperty = CastField<FObjectProperty>(*PropIterator);
		if (ObjectProperty && ObjectProperty->PropertyClass && ObjectProperty->PropertyClass->IsChildOf<UWidget>())
		{
			if ((ObjectProperty->HasMetaData("BindWidget") || ObjectProperty->HasMetaData("BindWidgetOptional")) &&
				ObjectProperty->HasMetaData("MustImplement"))
			{
				UWidget* Widget = BlueprintWidgetTree.FindWidget(ObjectProperty->GetFName());
				if (Widget &&
					!Widget->GetClass()->ImplementsInterface(ObjectProperty->GetClassMetaData("MustImplement")))
				{
					CompileLog.Error(
						FText::Format(FTextFormat(FText::FromString("{0} does not implement required interface {1}")),
									  FText::FromName(ObjectProperty->GetFName()),
									  FText::FromName(ObjectProperty->GetClassMetaData("MustImplement")->GetFName())));
				}
			}
		}
	}
}
#endif