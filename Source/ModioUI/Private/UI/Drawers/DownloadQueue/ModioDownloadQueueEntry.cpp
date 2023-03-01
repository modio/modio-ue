/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

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

	if (EntryBorder)
	{
		EntryBorder->OnMouseButtonDownEvent.BindDynamic(this, &UModioDownloadQueueEntry::OnEntryPressed);
	}
}

void UModioDownloadQueueEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	SetDataSource(ListItemObject);
}

void UModioDownloadQueueEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	// This function is never called. Saved for now in case this is needed in the future

	//if (EntryBorder)
	//{
	//	if (bIsSelected)
	//	{
	//		if (const FModioDownloadQueueEntryStyle* ResolvedStyle =
	//				EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
	//		{
	//			EntryBorder->SetBrush(ResolvedStyle->HighlightedBorderBrush);
	//		}
	//	}
	//	else
	//	{
	//		EntryBorder->SetBrush(FSlateNoResource());
	//	}
	//}
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

void UModioDownloadQueueEntry::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (EntryBorder)
	{
		if (const FModioDownloadQueueEntryStyle* ResolvedStyle = EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
		{
			EntryBorder->SetBrush(ResolvedStyle->HighlightedBorderBrush);
			HoveredSound = ResolvedStyle->HoveredSound;
			FSlateApplication::Get().PlaySound(HoveredSound);
		}
	}
}

void UModioDownloadQueueEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) {
	if (EntryBorder)
	{
		EntryBorder->SetBrush(FSlateNoResource());
	}
}

FEventReply UModioDownloadQueueEntry::OnEntryPressed(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
			{
				Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);

				if (const FModioDownloadQueueEntryStyle* ResolvedStyle =
						EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
				{
					PressedSound = ResolvedStyle->PressedSound;
					FSlateApplication::Get().PlaySound(PressedSound);
				}

				return FEventReply(true);
			}
		}
	}
	return FEventReply(false);
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
