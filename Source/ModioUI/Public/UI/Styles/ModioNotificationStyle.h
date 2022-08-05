#pragma once

#include "Brushes/SlateColorBrush.h"
#include "Brushes/SlateNoResource.h"
#include "Styling/SlateTypes.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"

#include "ModioNotificationStyle.generated.h"

USTRUCT(BlueprintType)
struct MODIOUI_API FModioNotificationStyle : public FSlateWidgetStyle
{
	GENERATED_BODY()

	static const FName TypeName;
	virtual const FName GetTypeName() const override
	{
		return TypeName;
	};

	static const FModioNotificationStyle& GetDefault()
	{
		static FModioNotificationStyle Default;
		return Default;
	}
	/// @modio-ue4 this *really* should get factored out into a static helper, huh
	TArray<FString> GetStyleNames() const
	{
		TArray<FString> StyleNames;
		TArray<FName> TmpStyleNames;
		if (const FModioRichTextStyle* RichTextStyle = TextStyleSet.FindStyle<FModioRichTextStyle>())
		{
			RichTextStyle->Styles.GenerateKeyArray(TmpStyleNames);
			for (const FName& Name : TmpStyleNames)
			{
				StyleNames.Add(Name.ToString());
			}
		}
		
		if (StyleNames.Num() == 0)
		{
			StyleNames.Add("Default");
		}
		return StyleNames;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioRichTextStyle"), Category="Widget")
	FModioUIStyleRef TextStyleSet;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category="Widget")
	FName PrimaryTextStyleName = FName("default");

	UPROPERTY(BlueprintReadWrite,EditAnywhere, meta = (GetOptions = "GetStyleNames"), Category="Widget")
	FName SecondaryTextStyleName = FName("default");
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FModioUIMaterialRef ErrorSuccessGlyph;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FModioUIMaterialRef ErrorFailureGlyph;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Widget")
	FModioUIMaterialRef BackgroundMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef SuccessColor;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widget")
	FModioUIColorRef ErrorColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	FMargin ContentPadding;
};

UCLASS(meta = (DisplayName = "ModioNotificationStyle"))
class UModioNotificationStyleContainer : public UModioUIWidgetStyleContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties), Category="Widget")
	FModioNotificationStyle Style;

	virtual void PostLoad() override
	{
		Super::PostLoad();
	}

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &Style;
	}
};
