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
