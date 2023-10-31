/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/Collection/ModioModCollectionTile.h"
#include "UI/Views/Collection/ModioEnableModSwitch.h"

#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "Libraries/ModioSDKLibrary.h"
#include "ModioUI4Subsystem.h"
#include "ModioUISubsystem.h"
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Interfaces/IModioUINotification.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioModCollectionTile::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
	if (!CollectionEntry)
	{
		return;
	}
	MoreOptionsMenu->MenuButton->SetInputHintVisibility(false);
	SubscribeButton->SetInputHintVisibility(false);
	// If the user is not subscribed (i.e. CollectionEntry is a system mod) and the mod state is not installed, hide
	// it.
	EModioModState ModState = CollectionEntry->Underlying.GetModState();
	if (ModState != EModioModState::Installed && CollectionEntry->bCachedSubscriptionStatus == false)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	UModioUI4Subsystem* UE4Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (IsValid(Subsystem))
	{
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);
		Subsystem->RequestLogoDownloadForModID(CollectionEntry->Underlying.GetModProfile().ModId);
	}
	if (ModName)
	{
		ModName->SetText(
			FText::FromString(TruncateLongModName(CollectionEntry->Underlying.GetModProfile().ProfileName, ModName)));
	}

	if (IsValid(UE4Subsystem))
	{
		UE4Subsystem->OnGlobalMouseClick.AddWeakLambda(this, [this]() {
			if (!MoreOptionsMenu->IsHovered())
			{
				MoreOptionsMenu->Close();
			}
		});
	}

	UpdateDiskSize();

	// Perform the initial hide of the remove mod button if we are not subscribed (because we want users to use
	// 'More Options' to do a force uninstall, in that case)
	if (SubscribeButton)
	{
		if (CollectionEntry->bCachedSubscriptionStatus == false)
		{
			SubscribeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			SubscribeButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (SubscriptionIndicator)
	{
		SubscriptionIndicator->SetVisibility(CollectionEntry->bCachedSubscriptionStatus
												 ? ESlateVisibility::HitTestInvisible
												 : ESlateVisibility::Collapsed);
	}
	if (StatusWidget)
	{
		StatusWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StatusWidget->SetDataSource(DataSource);
	}
	if (StatusLine)
	{
		if (Subsystem && Subsystem->GetIsCollectionModDisableUIEnabled())
		{
			bool bIsModEnabled = QueryModEnabled(CollectionEntry->Underlying.GetID());
			StatusLine->SetText(bIsModEnabled ? EnabledStatusText : DisabledStatusText);
		}
		else
		{
			StatusLine->SetText(CollectionEntry->bCachedSubscriptionStatus ? SubscribedStatusText
																		   : InstalledStatusText);
		}
	}
	if (TileButton)
	{
		TileButton->OnClicked.AddUniqueDynamic(this, &UModioModCollectionTile::ShowModDetails);
	}
	if (MoreOptionsMenu)
	{
		FModioUIMenuCommandList MenuEntries;
		FModioUIExecuteAction PositiveRatingDelegate;
		PositiveRatingDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitPositiveRating);
		FModioUIExecuteAction NegativeRatingDelegate;
		NegativeRatingDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitNegativeRating);
		FModioUIExecuteAction ReportDelegate;
		ReportDelegate.BindDynamic(this, &UModioModCollectionTile::SubmitModReport);

		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateUp", "Rate Up")},
									  FModioUIAction {PositiveRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("RateDown", "Rate Down")},
									  FModioUIAction {NegativeRatingDelegate});
		MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("Report", "Report")}, FModioUIAction {ReportDelegate});

		// Force Uninstall should only be available if current user isn't subscribed
		if (CollectionEntry->bCachedSubscriptionStatus == false)
		{
			FModioUIExecuteAction ForceUninstallDelegate;
			ForceUninstallDelegate.BindDynamic(this, &UModioModCollectionTile::ForceUninstall);
			MenuEntries.MappedActions.Add(FModioUIMenuEntry {LOCTEXT("ForceUninstall", "Force Uninstall")},
										  FModioUIAction {ForceUninstallDelegate});
			if (!TileFrame)
			{
				return;
			}

			TileFrame->GetDynamicMaterial()->SetVectorParameterValue(
				"InnerColor", InnerTileErrorColor.ResolveReference().GetSpecifiedColor());
		}
		MoreOptionsMenu->SetMenuEntries(MenuEntries);
	}

	if (EnableModSwitch && Subsystem && Subsystem->GetIsCollectionModDisableUIEnabled())
	{
		Subsystem->OnModEnabledChanged.AddUniqueDynamic(this, &UModioModCollectionTile::OnEnabledStateChanged);
		EnableModSwitch->OnButtonPressed.AddUniqueDynamic(this, &UModioModCollectionTile::OnModEnableButtonPressed);
		bool bIsModEnabled = QueryModEnabled(CollectionEntry->Underlying.GetID());
		EnableModSwitch->InitWithModState(!CollectionEntry->bCachedSubscriptionStatus, bIsModEnabled);
		SetTileFrameColor(bIsModEnabled);
	}
	else if (EnableModSwitch)
	{
		EnableModSwitch->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UModioModCollectionTile::NativeOnSetExpandedState(bool bExpanded)
{
	Super::NativeOnSetExpandedState(bExpanded);

	if (bExpanded) {}
}

void UModioModCollectionTile::OnModEnableButtonPressed()
{
	UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
	if (!CollectionEntry || !EnableModSwitch || !EnableModSwitch->EnableModButton->GetIsEnabled())
	{
		return;
	}

	RequestModEnabledState(CollectionEntry->Underlying.GetID(), !QueryModEnabled(CollectionEntry->Underlying.GetID()));
}

void UModioModCollectionTile::NativeConstruct()
{
	Super::NativeConstruct();
	if (TileBorder)
	{
		TileBorder->ClearHoveredState();
	}
}

void UModioModCollectionTile::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (EnableModSwitch)
	{
		EnableModSwitch->OnButtonHovered();
	}
	MoreOptionsMenu->MenuButton->SetInputHintVisibility(true);
	SubscribeButton->SetInputHintVisibility(true);
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UModioModCollectionTile::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	SubscribeButton->SetInputHintVisibility(false);
	MoreOptionsMenu->MenuButton->SetInputHintVisibility(false);

	if (EnableModSwitch)
	{
		EnableModSwitch->OnButtonUnhovered();
	}
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UModioModCollectionTile::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (EnableModSwitch)
	{
		EnableModSwitch->bUpdateTriggered = true;
		EnableModSwitch->OnButtonUnhovered();
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
	if (TileBorder)
	{
		TileBorder->RenderOnPhase = true;
	}

	if (!bHasFocus && TileFrame)
	{
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
		TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	}
	else
	{
		if (TileFrame)
		{
			TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
			TileFrame->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);

			if (EnableModSwitch && !EnableModSwitch->IsHovered())
			{
				EnableModSwitch->OnButtonHovered();
			}
		}
	}
}

