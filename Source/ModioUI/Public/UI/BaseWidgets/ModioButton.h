// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetLabel, FText, NewLabel);

/**
 *
 */
UCLASS()
class MODIOUI_API UModioButton : public UButton
{
	GENERATED_BODY()

protected:
	bool bWrapContent = false;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	void SynchronizeProperties() override;

	UPROPERTY(BlueprintAssignable, Category = "Data Binding")
	FOnSetLabel OnSetLabel;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, meta=(StyleClass="ModioButtonStyle", DesignerRebuild), Category="Appearance")
	FModioUIStyleRef ButtonStyle = FModioUIStyleRef {"DefaultButtonStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Appearance",
			  meta = (EditCondition = "bOverrideGlobalStyle"))
	FModioWidgetBorderStyle BorderStyle;

	TSharedPtr<SWidget> WrappedContent;
	virtual TSharedRef<SWidget> RebuildWidget() override;


	virtual void ApplyStyle();
	#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	#endif
public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Appearance")
	bool bOverrideGlobalStyle = false;

	// this should be a container of styles, and we store one of those globally

	UFUNCTION(BlueprintCallable, Category="ModioButton")
	void SetLabel(FText NewLabel);

	UFUNCTION(BlueprintCallable, Category="ModioButton")
	void SetButtonStyle(FModioUIStyleRef ButtonStyleRef, bool bApplyStyle = false);
};
