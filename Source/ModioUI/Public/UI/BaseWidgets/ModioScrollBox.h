// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ScrollBox.h"
#include "CoreMinimal.h"
#include "Widgets/Layout/SGridPanel.h"

#include "ModioScrollBox.generated.h"

UENUM(BlueprintType)
enum class EModioScrollBoxBarAlignment : uint8
{
	Default,
	Left,
	Right,
	Top,
	Bottom
};
/**
 *
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	EModioScrollBoxBarAlignment ScrollBarAlignment = EModioScrollBoxBarAlignment::Default;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	float AdditionalPadding = 0;
};