FReply UModioModCollectionTile::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	bHasFocus = true;

	if (EnableModSwitch)
	{
		EnableModSwitch->OnButtonHovered();
	}
	MoreOptionsMenu->MenuButton->SetInputHintVisibility(true);
	SubscribeButton->SetInputHintVisibility(true);
	return FReply::Unhandled();
}

void UModioModCollectionTile::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (EnableModSwitch)
	{
		EnableModSwitch->OnButtonUnhovered();
	}
	MoreOptionsMenu->MenuButton->SetInputHintVisibility(false);
	SubscribeButton->SetInputHintVisibility(false);
	bHasFocus = false;
}

void UModioModCollectionTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ModName)
	{
		UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
		if (!CollectionEntry)
		{
			return;
		}
		ModName->SetText(
			FText::FromString(TruncateLongModName(CollectionEntry->Underlying.GetModProfile().ProfileName, ModName)));
	}
}

void UModioModCollectionTile::OnRatingSubmissionComplete(FModioErrorCode ec, EModioRating Rating)
{
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->DisplayNotificationParams(UModioNotificationParamsLibrary::CreateRatingNotification(ec, DataSource));
	}
}
void UModioModCollectionTile::SubmitNegativeRating()
{
	if (!bIsUserAuthenticated)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowUserAuth();
		}
	}

	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateDownForModId(
				ModInfo->Underlying.GetID(),
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModCollectionTile::OnRatingSubmissionComplete,
														EModioRating::Negative));
		}
	}
}

