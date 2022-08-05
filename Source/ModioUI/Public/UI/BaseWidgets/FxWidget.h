// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SFxWidget.h"

#include "FxWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UFxWidget : public UWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<SFxWidget> MySFxWidget = nullptr;
	TSharedPtr<SWidget> MyContentWidget = nullptr;
	virtual void SynchronizeProperties() override
	{
		RebuildWidget();
		Super::SynchronizeProperties();
	}
	virtual void ReleaseSlateResources(bool bReleaseChildren) override
	{
		Super::ReleaseSlateResources(bReleaseChildren);
		MySFxWidget.Reset();
		MyContentWidget.Reset();
	}
	virtual TSharedRef<SWidget> RebuildWidget() override
	{
		return SAssignNew(MySFxWidget, SFxWidget)
			.RenderScale(RenderScale)
			.RenderScaleOrigin(RenderScaleOrigin)
			.LayoutScale(LayoutScale)
			.VisualOffset(VisualOffset)
			.IgnoreClipping(bIgnoreClipping)[GenerateContentWidget()];
	}

	virtual TSharedRef<SWidget> GenerateContentWidget()
	{
		TSharedPtr<SWidget> SlateMenuWidget;

		if (OnGetContent.IsBound())
		{
			UWidget* ContentWidget = OnGetContent.Execute();
			if (ContentWidget)
			{
				MyContentWidget = ContentWidget->TakeWidget();
			}
		}
		else if (ContentClass != nullptr && !ContentClass->HasAnyClassFlags(CLASS_Abstract))
		{
			if (UUserWidget* MenuWidget = CreateWidget(this, ContentClass))
			{
				MyContentWidget = MenuWidget->TakeWidget();
			}
		}
		else
		{
			MyContentWidget = SNullWidget::NullWidget;
		}
		return MyContentWidget.ToSharedRef();
	}

public:
	UFxWidget(const FObjectInitializer& Initializer)
		: Super(Initializer),
		  RenderScale(1),
		  RenderScaleOrigin(FVector2D::ZeroVector),
		  LayoutScale(1),
		  VisualOffset(FVector2D::ZeroVector) {};

	TSharedPtr<SWidget> GetContentWidget()
	{
		return MyContentWidget;
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float RenderScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FVector2D RenderScaleOrigin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	float LayoutScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FVector2D VisualOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	bool bIgnoreClipping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	TSubclassOf<UUserWidget> ContentClass;

	UPROPERTY(EditAnywhere, Category = "Events")
	FGetWidget OnGetContent;
};