// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"

#include "ModioRichTextBlockDecorator.generated.h"


class MODIOUI_API FModioRichTextDecorator : public ITextDecorator
{

public:
	FModioRichTextDecorator() = default;
	FModioRichTextDecorator(URichTextBlock* InOwner) : Owner(InOwner) {};
	bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override;

protected:
	virtual TSharedRef<ISlateRun> Create(const TSharedRef<class FTextLayout>& TextLayout,
										 const FTextRunParseResults& RunParseResult, const FString& OriginalText,
										 const TSharedRef<FString>& InOutModelText,
										 const ISlateStyle* Style) override;


	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo,
											  const FTextBlockStyle& DefaultTextStyle,
											  const ISlateStyle* StyleSet) const;
	virtual void CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle, FString& InOutString, const ISlateStyle* StyleSet) const;

	URichTextBlock* Owner;
};


/**
 * 
 */
UCLASS()
class MODIOUI_API UModioRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
	
public:
	TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};
