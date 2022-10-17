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

#include "Components/ScrollBox.h"
#include "CoreMinimal.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioScrollBox.generated.h"

/**
* Enumerator for the possible position slots for a scroll box bar
**/
UENUM(BlueprintType)
enum class EModioScrollBoxBarAlignment : uint8
{
	/** Default box bar position **/
	Default,

	/** Left box bar position **/
	Left,

	/** Right box bar position **/
	Right,

	/** Top box bar position **/
	Top,

	/** Bottom box bar position **/
	Bottom
};

/**
* Modio UI element that represents a box with the capacity to move
* around the content it displays
**/
UCLASS()
class MODIOUI_API UModioScrollBox : public UScrollBox
{
	GENERATED_BODY()

protected:
	SGridPanel::FSlot* ScrollbarSlot = nullptr;
	TSharedPtr<class SScrollBar> MyScrollbar;
	TSharedPtr<class SGridPanel> MyGridForCustomScrollbar;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	EModioScrollBoxBarAlignment ScrollBarAlignment = EModioScrollBoxBarAlignment::Default;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	float AdditionalPadding = 0;
};