void UModioModCollectionTile::SubmitPositiveRating()
{
	if (!bIsUserAuthenticated)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowUserAuth();
		}
	}

	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			// Needs additional payload param so we know which type of operation was completed
			Subsystem->RequestRateUpForModId(
				ModInfo->Underlying.GetID(),
				FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioModCollectionTile::OnRatingSubmissionComplete,
														EModioRating::Positive));
		}
	}
}

void UModioModCollectionTile::SubmitModReport()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->ShowModReportDialog(ModInfo);
		}
	}
}

void UModioModCollectionTile::ForceUninstall()
{
	UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
	if (ModInfo)
	{
		if (UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
		{
			if (Subsystem)
			{
				Subsystem->ShowUninstallConfirmationDialog(ModInfo);
			}
		}
	}
}

void UModioModCollectionTile::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// Replace with interface registration call in NativePreConstruct();
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->OnSubscriptionStatusChanged.AddUObject(this,
														  &UModioModCollectionTile::OnModSubscriptionStatusChanged);
	}
	if (TileBorder)
	{
		// TileBorder->SetHoveredState();
	}
}

/// We override this for ModCollectionTile specifically because it wraps a ModCollectionEntry, not a ModInfo
void UModioModCollectionTile::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
															   TOptional<FModioImageWrapper> Image,
															   EModioLogoSize LogoSize)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image, LogoSize);

	if (UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource))
	{
		if (ModID == ModInfo->Underlying.GetID())
		{
			if (ec)
			{
				UE_LOG(LogTemp, Error, TEXT("%s - %d"), *ec.GetErrorMessage(), ec.GetValue());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Error);
			}
			else
			{
				Thumbnail->LoadImageFromFileAsync(Image.GetValue());
				IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
																		   EModioUIAsyncOperationWidgetState::Success);
				UpdateDiskSize();
			}
		}
	}
}

void UModioModCollectionTile::UpdateDiskSize()
{
	if (SizeOnDiskLabel)
	{
		UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (!Subsystem)
		{
			return;
		}

		TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();

		UModioModCollectionEntryUI* CollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);
		if (!CollectionEntry)
		{
			return;
		}

		if (!UserSubscriptions.Contains(CollectionEntry->GetModID_Implementation()))
		{
			return;
		}

		const uint64 NumBytes =
			UserSubscriptions[CollectionEntry->GetModID_Implementation()].GetSizeOnDisk().Underlying;
		SizeOnDiskLabel->SetVisibility(ESlateVisibility::Collapsed);

		if (NumBytes < GB)
		{
			if (NumBytes > 0)
			{
				SizeOnDiskLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				SizeOnDiskLabel->SetText(UModioSDKLibrary::Filesize_ToString(NumBytes, 0, 0));
			}
		}
		else
		{
			SizeOnDiskLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SizeOnDiskLabel->SetText(UModioSDKLibrary::Filesize_ToString(NumBytes, 0, 1));
		}
	}
}

void UModioModCollectionTile::OnModSubscriptionStatusChanged(FModioModID ID, bool Subscribed)
{
	if (DataSource)
	{
		UModioModCollectionEntryUI* Data = Cast<UModioModCollectionEntryUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.GetModProfile().ModId == ID)
			{
				if (SubscribeButton)
				{
					SubscribeButton->SetVisibility(Subscribed ? ESlateVisibility::Visible
															  : ESlateVisibility::Collapsed);
				}
				if (SubscriptionIndicator)
				{
					SubscriptionIndicator->SetVisibility(Subscribed ? ESlateVisibility::HitTestInvisible
																	: ESlateVisibility::Collapsed);
				}

				UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
				if (StatusLine && Subsystem && !Subsystem->GetIsCollectionModDisableUIEnabled())
				{
					StatusLine->SetText(Subscribed ? SubscribedStatusText : InstalledStatusText);
				}
			}
		}
	}
}

void UModioModCollectionTile::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);
	if (bIsSelected)
	{
		if (EnableModSwitch)
		{
			EnableModSwitch->OnButtonHovered();
		}
		MoreOptionsMenu->MenuButton->SetInputHintVisibility(true);
		SubscribeButton->SetInputHintVisibility(true);
		if (!bCurrentExpandedState)
		{
			PlayAnimationForward(FocusTransition);
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
	}
	else
	{
		if (EnableModSwitch)
		{
			EnableModSwitch->OnButtonUnhovered();
		}
		MoreOptionsMenu->MenuButton->SetInputHintVisibility(false);
		SubscribeButton->SetInputHintVisibility(false);
		PlayAnimationReverse(FocusTransition);
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
}

void UModioModCollectionTile::NativeOnEntryReleased()
{
	// PlayAnimationReverse(FocusTransition);
	Super::NativeOnEntryReleased();
}

void UModioModCollectionTile::OnEnabledStateChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	if (UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource))
	{
		if (ModID != ModInfo->Underlying.GetID())
		{
			return;
		}
	}

	EnableModSwitch->SetButtonState(bNewSubscriptionState);
	StatusLine->SetText(bNewSubscriptionState ? EnabledStatusText : DisabledStatusText);
	SetTileFrameColor(bNewSubscriptionState);
}

