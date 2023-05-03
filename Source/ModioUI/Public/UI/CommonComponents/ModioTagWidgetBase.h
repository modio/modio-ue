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

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioTagWidgetBase.generated.h"

/**
* Modio UI element that represents a tag which is customizable
* in terms of the text style that can be used
**/
UCLASS()
class MODIOUI_API UModioTagWidgetBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	const struct FModioRichTextStyle& GetTagTextStyle();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FText TagTextFormat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FString TagValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* TagText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidgetOptional))
	UModioImage* TagBackground;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "ModioTagWidgetStyle"), Category = "Widgets")
	FModioUIStyleRef TagStyle = FModioUIStyleRef {"DefaultTagWidgetStyle"};

public:
	/**
	* Update the tag format
	* @param NewFormat The new tag format to set
	**/
	UFUNCTION()
	void SetTagTextFormat(FText NewFormat)
	{
		TagTextFormat = NewFormat;
	}
	
	/**
	* Update the tag value associated with a category
	* @param NewValue The new tag value to set
	**/
	UFUNCTION()
	void SetTagValue(FString Category, FString NewValue)
	{
		TagValue = NewValue;
	}
};
