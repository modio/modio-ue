/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioUI4Subsystem.h"
#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "Core/Input/ModioInputKeys.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioDownloadListWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/CommonComponents/ModioUserProfileButton.h"
#include "UI/Drawers/DownloadQueue/ModioDownloadQueueOpProgress.h"
#include "UI/Drawers/DownloadQueue/ModioDownloadQueueEntry.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioUIDownloadQueueWidget.h"

#include "ModioDownloadQueueDrawer.generated.h"

/**
* Modio UI element that represents a download queue, which displays 
* the current mods being processed
**/
UCLASS()
class MODIOUI_API UModioDownloadQueueDrawer : public UModioUserWidgetBase,
											  public IModioUIUserChangedReceiver,
											  public IModioUIDownloadQueueWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override
	{
		IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
		IModioUIDownloadQueueWidget::Execute_DisplayUserInfo(this, FModioOptionalUser {NewUser});
	}

	virtual void NativeOnInitialized()
	{
		bIsFocusable = true;
		IModioUIUserChangedReceiver::Register<UModioDownloadQueueDrawer>();
		if (LogOutButton)
		{
			LogOutButton->OnClicked.AddDynamic(this, &UModioDownloadQueueDrawer::OnLogoutClicked);
		}

		if (CurrentOpProgress)
		{
			CurrentOpProgress->SetVisibility(ESlateVisibility::Collapsed);
			CurrentOpProgress->OperationCompletedDelegate().BindDynamic(
				this, &UModioDownloadQueueDrawer::HandleOperationCompleted);
		}

		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (IsValid(Subsystem))
		{
			Subsystem->OnSubscriptionStatusChanged.AddUObject(
				this, &UModioDownloadQueueDrawer::OnSubsricptionChanged);
		}
		UModioUserWidgetBase::NativeOnInitialized();
	}

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
		if (CurrentOpProgress)
		{
			UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
			switch (CurrentOpProgress->CurrentStatus)
			{
				case EModioModManagementEventType::BeginInstall:
				case EModioModManagementEventType::BeginUninstall:
				case EModioModManagementEventType::BeginUpdate:
				case EModioModManagementEventType::BeginUpload:
					if (Subsystem)
					{
						if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
						{
							Execute_RefreshDownloadQueue(this);
						}
					}
					break;
				default:
					break;
			}
		}
	}

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override
	{
		UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

		if (Subsystem && (Subsystem->IsAnyDialogOpen() || !Subsystem->IsDownloadDrawerOpen()))
		{
			return FReply::Unhandled();
		}

		ConstructNavigationPath();
		if (CurrentNavIndex >= NavigationPath.Num())
		{
			CurrentNavIndex = 0;
		}
		
		if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Down) {

			if (NavigationPath.IsValidIndex(CurrentNavIndex + 1)) {

				CurrentNavIndex += 1;
			}
			if (NavigationPath.IsValidIndex(CurrentNavIndex))
			{
				NavigationPath[CurrentNavIndex]->SetFocus();
			}
			return FReply::Handled();
		}
		if (GetCommandKeyForEvent(InKeyEvent) == FModioInputKeys::Up)
		{
			if (NavigationPath.IsValidIndex(CurrentNavIndex - 1))
			{
				CurrentNavIndex -= 1;
			}
			if (NavigationPath.IsValidIndex(CurrentNavIndex))
			{
				NavigationPath[CurrentNavIndex]->SetFocus();
			}
			return FReply::Handled();
		}
		return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
	}

	void ConstructNavigationPath()
	{
		NavigationPath.Empty();
		NavigationPath.Add(LogOutButton);
		if (CurrentOpProgress->GetVisibility() != ESlateVisibility::Collapsed)
		{
			NavigationPath.Add(CurrentOpProgress->UnsubscribeButton);
		}
		for (auto& queueItems : OperationQueue->QueueList->GetDisplayedEntryWidgets())
		{
			UModioDownloadQueueEntry* queueEntry = Cast<UModioDownloadQueueEntry>(queueItems);
			if (!queueEntry)
			{
				continue;
			}
			NavigationPath.Add(queueEntry);
		}
		for (auto& completedItems : CompletedQueue->QueueList->GetDisplayedEntryWidgets())
		{
			UModioDownloadQueueEntry* completedEntry = Cast<UModioDownloadQueueEntry>(completedItems);
			if (!completedEntry)
			{
				continue;
			}
			NavigationPath.Add(completedEntry);
		}
	}

	UFUNCTION()
	void OnLogoutClicked()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->ShowLogoutDialog();
		}
	}

	UFUNCTION()
	void HandleOperationCompleted()
	{
		RefreshOperationQueue();
		Execute_RefreshDownloadQueue(this);
	}

	UFUNCTION()
	void OnSubsricptionChanged(FModioModID ID, bool Subscribed)
	{
		UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

		if (Subsystem && (Subsystem->IsAnyDialogOpen() || !Subsystem->IsDownloadDrawerOpen()))
		{
			return;
		}

		if (LogOutButton)
		{
			LogOutButton->SetKeyboardFocus();
		}
	}

	TArray<UModioModInfoUI*> PendingDownloads;
	TArray<UModioModInfoUI*> CompletedDownloads;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioUserProfileButton* ProfileIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* UserLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* ActivityText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* StatusText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* LogOutButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioDownloadQueueOpProgress* CurrentOpProgress;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioDownloadListWidgetBase* OperationQueue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioDownloadListWidgetBase* CompletedQueue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText DownloadingActionText = FText::FromString("Downloading...");
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText ExtractingActionText = FText::FromString("Extracting...");
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText InitializingActionText = FText::FromString("Initializing...");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Localization")
	FText NoDownloadText = FText::FromString("There is no mod being downloaded");

	TArray<UWidget*> NavigationPath;
	int CurrentNavIndex = 0;
	/// @brief Shows the newly updated user as the profile badge, name etc
	/// @param NewUser The new user to display, will be empty if no user is yet logged in
	virtual void NativeDisplayUserInfo(FModioOptionalUser NewUser) override
	{
		if (NewUser.Internal.IsSet())
		{
			UserLabel->SetText(FText::FromString(NewUser.Internal->Username));
		}
		else
		{
			// Set the label text to empty or hide it or whatever
		}
		if (ProfileIcon)
		{
			// This should no longer be necessary as the profile icon is itself listening for user changed events.
			// Direct call here stinks. Refactor to use the interface later.
			// ProfileIcon->NativeUserChanged(NewUser.Internal);
		}
	}

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override 
	{
		UModioUI4Subsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();

		RefreshOperationQueue();
		ConstructNavigationPath();
		CurrentNavIndex = 0;

		if (NavigationPath.IsValidIndex(CurrentNavIndex) && IsValid(UISubsystem) && !(UISubsystem->GetLastInputDevice() == EModioUIInputMode::Mouse))
		{
			NavigationPath[CurrentNavIndex]->SetFocus();
		}
		return FReply::Handled();
	}

	/// @brief Notifies the widget to fetch the current op and any queued ops for display, is this necessary?
