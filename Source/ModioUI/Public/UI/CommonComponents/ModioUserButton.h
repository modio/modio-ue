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

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioUserButton.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioUserButton : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<class UButton> RootButton;
	virtual bool Initialize() override
	{
		const bool bInitializedThisCall = Super::Initialize();
		if (bInitializedThisCall)
		{
			UButton* RootButtonRaw =
				WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(TEXT("RootButtonWidget")));
			RootButton = RootButtonRaw;
			if (WidgetTree->RootWidget)
			{
				UButtonSlot* NewSlot = Cast<UButtonSlot>(RootButtonRaw->AddChild(WidgetTree->RootWidget));
				NewSlot->SetPadding(FMargin());
				NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				WidgetTree->RootWidget = RootButtonRaw;
			}
		}
		return bInitializedThisCall;
	}
};
