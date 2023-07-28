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
#include "GameFramework/InputSettings.h"
#include "Settings/ModioUISettings.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioDownloadQueueEntryStyle.h"

void UModioDownloadQueueEntry::NativeOnSetDataSource()
{
	UModioUserWidgetBase::NativeOnSetDataSource();
	UModioModInfoUI* ActualData = Cast<UModioModInfoUI>(DataSource);
	if (ActualData)
	{
		if (ModSizeLabel)
		{
			ModSizeLabel->SetText(UModioSDKLibrary::Filesize_ToString(ActualData->Underlying.FileInfo.Filesize));
		}
		if (ModStatusLabel)
		{
			ModStatusLabel->SetVisibility(ESlateVisibility::Collapsed);
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
	if (EntryBorder)
	{
		if (bIsSelected)
		{
			if (const FModioDownloadQueueEntryStyle* ResolvedStyle =
					EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
			{
				EntryBorder->SetBrush(ResolvedStyle->HighlightedBorderBrush);
				HoveredSound = ResolvedStyle->HoveredSound;
				FSlateApplication::Get().PlaySound(HoveredSound);
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

	if (EntryBorder)
	{
		if (const FModioDownloadQueueEntryStyle* ResolvedStyle = EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
		{
			EntryBorder->SetBrush(ResolvedStyle->HighlightedBorderBrush);
			HoveredSound = ResolvedStyle->HoveredSound;
			FSlateApplication::Get().PlaySound(HoveredSound);
		}
	}
	return OriginalReply;
}

void UModioDownloadQueueEntry::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (EntryBorder)
	{
		EntryBorder->SetBrush(FSlateNoResource());
	}
}

void UModioDownloadQueueEntry::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ec,
																TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ec, Image, LogoSize);
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

void UModioDownloadQueueEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

void UModioDownloadQueueEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (EntryBorder)
	{
		EntryBorder->SetBrush(FSlateNoResource());
	}
}

FReply UModioDownloadQueueEntry::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
	{
		if (UInputSettings* CurrentInputSettings = UInputSettings::GetInputSettings())
		{
			FModioInputMapping MappedKey;

			for (int i = 0; i < CurrentUISettings->ModioToProjectInputMappings.Num(); i++)
			{
				if (CurrentUISettings->ModioToProjectInputMappings[i].VirtualKey == FModioInputNames::Confirm)
				{
					MappedKey = CurrentUISettings->ModioToProjectInputMappings[i];
					break;
				}
			}

			if (MappedKey.MappedProjectInputs.IsValidIndex(0))
			{
				TArray<FInputActionKeyMapping> ActionMappings;
				CurrentInputSettings->GetActionMappingByName(MappedKey.MappedProjectInputs[0], ActionMappings);

				for (auto& ActionMapping : ActionMappings)
				{
					if (ActionMapping.Key == InKeyEvent.GetKey()) {

						if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
						{
							if (UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource))
							{
								Subsystem->ShowDetailsForMod(ModInfo->Underlying.ModId);
								GEngine->GetEngineSubsystem<UModioUISubsystem>()->CloseDownloadDrawer();

								if (const FModioDownloadQueueEntryStyle* ResolvedStyle =
										EntryStyle.FindStyle<FModioDownloadQueueEntryStyle>())
								{
									PressedSound = ResolvedStyle->PressedSound;
									FSlateApplication::Get().PlaySound(PressedSound);
								}

								
								return FReply::Handled();
							}
						}
					}
				}
			}
		}
	}
	return FReply::Unhandled();
}

void UModioDownloadQueueEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UModioModInfoUI* ModInfo = Cast<UModioModInfoUI>(DataSource);

	if (ModNameLabel && IsValid(ModInfo))
	{
		FString modName = TruncateLongModName(ModInfo->Underlying.ProfileName, ModNameLabel, truncateDivider);
		ModNameLabel->SetText(FText::FromString(modName));
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
				GEngine->GetEngineSubsystem<UModioUISubsystem>()->CloseDownloadDrawer();

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
