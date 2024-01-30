/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Widgets/Layout/SSpacer.h"
#include "Templates/SubclassOf.h"
#include "ModioCommonTickableWidget.generated.h"

class UTickableModioCommonWidget;

/**
 * Empty widget that can be used to tick a UTickableModioCommonWidget
 */
class MODIOUI_API STickableModioCommonWidget : public SSpacer
{
public:
	STickableModioCommonWidget()
	{
		SetCanTick(true);
	}

	SLATE_BEGIN_ARGS(STickableModioCommonWidget) {}
		SLATE_ARGUMENT(UTickableModioCommonWidget*, OwningWidget)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		OwningWidget = InArgs._OwningWidget;
	}

	//~ Begin SWidget Interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	//~ End SWidget Interface

private:
	/** UMG widget that owns this slate widget */
	UTickableModioCommonWidget* OwningWidget = nullptr;
};

/**
 * Widget that can be ticked
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UTickableModioCommonWidget : public UWidget
{
	GENERATED_BODY()

protected:
	friend class STickableModioCommonWidget;

	/**
	 * Called every frame
	 * @param DeltaTime Time since last frame
	 */
	virtual void Tick(float DeltaTime)
	{
		
	}

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override
	{
		MyTickableWidget = SNew(STickableModioCommonWidget)
		                   .OwningWidget(this);
		return MyTickableWidget.ToSharedRef();
	}

	virtual void ReleaseSlateResources(bool bReleaseChildren) override
	{
		Super::ReleaseSlateResources(bReleaseChildren);
		MyTickableWidget.Reset();
	}
	//~ End UWidget Interface

private:
	/** Underlying slate widget */
	TSharedPtr<STickableModioCommonWidget> MyTickableWidget;
};

inline void STickableModioCommonWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (OwningWidget)
	{
		OwningWidget->Tick(InDeltaTime);
	}
}
