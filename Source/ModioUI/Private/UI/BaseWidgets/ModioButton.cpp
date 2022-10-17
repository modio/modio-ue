/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioButton.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Settings/ModioUISettings.h"
#include "Slate/SObjectWidget.h"
#include "Slate/SRetainerWidget.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "UI/BaseWidgets/Slate/SModioButtonBase.h"
#include "Components/ButtonSlot.h"

void UModioButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	WrappedContent.Reset();
}

void UModioButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	// ApplyStyleOverrides();
}

TSharedRef<SWidget> UModioButton::RebuildWidget()
{
		MyButton = SNew(SModioButtonBase)
				   .OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
				   .OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
				   .OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
				   .OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
				   .OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
				   .ButtonStyle(&WidgetStyle)
				   .ClickMethod(ClickMethod)
				   .TouchMethod(TouchMethod)
				   .PressMethod(PressMethod)
				   .IsFocusable(IsFocusable);

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	WrappedContent = MyButton;
	bWrapContent = false;
	ApplyStyle();
	return WrappedContent.ToSharedRef();
}

void UModioButton::ApplyStyle()
{
	const FModioButtonStyle* Style = ButtonStyle.FindStyle<FModioButtonStyle>();
	if (Style && MyButton)
	{
		MyButton->SetButtonStyle(&Style->ButtonStyle);
		MyButton->SetContentPadding(Style->ContentPadding);

		const FModioWidgetBorderStyle* ActualBorderStyle = Style->BorderStyle.FindStyle<FModioWidgetBorderStyle>();
		if (ActualBorderStyle && ActualBorderStyle->bMaskWithMaterial)
		{
			TSharedPtr<SRetainerWidget> MyWrappedContent;

			if (bWrapContent == false)
			{
				MyWrappedContent = SAssignNew(WrappedContent, SRetainerWidget)[MyButton.ToSharedRef()];
				bWrapContent = true;
			}
			else
			{
				MyWrappedContent = StaticCastSharedPtr<SRetainerWidget>(WrappedContent);
			}

			MyWrappedContent->SetEffectMaterial(ActualBorderStyle->MaskMaterial.LoadSynchronous());
			MyWrappedContent->SetTextureParameter("WidgetTexture");
			MyWrappedContent->GetEffectMaterial()->SetScalarParameterValue("Hovered", 0);
			ApplyModioBorderStyle(MyWrappedContent->GetEffectMaterial(), ActualBorderStyle);
		}
		else
		{
			bWrapContent = false;
		}
	}
}
#if WITH_EDITOR
void UModioButton::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == "ButtonStyle")
	{
		/*ApplyStyle();
		/ *
		InvalidateLayoutAndVolatility();

		TSharedPtr<SWidget> SafeWidget = GetCachedWidget();
		if (SafeWidget.IsValid())
		{
			SafeWidget->Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
		}* /
		MyWidget.Reset();
		TakeWidget();*/
	}
}
#endif
void UModioButton::SetLabel(FText NewLabel)
{
	OnSetLabel.Broadcast(NewLabel);
}

void UModioButton::SetButtonStyle(FModioUIStyleRef ButtonStyleRef, bool bApplyStyle)
{
	ButtonStyle = ButtonStyleRef;
	if (bApplyStyle)
	{
		ApplyStyle();
	}
}

/*
UModioButton::UModioButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}*/