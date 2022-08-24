// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Drawers/DownloadQueue/ModioDownloadQueueEntry.h"
#include "Core/ModioModInfoUI.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioDownloadQueueEntryStyle.h"

void UModioDownloadQueueEntry::NativeOnSetDataSource()
{
	UModioUserWidgetBase::NativeOnSetDataSource();
	UModioModInfoUI* ActualData = Cast<UModioModInfoUI>(DataSource);
	if (ActualData)
	{
		if (ModNameLabel)
		{
			ModNameLabel->SetText(FText::FromString(ActualData->Underlying.ProfileName));
		}
		if (ModSizeLabel)
		{
			ModSizeLabel->SetText(UModioSDKLibrary::Filesize_ToString(ActualData->Underlying.FileInfo.Filesize));
		}
		if (ModStatusLabel)
		{
			ModStatusLabel->SetVisibility(ESlateVisibility::Hidden);
		}
		if (ModThumbnail)
		{
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				Subsystem->RequestLogoDownloadForModID(ActualData->Underlying.ModId);
			}
		}
	}
}

void UModioDownloadQueueEntry::OnUnsubClicked()
{
	UModioModInfoUI* ActualData = Cast<UModioModInfoUI>(DataSource);
	if (ActualData)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->ShowModUnsubscribeDialog(ActualData);
		}
	}
}

void UModioDownloadQueueEntry::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (const FModioDownloadQueueEntryStyle* ResolvedStyle = EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
	{
		if (UnsubscribeButton)
		{
			if (const FModioButtonStyle* ResolvedButtonStyle =
					ResolvedStyle->UnsubscribeButtonStyle.FindStyle<FModioButtonStyle>())
			{
				UnsubscribeButton->SetButtonStyle(ResolvedStyle->UnsubscribeButtonStyle);
			}
		}
		if (ModNameLabel)
		{
			ModNameLabel->GetStyleDelegate().BindUObject(this, &UModioDownloadQueueEntry::GetRichTextStyle);
		}
		if (ModStatusLabel)
		{
			ModStatusLabel->GetStyleDelegate().BindUObject(this, &UModioDownloadQueueEntry::GetRichTextStyle);
		}
		if (ModSizeLabel)
		{
			ModSizeLabel->GetStyleDelegate().BindUObject(this, &UModioDownloadQueueEntry::GetRichTextStyle);
		}
	}
}

void UModioDownloadQueueEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (UnsubscribeButton)
	{
		UnsubscribeButton->OnClicked.AddDynamic(this, &UModioDownloadQueueEntry::OnUnsubClicked);
	}
	IModioUIMediaDownloadCompletedReceiver::Register<UModioDownloadQueueEntry>(EModioUIMediaDownloadEventType::ModLogo);
	bIsFocusable = true;
}

void UModioDownloadQueueEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
}

void UModioDownloadQueueEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (EntryBorder)
	{
		if (bIsSelected)
		{
			if (const FModioDownloadQueueEntryStyle* ResolvedStyle =
					EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
			{
				EntryBorder->SetBrush(ResolvedStyle->HighlightedBorderBrush);
			}
		}
		else
		{
			EntryBorder->SetBrush(FSlateNoResource());
		}
	}
}

FReply UModioDownloadQueueEntry::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply OriginalReply = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	if (UnsubscribeButton)
	{
		UnsubscribeButton->SetFocus();
		OriginalReply.SetUserFocus(UnsubscribeButton->TakeWidget(), EFocusCause::Navigation);
	}
	return OriginalReply;
}

void UModioDownloadQueueEntry::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image);
	if (ec)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - %d"), *ec.GetErrorMessage(), ec.GetValue());
	}
	else
	{
		if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
		{
			if (ModID == ModInfo->Underlying.ModId)
			{
				ModThumbnail->LoadImageFromFileAsync(Image.GetValue());
			}
		}
	}
}

const FModioRichTextStyle& UModioDownloadQueueEntry::GetRichTextStyle()
{
	if (const FModioDownloadQueueEntryStyle* ResolvedStyle = EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
	{
		if (const FModioRichTextStyle* ResolvedTextStyle = ResolvedStyle->TextStyle.FindStyle<FModioRichTextStyle>())
		{
			return *ResolvedTextStyle;
		}
	}
	return FModioRichTextStyle::GetDefault();
}
