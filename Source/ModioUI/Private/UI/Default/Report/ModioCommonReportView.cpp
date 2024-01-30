/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Default/Report/ModioCommonReportView.h"

#include "Components/WidgetSwitcherSlot.h"
#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Foundation/Base/ModioCommonWidgetSwitcher.h"
#include "Core/ModioModInfoUI.h"
#include "ModioSettings.h"
#include "ModioUI.h"
#include "UI/Default/Report/ModioCommonReportEmailView.h"
#include "UI/Default/Report/ModioCommonReportReasonView.h"
#include "UI/Default/Report/ModioCommonReportMessageView.h"
#include "UI/Default/Report/ModioCommonReportSummaryView.h"
#include "UI/Foundation/Base/Report/ModioCommonReportSummaryViewBase.h"
#include "UI/Default/Dialog/ModioCommonDialogInfo.h"
#include "UI/Default/ModBrowser/ModioCommonModBrowser.h"

void UModioCommonReportView::NativeOnActivated()
{
	if (ReportSwitcher)
	{
		ReportSwitcher->ClearChildren();
		TArray<UWidgetSwitcherSlot*> SwitcherSlots;

		if (ReportReasonViewClass)
		{
			ReportReasonView = CreateWidget<UModioCommonReportReasonViewBase>(this, ReportReasonViewClass);
			ReportReasonView->OnProceedClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportReasonViewProceedClicked);
			ReportReasonView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportReasonViewCancelClicked);
			SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(ReportSwitcher->AddChild(ReportReasonView)));
		}

		if (ReportEmailViewClass)
		{
			ReportEmailView = CreateWidget<UModioCommonReportEmailViewBase>(this, ReportEmailViewClass);
			ReportEmailView->OnBackClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportEmailViewBackClicked);
			ReportEmailView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportEmailViewCancelClicked);
			ReportEmailView->OnSubmitClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportEmailViewSubmitClicked);
			SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(ReportSwitcher->AddChild(ReportEmailView)));
		}

		if (ReportMessageViewClass)
		{
			ReportMessageView = CreateWidget<UModioCommonReportMessageViewBase>(this, ReportMessageViewClass);
			ReportMessageView->OnBackClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportMessageViewBackClicked);
			ReportMessageView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportMessageViewCancelClicked);
			ReportMessageView->OnSubmitClicked.AddUniqueDynamic(this,&UModioCommonReportView::OnReportMessageViewSubmitClicked);
			SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(ReportSwitcher->AddChild(ReportMessageView)));
		}

		if (ReportSummaryViewClass)
		{
			ReportSummaryView = CreateWidget<UModioCommonReportSummaryViewBase>(this, ReportSummaryViewClass);
			ReportSummaryView->OnBackClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportSummaryViewBackClicked);
			ReportSummaryView->OnCancelClicked.AddUniqueDynamic(this, &UModioCommonReportView::OnReportSummaryViewCancelClicked);
			ReportSummaryView->OnSubmitClicked.AddUniqueDynamic(this,&UModioCommonReportView::OnReportSummaryViewSubmitClicked);
			SwitcherSlots.Add(Cast<UWidgetSwitcherSlot>(ReportSwitcher->AddChild(ReportSummaryView)));
		}

		for (UWidgetSwitcherSlot* SwitcherSlot : SwitcherSlots)
		{
			if (SwitcherSlot)
			{
				SwitcherSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				SwitcherSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
		}

		ReportSwitcher->SetActiveWidgetIndex(0);
	}

	Super::NativeOnActivated();
}

void UModioCommonReportView::NativeOnDeactivated()
{
	if (ReportSwitcher)
	{
		ReportSwitcher->SetActiveWidgetIndex(0);
		ReportSwitcher->ClearChildren();
	}
	Super::NativeOnDeactivated();
}

void UModioCommonReportView::NativeOnSetDataSource() 
{
	Super::NativeOnSetDataSource();

	UModioModInfoUI* ModInfoUI = Cast<UModioModInfoUI>(DataSource);
	if (ModInfoUI) 
	{
		if (ReportReasonView)
		{
			ReportReasonView->SetDataSource(ModInfoUI);
		}

		if (ReportEmailView)
		{
			ReportEmailView->SetDataSource(ModInfoUI);
		}

		if (ReportMessageView)
		{
			ReportMessageView->SetDataSource(ModInfoUI);
		}

		if (ReportSummaryView)
		{
			ReportSummaryView->SetDataSource(ModInfoUI);
		}
	}
}