public:
	virtual void NativeRefreshDownloadQueue() override
	{
		UModioUI4Subsystem* UI4Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
		
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();

			// Early out if we do not find any subscriptions
			if (UserSubscriptions.Num() <= 0) {
				return;
			}

			if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
			{
				if (CurrentOpProgress)
				{
					if (UModioModInfoUI* NewModInfoWrapper = NewObject<UModioModInfoUI>())
					{
						NewModInfoWrapper->Underlying = UserSubscriptions[CurrentProgress->ID].GetModProfile();
						CurrentOpProgress->SetDataSource(NewModInfoWrapper);
						CurrentOpProgress->SetVisibility(ESlateVisibility::Visible);
						CurrentOpProgress->UnsubscribeButton->IsFocusable = true;
					}
				}
				if (StatusText)
				{
					StatusText->SetVisibility(ESlateVisibility::Collapsed);
				}
				if (ActivityText)
				{
					switch (CurrentProgress->GetCurrentState())
					{
						case EModioModProgressState::Downloading:
							ActivityText->SetText(DownloadingActionText);
							break;
						case EModioModProgressState::Extracting:
							ActivityText->SetText(ExtractingActionText);
							break;
						case EModioModProgressState::Initializing:
							ActivityText->SetText(InitializingActionText);
							break;
						default:
							ActivityText->SetText(DownloadingActionText);
							
					}
					CurrentOpProgress->UpdateProgress(CurrentProgress.GetValue());
				}
			}
			else
			{
				if (ActivityText)
				{
					ActivityText->SetText(DownloadingActionText);
				}
				if (StatusText)
				{
					StatusText->SetVisibility(ESlateVisibility::Visible);
					StatusText->SetText(NoDownloadText);
				}
				if (CurrentOpProgress)
				{
					CurrentOpProgress->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

		}
	}

	void RefreshOperationQueue()
	{
		UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (!Subsystem)
		{
			return;
		}
		
		TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();

		// Early out if we do not find any subscriptions
		if (UserSubscriptions.Num() == 0)
		{
			return;
		}
		

		if (OperationQueue)
		{
			// Rebuild the Pending Downloads list
			PendingDownloads.Empty();
			TArray<FModioModCollectionEntry> Entries;
			UserSubscriptions.GenerateValueArray(Entries);
			Algo::TransformIf(
				Entries, PendingDownloads,
				[](const FModioModCollectionEntry& Entry) {
					// Filter all mods that are either installed, have uninstallations pending
					// or currently being downloaded
					return ((Entry.GetModState() != EModioModState::Installed) &&
							(Entry.GetModState() != EModioModState::UninstallPending) &&
							(Entry.GetModState() != EModioModState::Downloading) &&
							(Entry.GetModState() != EModioModState::Extracting));
				},
				[](FModioModCollectionEntry Entry) {
					UModioModInfoUI* EntryWrapper = NewObject<UModioModInfoUI>();
					if (EntryWrapper)
					{
						EntryWrapper->Underlying = Entry.GetModProfile();
					}
					return EntryWrapper;
				});

			OperationQueue->UpdateListItems(PendingDownloads);

			// Rebuild the Completed Downloads list
			CompletedDownloads.Empty();

			TArray<FModioModCollectionEntry> CompletedDownloadEntries;
			UserSubscriptions.GenerateValueArray(Entries);
			Algo::TransformIf(
				Entries, CompletedDownloads,
				[](const FModioModCollectionEntry& Entry) {
					// Filter all mods that were installed this session
					if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
					{
						return ((Entry.GetModState() == EModioModState::Installed) &&
								UISubsystem->ModsDownloadedThisSession.Contains(Entry.GetID()));
					}
					return false;
				},
				[](FModioModCollectionEntry Entry) {
					UModioModInfoUI* EntryWrapper = NewObject<UModioModInfoUI>();
					if (EntryWrapper)
					{
						EntryWrapper->Underlying = Entry.GetModProfile();
					}
					return EntryWrapper;
				});
			CompletedQueue->UpdateListItems(CompletedDownloads);
		}
	}
};
