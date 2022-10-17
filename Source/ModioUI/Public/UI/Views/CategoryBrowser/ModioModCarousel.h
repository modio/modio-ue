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
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "UI/Interfaces/IModioInputMappingAccessor.h"
#include "Widgets/SWidget.h"

#include "ModioModCarousel.generated.h"

template<typename ItemType>
class SWidgetCarouselWithNavigation;

/**
 *
 */
UCLASS()
class MODIOUI_API UModioCarousel : public UWidget, public IModioInputMappingAccessor
{
	GENERATED_BODY()

protected:
	TSharedPtr<SWidgetCarouselWithNavigation<UModioModInfoUI*>> MyCarouselWidget;
	UPROPERTY()
	TArray<UUserWidget*> EntryWidgets;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	UPROPERTY()
	TArray<UModioModInfoUI*> ModsToDisplay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ListEntries,
			  meta = (DesignerRebuild, AllowPrivateAccess = true, MustImplement = UserListEntry))
	TSubclassOf<UUserWidget> EntryWidgetClass;

	TSharedRef<SWidget> OnGenerateWidget(UModioModInfoUI* Item);

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	void SetItems(TArray<UModioModInfoUI*> Items);
};
