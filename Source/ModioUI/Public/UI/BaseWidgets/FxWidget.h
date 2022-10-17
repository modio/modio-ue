/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SFxWidget.h"

#include "FxWidget.generated.h"

/**
* Modio UI element that extends the functionality of UWidget with
* further pointers to a content widget and properties to scale the 
* rendered elements
**/
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
	/**
	* Default UFxWidget constructor 
	* @param Initializer The FObjectInitializer to use as base configuration
	**/
	UFxWidget(const FObjectInitializer& Initializer)
		: Super(Initializer),
		  RenderScale(1),
		  RenderScaleOrigin(FVector2D::ZeroVector),
		  LayoutScale(1),
		  VisualOffset(FVector2D::ZeroVector) {};

	/**
	* Retrieve the Widget for this instance
	* @return The Widget shared pointer of this instance
	**/
	TSharedPtr<SWidget> GetContentWidget()
	{
		return MyContentWidget;
	};

	/**
	* Stored property of the proportion scale to render the widget
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	float RenderScale;

	/**
	* Stored property of the 2D point where the widget origin is located
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FVector2D RenderScaleOrigin;

	/**
	* Stored property to the scale of the widget layout
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	float LayoutScale;

	/**
	* Stored property of the 2D point that represents the offset of the contents to display
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	FVector2D VisualOffset;

	/**
	* Stored property boolean when it is true, the widget ignores clipping, otherwise false
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	bool bIgnoreClipping;

	/**
	* Stored property of the user widget subclass that stores the content to display
	**/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> ContentClass;

	/**
	* Stored property of the content to display when an action occurs in the widget
	**/
	UPROPERTY(EditAnywhere, Category = "Events")
	FGetWidget OnGetContent;
};