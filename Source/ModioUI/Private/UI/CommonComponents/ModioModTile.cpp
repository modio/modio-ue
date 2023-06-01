/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioModTile.h"
#include "Components/ListViewBase.h"
#include "Engine/Engine.h"
#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Interfaces/IModioUINotification.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioModTile::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (!DataSource)
	{
		return;
	}

	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																	   EModioUIAsyncOperationWidgetState::InProgress);
			Subsystem->RequestLogoDownloadForModID(ModInfo->Underlying.ModId);
		}

		if (SubscriptionIndicator)
		{
			SubscriptionIndicator->SetDataSource(DataSource);
		}
	}
}

void UModioModTile::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected)
	{
		if (!bCurrentExpandedState)
		{
			// PlayAnimationForward(FocusTransition);
		}
	}
	else
	{
		// PlayAnimationReverse(FocusTransition);
	}

	// SetRenderScale(bIsSelected ? FVector2D(1.25) : FVector2D(1));
	// SetRenderTransformPivot(FVector2D(1.25f / 2));
	// SetPadding(bIsSelected? -100: 8);
}

void UModioModTile::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();
	// PlayAnimationReverse(FocusTransition);
}


void UModioModTile::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!AllowMouseHoverFocus())
	{
		return;
	}

	Super::NativeOnMouseEnter(MyGeometry, MouseEvent);
}

void UModioModTile::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!AllowMouseHoverFocus())
	{
		return;
	}

	Super::NativeOnMouseLeave(InMouseEvent);
}

bool UModioModTile::AllowMouseHoverFocus() 
{
	UModioModTile* target = Cast<UModioModTile>(UISubsystem->GetCurrentFocusTarget());
	if (!IsValid(target)) 
	{
		return true;
	}
	return !target->MoreOptionsMenu->GetIsMenuOpen();
}

void UModioModTile::NativeOnSetExpandedState(bool bExpandedState)
{
	bool bOldExpandedState = bCurrentExpandedState;

	Super::NativeOnSetExpandedState(bExpandedState);
	SubscribeButton->SetVisibility(bExpandedState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	MoreOptionsMenu->SetVisibility(bExpandedState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	/*if (SizeOverride)
	{
		if (!bExpandedState)
		{
			SizeOverride->ClearWidthOverride();
			SizeOverride->ClearHeightOverride();
		}
	}*/
}

FReply UModioModTile::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!AllowMouseHoverFocus())
	{
		UISubsystem->SetCurrentFocusTarget(nullptr);
		return FReply::Handled();
	}

	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return OnThumbnailMouseDown(InGeometry, InMouseEvent).NativeReply;
}

void UModioModTile::OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayErrorNotification(UModioNotificationParamsLibrary::CreateRatingNotification(ec, DataSource));
	}
}

void UModioModTile::SubmitNegativeRating()
{
	if (!bIsUserAuthenticated)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowUserAuthenticationDialog();
		}
	}

	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateDownForModId(
				ModInfo->Underlying.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModTile::OnRatingSubmissionComplete,
														EModioRating::Negative));
		}
	}
}

void UModioModTile::SubmitPositiveRating()
{
	if (!bIsUserAuthenticated)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowUserAuthenticationDialog();
		}
	}

	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateUpForModId(
				ModInfo->Underlying.ModId,
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModTile::OnRatingSubmissionComplete,
														EModioRating::Positive));
		}
	}
}

void UModioModTile::SubmitModReport()
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowModReportDialog(ModInfo);
		}
	}
}

