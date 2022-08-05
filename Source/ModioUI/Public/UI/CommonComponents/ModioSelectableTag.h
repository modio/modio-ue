// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioCheckBox.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Interfaces/IModioUIUserStringListEntry.h"

#include "ModioSelectableTag.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioSelectableTag : public UModioUserWidgetBase,
										public IUserObjectListEntry,
										public IModioUIUserStringListEntry
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	virtual void OnCheckboxStateChanged(bool bIsChecked);

	virtual void NativeSetListItemString(const FString& InString) override
	{
		if (TagSelectedCheckbox)
		{
			TagSelectedCheckbox->SetLabelText(FText::FromString(InString));
		}
	};

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioCheckBox* TagSelectedCheckbox;
};
