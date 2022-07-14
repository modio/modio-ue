// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Notifications/ModioNotificationWidgetBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/Styles/ModioNotificationStyle.h"
#include "Widgets/Notifications/INotificationWidget.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioNotificationErrorWidgetBase::NativeConfigure(const FModioNotificationParams& Params)
{
	Super::NativeConfigure(Params);
	bool bIsError = Params.ErrorCode;

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		bIsError = bPreviewAsError;
	}
#endif

	if (NotificationTitle)
	{
		if (bIsError)
		{
			if (Params.NamedTextFormats.Contains(FName("ErrorText")))
			{
				NotificationTitle->SetText(FText::Format(Params.NamedTextFormats["ErrorText"], Params.FormatArgs));
			}
			else
			{
				NotificationTitle->SetText(LOCTEXT("GenericErrorText", "Error"));
			}
		}
		else
		{
			if (Params.NamedTextFormats.Contains(FName("SuccessText")))
			{
				NotificationTitle->SetText(FText::Format(Params.NamedTextFormats["SuccessText"], Params.FormatArgs));
			}
			else
			{
				NotificationTitle->SetText(LOCTEXT("GenericSuccessText", "Success!"));
			}
		}
	}

	if (NotificationMessage)
	{
		if (Params.NamedTextFormats.Contains(FName("MessageText")))
		{
			NotificationMessage->SetText(FText::Format(Params.NamedTextFormats["MessageText"], Params.FormatArgs));
			NotificationMessage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			NotificationMessage->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (const FModioNotificationStyle* ResolvedStyle = NotificationStyle.FindStyle<FModioNotificationStyle>())
	{
		if (StatusIndicator)
		{
			UMaterialInterface* GlyphMaterial = nullptr;
			if (bIsError)
			{
				GlyphMaterial = ResolvedStyle->ErrorFailureGlyph.ResolveReference();
			}
			else
			{
				GlyphMaterial = ResolvedStyle->ErrorSuccessGlyph.ResolveReference();
			}
			if (GlyphMaterial)
			{
				if (UMaterialInstanceDynamic* AsDynamicMaterial = Cast<UMaterialInstanceDynamic>(GlyphMaterial))
				{
					UMaterialInstanceDynamic* NewInstance =
						UMaterialInstanceDynamic::Create(AsDynamicMaterial->Parent, this);
					NewInstance->CopyInterpParameters(AsDynamicMaterial);
					StatusIndicator->SetBrushFromMaterial(NewInstance);
				}
				else
				{
					UMaterialInstanceDynamic* NewMaterialInstance =
						UMaterialInstanceDynamic::Create(GlyphMaterial, this);
					StatusIndicator->SetBrushFromMaterial(NewMaterialInstance);
				}
			}
		}
		if (NotificationBackground)
		{
			if (UMaterialInterface* BackgroundMaterial = ResolvedStyle->BackgroundMaterial.ResolveReference())
			{
				UMaterialInstanceDynamic* NewMaterialInstance = nullptr;
				if (UMaterialInstanceDynamic* AsDynamicMaterial = Cast<UMaterialInstanceDynamic>(BackgroundMaterial))
				{
					NewMaterialInstance = UMaterialInstanceDynamic::Create(AsDynamicMaterial->Parent, this);
					NewMaterialInstance->K2_CopyMaterialInstanceParameters(AsDynamicMaterial);
				}
				else
				{
					NewMaterialInstance =
						UMaterialInstanceDynamic::Create(BackgroundMaterial, this);
				}

				if (NewMaterialInstance)
				{
					if (bIsError)
					{
						NewMaterialInstance->SetVectorParameterValue(
							FName("Color"), ResolvedStyle->ErrorColor.ResolveReference().GetSpecifiedColor());
					}
					else
					{
						NewMaterialInstance->SetVectorParameterValue(
							FName("Color"), ResolvedStyle->SuccessColor.ResolveReference().GetSpecifiedColor());
					}
					NotificationBackground->SetBrushFromMaterial(NewMaterialInstance);
				}
			}
		}
	}
}

void UModioNotificationErrorWidgetBase::NativeOnDisplay()
{
	bDisplayStarted = true;
	if (!FMath::IsNearlyZero(ExpiryDuration))
	{
		StartTime = FDateTime::Now();
	}
}

void UModioNotificationErrorWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bDisplayStarted)
	{
		if ((FDateTime::Now() - StartTime).GetTotalSeconds() > ExpiryDuration)
		{
			ExpireDelegate.ExecuteIfBound(this);
		}
	}
}

void UModioNotificationWidgetBase::NativeSetNotificationExpireHandler(const FOnNotificationExpired& InDelegate)
{
	ExpireDelegate = InDelegate;
}

void UModioNotificationWidgetBase::NativeConfigure(const FModioNotificationParams& Params)
{
	if (!FMath::IsNearlyZero(Params.Duration))
	{
		ExpiryDuration = Params.Duration;
	}
}
#if WITH_EDITOR
void UModioNotificationWidgetBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (IsDesignTime())
	{
		IModioUINotification::Execute_Configure(this, PreviewParameters);
	}
}
#endif

#include "Loc/EndModioLocNamespace.h"