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

#include "Components/ComboBoxString.h"
#include "CoreMinimal.h"
#include "Input/PopupMethodReply.h"
#include "UI/BaseWidgets/Slate/SModioComboBox.h"

#include "ModioComboBoxString.generated.h"

/**
* Modio UI element that represents an entry within a combo box
**/
UCLASS()
class MODIOUI_API UModioComboBoxString : public UComboBoxString
{
	GENERATED_BODY()
protected:
	TSharedPtr<SModioComboBox<TSharedPtr<FString>>> MyModioComboBox;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TEnumAsByte<EMenuPlacement> Placement = MenuPlacement_ComboBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bCreateNewWindow = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	bool bWrapMenuContent = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	TSoftObjectPtr<UMaterialInterface> ContentWrapMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	bool bEnableBorder = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	FColor BorderColor = FColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderRadius = 48;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderThickness = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Border Effect")
	float BorderOpacity = 1;

	TSharedPtr<class SRetainerWidget> MyWrappedContent;
	TSharedPtr<SRetainerWidget> MyWrappedButton;

	/** Called by slate when it needs to generate a new item for the combobox */
	virtual TSharedRef<SWidget> HandleGenerateWidget(TSharedPtr<FString> Item) const;
};