void UModioModTile::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MoreOptionsMenu)
	{
		FModioUIMenuCommandList MenuEntries;
		FModioUIExecuteAction PositiveRatingDelegate;
		PositiveRatingDelegate.BindDynamic(this, &UModioModTile::SubmitPositiveRating);
		FModioUIExecuteAction NegativeRatingDelegate;
		NegativeRatingDelegate.BindDynamic(this, &UModioModTile::SubmitNegativeRating);
		FModioUIExecuteAction ReportDelegate;
		ReportDelegate.BindDynamic(this, &UModioModTile::SubmitModReport);
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateUp", "Rate Up")},
									  FModioUIAction {PositiveRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateDown", "Rate Down")},
									  FModioUIAction {NegativeRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("Report", "Report")}, FModioUIAction {ReportDelegate});

		MoreOptionsMenu->SetMenuEntries(MenuEntries);
	}
	if (Thumbnail)
	{
		Thumbnail->OnMouseButtonDownEvent.BindDynamic(this, &UModioModTile::OnThumbnailMouseDown);
	}

	if (SubscriptionIndicator)
	{
		SubscriptionIndicator->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FEventReply UModioModTile::OnThumbnailMouseDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsTouchEvent())
	{
	//	return;
	}
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (!AllowMouseHoverFocus() || MoreOptionsMenu->GetIsMenuOpen()) 
			{
				Subsystem->SetCurrentFocusTarget(nullptr);
				MoreOptionsMenu->Close();
				return FEventReply(true);
			}

			if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
			{
				Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);

				FSlateApplication::Get().PlaySound(PressedSound);

				return FEventReply(true);
			}
		}
	}
	return FEventReply(false);
}

void UModioModTile::NativeMoreOptionsClicked()
{
	if (MoreOptionsMenu)
	{
		UISubsystem->SetCurrentFocusTarget(this);
		MoreOptionsMenu->ToggleOpen(true);
	}
}

void UModioModTile::NativeReportClicked()
{
	UModioModInfoUI* Data = Cast<UModioModInfoUI>(DataSource);
	if (Data)
	{
		OnReportClicked.Broadcast(Data);
	}
}

FReply UModioModTile::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MoreOptionsMenu->GetIsMenuOpen() && !IsHovered()) 
	{
		MoreOptionsMenu->Close();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UModioModTile::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	Super::BuildCommandList(CommandList);

	CommandList->MapAction(FModioCommonUICommands::Get().MoreOptions,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioModTile::NativeMoreOptionsClicked)));
}

void UModioModTile::NativeTileClicked() 
{
	if (MoreOptionsMenu->GetIsMenuOpen()) 
	{
		MoreOptionsMenu->SelectCurrentMenuItem();
		return;
	}

	Super::NativeTileClicked();
}

void UModioModTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	bool bIsSelected = IUserObjectListEntry::IsListItemSelected();
	bool bFocusOrFocusedDescendents = (HasAnyUserFocus() || HasFocusedDescendants());
	bool bDisplayButtons = bIsSelected && bFocusOrFocusedDescendents;

	if (TileActiveFrame)
	{
		// Keep the non visible option as hidden, collapsed causes SRetainerWidget size 0 errors
		ESlateVisibility FrameVisibility = bFocusOrFocusedDescendents && TileActiveFrame->GetRenderOpacity() >= 0.1f
											   ? ESlateVisibility::Visible
											   : ESlateVisibility::Hidden;
		SubscribeButton->SetVisibility(FrameVisibility);
		MoreOptionsMenu->SetVisibility(FrameVisibility);
	}
	Invalidate(EInvalidateWidgetReason::LayoutAndVolatility);

	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);
	if (ModName && IsValid(ModInfo))
	{
		FString modName = ModInfo->Underlying.ProfileName;
		modName = TruncateLongModName(modName, ModName, truncateDivider);
		ModName->SetText(FText::FromString(modName));
	}
}

void UModioModTile::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	MoreOptionsMenu->Close();
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	
}

void UModioModTile::SetSizeOverride(FVector2D NewSize)
{
	/*
		SizeOverride->SetWidthOverride(NewSize.X);
		SizeOverride->SetHeightOverride(NewSize.Y);*/
}

#include "Loc/EndModioLocNamespace.h"