// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ComboBoxString.h"
#include "CoreMinimal.h"
#include "Input/PopupMethodReply.h"
#include "UI/BaseWidgets/Slate/SModioComboBox.h"

#include "ModioComboBoxString.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioComboBoxString : public UComboBoxString
{
	GENERATED_BODY()
protected:
	TSharedPtr<SModioComboBox<TSharedPtr<FString>>> MyModioComboBox;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TEnumAsByte<EMenuPlacement> Placement = MenuPlacement_ComboBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bCreateNewWindow = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bWrapMenuContent = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
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
