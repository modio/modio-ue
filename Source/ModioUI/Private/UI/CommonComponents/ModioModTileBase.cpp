/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioModTileBase.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Logging/LogMacros.h"
#include "ModioUISubsystem.h"
#include "Rendering/DrawElements.h"
#include "Fonts/FontMeasure.h"
#include "TimerManager.h"
#include "UI/Commands/ModioCommonUICommands.h"

void UModioModTileBase::NativeConstruct()
{
	Super::NativeConstruct();
	UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	bIsFocusable = true;
	IModioUIMediaDownloadCompletedReceiver::Register<UModioModTileBase>(EModioUIMediaDownloadEventType::ModLogo);
	IModioUIAuthenticationChangedReceiver::Register<UModioModTileBase>();
}

UModioModInfoUI* UModioModTileBase::GetAsModInfoUIObject()
{
	if (DataSource)
	{
		return Cast<UModioModInfoUI>(DataSource);
	}

	return nullptr;
}

void UModioModTileBase::NativeOnAuthenticationChanged(TOptional<FModioUser> User)
{
	bIsUserAuthenticated = User.IsSet();
}

bool UModioModTileBase::GetSubscriptionState()
{
	UModioModInfoUI* BoundModInfo = GetAsModInfoUIObject();
	if (BoundModInfo)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			return Subsystem->QueryUserSubscriptions().Contains(BoundModInfo->Underlying.ModId);
		}
	}
	return false;
}

void UModioModTileBase::EnableSubscribeButton()
{
	SubscribeButton->SetIsEnabled(true);
}

void UModioModTileBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SubscribeButton)
	{
		SubscribeButton->OnClicked.AddDynamic(this, &UModioModTileBase::NativeSubscribeClicked);
	}

	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->OnSubscriptionStatusChanged.AddUObject(this, &UModioModTileBase::OnModSubscriptionStatusChanged);
	}
	FOnSetOperationState HandlerDelegate;
	HandlerDelegate.BindDynamic(this, &UModioModTileBase::HandleModLogoOperationStateChanged);
	IModioUIAsyncOperationWidget::Execute_SetOperationStateDelegate(this, HandlerDelegate);
	if (TileBorder)
	{
		TileBorder->RenderOnPhase = false;
	}

	const FModioModTileStyle* ModTileStyle = Style.FindStyle<FModioModTileStyle>();
	if (ModTileStyle) 
	{
		HoveredSound = ModTileStyle->HoveredSound;
		PressedSound = ModTileStyle->PressedSound;
	}

}

void UModioModTileBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();

	UModioModInfoUI* ModInfo = GetAsModInfoUIObject();
	if (ModInfo)
	{
		if (SubscribeButton)
		{
			SubscribeButton->SetLabel(SubscribeLabel);

			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				// Can hit this function directly because the primary subsystem caches the subscriptions, so we dont
				// have to do it manually in the UI subsystem
				if (Subsystem->QueryUserSubscriptions().Contains(ModInfo->Underlying.ModId))
				{
					SubscribeButton->SetLabel(UnsubscribeLabel);
				}
			}
		}
	}
}

void UModioModTileBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
	if (TileBorder)
	{
		TileBorder->RenderOnPhase = true;
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	}
}

void UModioModTileBase::NativeOnEntryReleased()
{
	IUserListEntry::NativeOnEntryReleased();
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
		TileBorder->DisableBorder();
		TileBorder->RenderOnPhase = false;
	}
}

void UModioModTileBase::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
														 TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image);
	UModioModInfoUI* ModInfo = GetAsModInfoUIObject();
	if (ModInfo)
	{
		if (ModID == ModInfo->Underlying.ModId)
		{
			CurrentModId = ModID;
			if (ec)
			{
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Error);
				UE_LOG(LogTemp, Error, TEXT("%s - %d"), *ec.GetErrorMessage(), ec.GetValue());
			}
			else
			{
				Thumbnail->LoadImageFromFileAsync(Image.GetValue());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Success);
			}
		}
	}
}