UModioModInfoUI* UModioModCollectionTile::GetAsModInfoUIObject()
{
	UModioModCollectionEntryUI* ModCollectionEntry = Cast<UModioModCollectionEntryUI>(DataSource);

	// We want to "convert" this UModioModCollectionEntryUI into a UModioModInfoUI with appropriate Underlying mod data
	// so we can pass it around our event system appropriately
	if (ModCollectionEntry)
	{
		UModioModInfoUI* ModInfo = NewObject<UModioModInfoUI>();

		ModInfo->Underlying = ModCollectionEntry->Underlying.GetModProfile();

		return ModInfo;
	}

	return nullptr;
}

void UModioModCollectionTile::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	Super::BuildCommandList(CommandList);

	CommandList->MapAction(
		FModioCommonUICommands::Get().MoreOptions,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioModCollectionTile::NativeMoreOptionsClicked)));
	CommandList->MapAction(
		FModioCommonUICommands::Get().Confirm,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioModCollectionTile::NativeCollectionTileClicked)));
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (EnableModSwitch && Subsystem && Subsystem->GetIsCollectionModDisableUIEnabled())
	{
		CommandList->MapAction(
			FModioCommonUICommands::Get().EnableDisableMod,
			FUIAction(FExecuteAction::CreateUObject(this, &UModioModCollectionTile::OnModEnableButtonPressed)));
	}
}

void UModioModCollectionTile::NativeMoreOptionsClicked()
{
	if (MoreOptionsMenu->GetIsMenuOpen())
	{
		SetFocus();
		return;
	}
	if (MoreOptionsMenu)
	{
		MoreOptionsMenu->ToggleOpen(true);
	}
}

FReply UModioModCollectionTile::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
															   const FPointerEvent& InMouseEvent)
{
	if (MoreOptionsMenu->GetIsMenuOpen() && !IsHovered())
	{
		MoreOptionsMenu->Close();
	}
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UModioModCollectionTile::SetTileFrameColor(bool bEnabledState)
{
	if (!TileFrame)
	{
		return;
	}

	TileFrame->GetDynamicMaterial()->SetVectorParameterValue(
		"InnerColor", bEnabledState ? InnerTileEnabledColor.ResolveReference().GetSpecifiedColor()
									: InnerTileDisabledColor.ResolveReference().GetSpecifiedColor());
}

void UModioModCollectionTile::NativeCollectionTileClicked()
{
	if (MoreOptionsMenu->GetIsMenuOpen())
	{
		MoreOptionsMenu->SelectCurrentMenuItem();
		return;
	}

	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		UModioModCollectionEntryUI* ModInfo = Cast<UModioModCollectionEntryUI>(DataSource);
		if (IsValid(ModInfo))
		{
			Subsystem->ShowDetailsForMod(ModInfo->Underlying.GetID());
			FSlateApplication::Get().PlaySound(PressedSound);
		}
	}
}

void UModioModCollectionTile::ShowModDetails()
{
	if (UModioUI4Subsystem* Subsystem4 = GEngine->GetEngineSubsystem<UModioUI4Subsystem>())
	{
		if (MoreOptionsMenu->GetIsMenuOpen())
		{
			Subsystem4->SetCurrentFocusTarget(nullptr);
			MoreOptionsMenu->Close();
			return;
		}

		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (UModioModInfoUI* ModInfo = GetAsModInfoUIObject())
			{
				Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);

				FSlateApplication::Get().PlaySound(PressedSound);
				return;
			}
		}
	}
}

void UModioModCollectionTile::NativeSubscribeClicked()
{
	if (!GetSubscriptionState())
	{
		return;
	}

	Super::NativeSubscribeClicked();
}

#include "Loc/EndModioLocNamespace.h"