UWidget* UModioCommonReportView::NativeGetDesiredFocusTarget() const
{
	if (UWidget* WidgetToFocus = BP_GetDesiredFocusTarget())
	{
		return WidgetToFocus;
	}

	if (ReportSwitcher && ReportSwitcher->GetChildrenCount() > 0)
	{
		if (UModioCommonActivatableWidget* ActivatableWidget = Cast<UModioCommonActivatableWidget>(ReportSwitcher->GetActiveWidget()))
		{
			return ActivatableWidget->GetDesiredFocusTarget();
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UModioCommonReportView::OnReportReasonViewProceedClicked_Implementation(EModioReportType Type) 
{
	if (ReportEmailView) 
	{
		ReportParams.Type = Type;
		ReportSwitcher->SetActiveWidget(ReportEmailView);
	}
}

void UModioCommonReportView::OnReportReasonViewCancelClicked_Implementation() 
{
	DeactivateWidget();
}

void UModioCommonReportView::OnReportEmailViewBackClicked_Implementation() 
{
	if (ReportReasonView) 
	{
		ReportSwitcher->SetActiveWidget(ReportReasonView);
	}
}

void UModioCommonReportView::OnReportEmailViewSubmitClicked_Implementation(const FString& Email) 
{
	if (ReportEmailView) 
	{
		if (!ReportEmailView->IsEmailValid(Email))
		{
			UE_LOG(ModioUI, Error, TEXT("Unable to submit report due to invalid email address (%s)"), *Email);
			return;
		}
	}

	if (ReportMessageView) 
	{
		ReportParams.ReporterContact = Email;
		ReportSwitcher->SetActiveWidget(ReportMessageView);
	}
}

void UModioCommonReportView::OnReportEmailViewCancelClicked_Implementation() 
{
	DeactivateWidget();
}

void UModioCommonReportView::OnReportMessageViewBackClicked_Implementation() 
{
	if (ReportEmailView) 
	{
		ReportSwitcher->SetActiveWidget(ReportEmailView);
	}
}

void UModioCommonReportView::OnReportMessageViewSubmitClicked_Implementation(const FString& Message)
{
	UModioCommonReportMessageView* Ref = Cast<UModioCommonReportMessageView>(ReportMessageView);
	if(Ref) 
	{
		if (!Ref->IsMessageValid(Message)) 
		{
			return;
		}
	}
	if (ReportSummaryView) 
	{
		ReportParams.ReportDescription = Message;
		ReportSummaryView->SetReportParams(ReportParams);
		ReportSwitcher->SetActiveWidget(ReportSummaryView);
	}
}

void UModioCommonReportView::OnReportMessageViewCancelClicked_Implementation() 
{
	DeactivateWidget();
}

void UModioCommonReportView::OnReportSummaryViewBackClicked_Implementation()
{
	if (ReportEmailView) 
	{
		ReportSwitcher->SetActiveWidget(ReportMessageView);
	}
}

void UModioCommonReportView::OnReportSummaryViewSubmitClicked_Implementation()
{
	UModioCommonReportSummaryView* Ref = Cast<UModioCommonReportSummaryView>(ReportSummaryView);
	if(Ref)
	{
		if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>()) 
		{
			const UModioSettings* Settings = GetDefault<UModioSettings>();
			ReportParams.ResourceID = Settings->GameId;
			ModioSubsystem->ReportContentAsync(ReportParams, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioCommonReportView::HandleReportSubmit));

			if (Ref) 
			{
				Ref->SetIsBusy(true);
			}
		}
	}
}

void UModioCommonReportView::OnReportSummaryViewCancelClicked_Implementation()
{
	DeactivateWidget();
}

void UModioCommonReportView::HandleReportSubmit_Implementation(FModioErrorCode ErrorCode) 
{
	UModioCommonReportSummaryView* Ref = Cast<UModioCommonReportSummaryView>(ReportSummaryView);
	if (Ref)
	{
		Ref->SetIsBusy(false);
	}

	if (ErrorCode == 0) 
	{
		if (UModioModInfoUI* ModInfoUI = Cast<UModioModInfoUI>(DataSource))
		{
			UE_LOG(ModioUI, Log, TEXT("Report successfully submitted for mod %s"), *ModInfoUI->Underlying.ProfileName);
			DeactivateWidget();

			if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				if (UModioCommonDialogInfo* DialogInfo = UModioCommonDialogLibrary::CreateManualDialogInfo(FText::FromString(ModInfoUI->Underlying.ProfileName), NSLOCTEXT("Modio", "ReportSubmissionPopupText", "Your report has been submitted successfully.")))
				{
					ModioUISubsystem->ShowDialog(DialogInfo);
				}
			}
		}
		else
		{
			UE_LOG(ModioUI, Error, TEXT("Report successfully submitted, but no mod info was found"));
		}
	}
	else
	{
		if (UModioModInfoUI* ModInfoUI = Cast<UModioModInfoUI>(DataSource))
		{
			if (UModioUISubsystem* ModioUISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				if (UModioCommonDialogInfo* DialogInfo = UModioCommonDialogLibrary::CreateManualDialogInfo(FText::FromString(ModInfoUI->Underlying.ProfileName), NSLOCTEXT("Modio", "ReportSubmissionPopupText", "Failed to report due to unknown circumstances. Make sure to write your message containing correct information.")))
				{
					ModioUISubsystem->ShowDialog(DialogInfo);
				}
			}
		}
	}
}
