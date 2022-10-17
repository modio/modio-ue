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

#include "Components/RichTextBlock.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioRichTextBlock.generated.h"

/**
* Class definition for the Modio rich text block that retains information 
* about the supported styles
**/
UCLASS()
class MODIOUI_API UModioRichTextBlock : public URichTextBlock
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	UModioRichTextBlock(const FObjectInitializer& Initializer);

	UFUNCTION()
	TArray<FString> GetStyleNames() const;

	virtual void UpdateStyleData() override;

	/*
		UFUNCTION(BlueprintNativeEvent)
		void GetStyle(FModioRichTextStyle& Style);*/

	FOnGetRichTextStyle OnGetStyle;

	// TODO: Override RebuildWidget to pass a pointer to this in
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category = "Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category = "Widgets")
	FName DefaultStyleName = FName("default");

public:
	virtual void SynchronizeProperties() override;

	/**
	* Retrieve the delegate assigned to this instance
	* @return The rich text style delegate 
	**/
	FOnGetRichTextStyle& GetStyleDelegate();

	/**
	* Add a text decorator to this instance
	* @param DecoratorClass Subclass of URichTextBlockDecorator to add
	**/
	void AddDecorator(TSubclassOf<URichTextBlockDecorator> DecoratorClass);

	/**
	* Change the default style to use
	* @param DefaultName The new style name to use
	**/
	void SetDefaultStyleName(FName DefaultName = "default");
};
