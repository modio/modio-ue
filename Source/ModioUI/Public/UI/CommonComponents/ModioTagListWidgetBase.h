// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioTagListWidgetBase.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTagListWidgetBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnSetDataSource() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* TagSelectorList;
	int32 CurrentlyDisplayedTagCategoryIndex = 0;
	int32 TagCategoryCount = 0;

public:
	UFUNCTION(BlueprintCallable, Category="ModioTagWidgetBase")
	TArray<FString> GetSelectedTags();

	UFUNCTION(BlueprintCallable, Category="ModioTagWidgetBase")
	void ClearSelectedTags();

	UFUNCTION(BlueprintCallable, Category="ModioTagWidgetBase")
	void DisplayNextTagCategory();

	UFUNCTION(BlueprintCallable, Category="ModioTagWidgetBase")
	void DisplayPrevTagCategory();
};
