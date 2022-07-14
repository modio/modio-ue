// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioButton.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Settings/ModioUISettings.h"
#include "Slate/SObjectWidget.h"
#include "Slate/SRetainerWidget.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"

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
	// OnHovered.Clear();
	// OnHovered.AddDynamic(this, &UModioButton::SetHovered);
	Super::RebuildWidget();
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