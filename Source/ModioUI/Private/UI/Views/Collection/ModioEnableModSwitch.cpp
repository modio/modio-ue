/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/Collection/ModioEnableModSwitch.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioRoundedImage.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequenceTickManager.h"
#include "UI/Views/Collection/ModioModCollectionTile.h"


void UModioEnableModSwitch::NativeConstruct()
{
	SwitchButton->OnPressed.AddUniqueDynamic(this, &UModioEnableModSwitch::TryToggleButton);
	SwitchButton->OnHovered.AddUniqueDynamic(this, &UModioEnableModSwitch::OnButtonHovered);
	SwitchButton->OnUnhovered.AddUniqueDynamic(this, &UModioEnableModSwitch::OnButtonUnhovered);
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
}

void UModioEnableModSwitch::TryToggleButton()
{
	OnButtonPressed.Broadcast();
}

void UModioEnableModSwitch::InitWithModState(bool bIsInactive, bool bEnabled) 
{
	if (bIsInactive)
	{
		EnableModButton->SetIsEnabled(false);
		SwitchButton->SetIsEnabled(false);
		PlayAnimation(ModErrorAnimation);
		return;
	}
	EnableModButton->SetIsEnabled(true);
	SwitchButton->SetIsEnabled(true);
	SetButtonState(bEnabled);
}

void UModioEnableModSwitch::SetButtonState(bool bEnabled) 
{

	EnableModButton->SetStyle(bEnabled ? EnabledStyle.FindStyle<FModioButtonStyle>()->ButtonStyle
									   : DisabledStyle.FindStyle<FModioButtonStyle>()->ButtonStyle);

	if (bUpdateTriggered)
	{
		bUpdateTriggered = false;
		bErrorState = false;
		bIsModEnabled = bEnabled;
		ButtonSizeBox->SetRenderTranslation(bIsModEnabled ? ButtonInitTransform : -ButtonInitTransform);
		return;
	}
	if ((!bIsModEnabled || bErrorState) && bEnabled) 
	{
		PlayAnimation(EnableModAnimation);
	}
	else if ((bIsModEnabled || bErrorState) && !bEnabled)
	{
		PlayAnimationReverse(EnableModAnimation);
	}
	bErrorState = false;
	bIsModEnabled = bEnabled;
}

void UModioEnableModSwitch::OnButtonHovered()
{
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 1);
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 1);
	InputBindingScaleBox->SetVisibility(ESlateVisibility::Visible);
}
void UModioEnableModSwitch::OnButtonUnhovered()
{
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("Hovered"), 0);
	BackgroundImage->GetDynamicMaterial()->SetScalarParameterValue(FName("EnableBorder"), 0);
	InputBindingScaleBox->SetVisibility(ESlateVisibility::Collapsed);
}