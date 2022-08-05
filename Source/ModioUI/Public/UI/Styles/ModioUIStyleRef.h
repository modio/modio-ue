#pragma once
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Styling/ISlateStyle.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "UObject/NameTypes.h"

#include "ModioUIStyleRef.generated.h"

USTRUCT(BlueprintType)
struct FModioUIStyleRef
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Style")
	FName StyleName;

public:
	FModioUIStyleRef() : StyleName(NAME_None) {};
	FModioUIStyleRef(FName StyleName) : StyleName(StyleName) {};
	// TODO: Implement a custom K2Node for resolving the style
	template<typename StyleType>
	const StyleType* const FindStyle() const
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			ISlateStyle* DefaultStyle = (Subsystem->GetDefaultStyleSet());
			if (DefaultStyle && DefaultStyle->HasWidgetStyle<StyleType>(StyleName))
			{
				return &DefaultStyle->GetWidgetStyle<StyleType>(StyleName);
			}
		}
		return nullptr;
	}
	void SetStyleName(FName NewName)
	{
		StyleName = NewName;
	}
	FName GetStyleName() const
	{
		return StyleName;
	}
};