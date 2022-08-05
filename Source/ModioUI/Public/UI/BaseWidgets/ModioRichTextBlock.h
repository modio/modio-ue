// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/RichTextBlock.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioRichTextBlock.generated.h"

/**
 *
 */
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category="Widgets")
	FName DefaultStyleName = FName("default");
	

public:
	virtual void SynchronizeProperties() override;
	FOnGetRichTextStyle& GetStyleDelegate();

	void AddDecorator(TSubclassOf<URichTextBlockDecorator> DecoratorClass);
	void SetDefaultStyleName(FName DefaultName = "default");
};
