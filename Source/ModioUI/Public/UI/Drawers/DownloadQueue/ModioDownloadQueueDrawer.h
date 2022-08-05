#pragma once

#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/CommonComponents/ModioUserProfileButton.h"
#include "UI/Drawers/DownloadQueue/ModioDownloadQueueOpProgress.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioUIDownloadQueueWidget.h"

#include "ModioDownloadQueueDrawer.generated.h"

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
		IModioUIUserChangedReceiver::Register<UModioDownloadQueueDrawer>();
		if (LogOutButton)
		{
			LogOutButton->OnClicked.AddDynamic(this, &UModioDownloadQueueDrawer::OnLogoutClicked);
		}
		if (CurrentOpProgress)
		{
			CurrentOpProgress->OperationCompletedDelegate().BindDynamic(
				this, &UModioDownloadQueueDrawer::HandleOperationCompleted);
		}
		UModioUserWidgetBase::NativeOnInitialized();
		bIsFocusable = true;
	}
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
		// We might want to move this into the actual Download progress widget itself?
		if (PendingDownloads.Num() == 0)
		{
			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
				{
					Execute_RefreshDownloadQueue(this);
				}
			}
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
		Execute_RefreshDownloadQueue(this);
	}

	TArray<UModioModInfoUI*> PendingDownloads;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioUserProfileButton* ProfileIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* UserLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* ActivityText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* LogOutButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioDownloadQueueOpProgress* CurrentOpProgress;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextButton* UnsubscribeButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* OperationQueue;

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
	/// @brief Notifies the widget to fetch the current op and any queued ops for display, is this necessary?
	virtual void NativeRefreshDownloadQueue() override
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();

			if (TOptional<FModioModProgressInfo> CurrentProgress = Subsystem->QueryCurrentModUpdate())
			{
				if (CurrentOpProgress)
				{
					if (UModioModInfoUI* NewModInfoWrapper = NewObject<UModioModInfoUI>())
					{
						NewModInfoWrapper->Underlying = UserSubscriptions[CurrentProgress->ID].GetModProfile();
						CurrentOpProgress->SetDataSource(NewModInfoWrapper);
						// CurrentOpProgress->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					}
				}
				if (ActivityText)
				{
					if (CurrentProgress->CurrentlyDownloadedBytes < CurrentProgress->TotalDownloadSize) {}
					if (CurrentProgress->CurrentlyExtractedBytes < CurrentProgress->TotalExtractedSizeOnDisk) {}
				}
			}
			else
			{
				if (CurrentOpProgress)
				{
					CurrentOpProgress->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (UnsubscribeButton)
			{
				UnsubscribeButton->SetVisibility(CurrentOpProgress->GetVisibility());
			}
			if (OperationQueue)
			{
				PendingDownloads.Empty();
				TArray<FModioModCollectionEntry> Entries;
				UserSubscriptions.GenerateValueArray(Entries);
				Algo::TransformIf(
					Entries, PendingDownloads,
					[](const FModioModCollectionEntry& Entry) {
						// Filter all mods that are either installed or have uninstallations pending
						return ((Entry.GetModState() != EModioModState::Installed) &&
								(Entry.GetModState() != EModioModState::UninstallPending));
					},
					[](FModioModCollectionEntry Entry) {
						UModioModInfoUI* EntryWrapper = NewObject<UModioModInfoUI>();
						if (EntryWrapper)
						{
							EntryWrapper->Underlying = Entry.GetModProfile();
						}
						return EntryWrapper;
					});
				OperationQueue->SetListItems(PendingDownloads);
				OperationQueue->RequestRefresh();
			}
		}
	}
};