FReply UModioModTileBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Subscribe && !InKeyEvent.IsRepeat() && SubscribeButton && SubscribeButton->GetIsEnabled())
	{
		NativeSubscribeClicked();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UModioModTileBase::SynchronizeProperties()
{
	SetExpandedState(bCurrentExpandedState);
	SetClipping(EWidgetClipping::ClipToBoundsWithoutIntersecting);
	Super::SynchronizeProperties();
}

void UModioModTileBase::NativeOnSetExpandedState(bool bExpanded)
{
	bRoutedOnSetExpandedState = true;
	bCurrentExpandedState = bExpanded;
	OnSetExpandedState(bExpanded);
}

void UModioModTileBase::HandleModLogoOperationStateChanged(EModioUIAsyncOperationWidgetState NewState)
{
	if (Thumbnail)
	{
		switch (NewState)
		{
			case EModioUIAsyncOperationWidgetState::Error:
				if (UMaterialInterface* ReferencedErrorMaterial = ErrorMaterial.ResolveReference())
				{
					Thumbnail->SetBrushFromMaterial(ReferencedErrorMaterial);
				}
				break;
			case EModioUIAsyncOperationWidgetState::InProgress:
				if (UMaterialInterface* ReferencedLoadingMaterial = LoadingMaterial.ResolveReference())
				{
					Thumbnail->SetBrushFromMaterial(ReferencedLoadingMaterial);
				}
				break;
			case EModioUIAsyncOperationWidgetState::Success:
				// No need to do anything, the normal callback for the image fetch will handle
				break;
		}
	}
}

int32 UModioModTileBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
									 const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
									 int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							  bParentEnabled);
	/*TSharedPtr<SWidget> PinnedWidget = MyWidget.Pin();
	return PinnedWidget->Paint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							  bParentEnabled);*/
}

void UModioModTileBase::SetExpandedState(bool bExpanded)
{
	bRoutedOnSetExpandedState = false;
	NativeOnSetExpandedState(bExpanded);
	checkf(bRoutedOnSetExpandedState, TEXT("Overridden implementations of NativeOnSetExpandedState should call "
										   "Super::NativeOnSetExpandedState to route events to blueprint"));
}

void UModioModTileBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{	
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (TileBorder)
	{
		TileBorder->SetHoveredState();
		TileBorder->EnableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);
	}
	PlayAnimation(FocusTransition);
}

FReply UModioModTileBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// TODO: @modio-ue4 should match our key mappings stuff
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			UModioModInfoUI* ModInfo = GetAsModInfoUIObject();
			if (ModInfo)
			{
				Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);
				FSlateApplication::Get().PlaySound(PressedSound);
				return FReply::Handled();
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UModioModTileBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
		TileBorder->DisableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	}

	PlayAnimationReverse(FocusTransition);
}

void UModioModTileBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	if (UISubsystem) 
	{
		UISubsystem->SetCurrentFocusTarget(this);
	}
	if (TileBorder)
	{
		TileBorder->SetHoveredState();
		TileBorder->EnableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);
	}
	PlayAnimation(FocusTransition);
}

void UModioModTileBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
		TileBorder->DisableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	}
	PlayAnimationReverse(FocusTransition);
}

FReply UModioModTileBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	// return FReply::Handled();
	if (TileBorder)
	{
		TileBorder->SetHoveredState();
		TileBorder->EnableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);
	}

	FSlateApplication::Get().PlaySound(HoveredSound);
	PlayAnimation(FocusTransition);
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UModioModTileBase::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
		TileBorder->DisableBorder();
	}
	if (TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	}
}

void UModioModTileBase::OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed)
{
	if (DataSource)
	{
		UModioModInfoUI* Data = GetAsModInfoUIObject();
		if (Data)
		{
			if (Data->Underlying.ModId == ID)
			{
				if (SubscribeButton && !Subscribed)
				{
					SubscribeButton->SetLabel(SubscribeLabel);
					GWorld->GetTimerManager().SetTimer(SetFocusTimerHandle, this,
													   &UModioModTileBase::EnableSubscribeButton, 0.6, false);
				}
				else if (SubscribeButton && Subscribed)
				{
					SubscribeButton->SetLabel(UnsubscribeLabel);
					GWorld->GetTimerManager().SetTimer(SetFocusTimerHandle, this,
													   &UModioModTileBase::EnableSubscribeButton, 0.6, false);
				}
				if (SubscriptionIndicator)
				{
					SubscriptionIndicator->SetVisibility(Subscribed ? ESlateVisibility::HitTestInvisible
																	: ESlateVisibility::Collapsed);
				}
			}
		}
	}
}

void UModioModTileBase::NativeSubscribeClicked()
{
	UModioModInfoUI* Data = GetAsModInfoUIObject();

	if (CurrentModId == Data->Underlying.ModId)
	{
		SubscribeButton->SetIsEnabled(false);
	}

	GWorld->GetTimerManager().SetTimer(SetFocusTimerHandle, this, &UModioModTileBase::EnableSubscribeButton, 0.6, false);

	if (!bIsUserAuthenticated)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowUserAuthenticationDialog();
		}
	}

	if (!GetSubscriptionState())
	{
		if (Data)
		{
			OnSubscribeClicked.Broadcast(Data);
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->RequestSubscriptionForModID(Data->Underlying.ModId);
			}
		}
	}
	else
	{
		if (Data)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->RequestRemoveSubscriptionForModID(Data->Underlying.ModId);
			}
		}
	}
}

void UModioModTileBase::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	CommandList->MapAction(FModioCommonUICommands::Get().Confirm,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioModTileBase::NativeTileClicked)));
}

void UModioModTileBase::NativeTileClicked()
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
		{
			Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);
			FSlateApplication::Get().PlaySound(PressedSound);
		}
	}
}