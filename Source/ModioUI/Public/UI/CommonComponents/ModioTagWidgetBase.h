// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioTagWidgetBase.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioTagWidgetBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	const struct FModioRichTextStyle& GetTagTextStyle();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FText TagTextFormat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FString TagCategory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FString TagValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* TagText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidgetOptional))
	UModioImage* TagBackground;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioTagWidgetStyle"), Category="Widgets")
	FModioUIStyleRef TagStyle = FModioUIStyleRef {"DefaultTagWidgetStyle"};

public:
	UFUNCTION()
	void SetTagTextFormat(FText NewFormat)
	{
		TagTextFormat = NewFormat;
	}
	UFUNCTION()
	void SetTagValue(FString Category, FString NewValue)
	{
		TagCategory = Category;
		TagValue = NewValue;
	}
};
