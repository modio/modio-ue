#pragma once
#include "Brushes/SlateColorBrush.h"
#include "Engine/Engine.h"
#include "Math/Color.h"
#include "ModioUISubsystem.h"
#include "UObject/NameTypes.h"

#include "ModioUIColor.generated.h"

USTRUCT(BlueprintType)
struct FModioUIColor
{
	GENERATED_BODY()
protected:
	TSharedPtr<FLinearColor> InternalColor;
	TSharedPtr<FSlateColorBrush> CachedBrush;

public:
	FModioUIColor() : FModioUIColor(FLinearColor::White) {};

	FModioUIColor(const FModioUIColor& Other) = default;

	FModioUIColor(FLinearColor Color)
	{
		InternalColor = MakeShared<FLinearColor>(Color);
	};

	operator FSlateColor()
	{
		return FSlateColor(InternalColor.ToSharedRef());
	}

	operator const FSlateColor() const
	{
		return FSlateColor(InternalColor.ToSharedRef());
	}

	explicit operator FLinearColor()
	{
		return *InternalColor.Get();
	}

	const FSlateColorBrush* GetBrush()
	{
		if (!CachedBrush)
		{
			CachedBrush = MakeShared<FSlateColorBrush>(InternalColor.ToSharedRef());
		}
		return CachedBrush.Get();
	}

	FLinearColor* GetUnderlyingMutableColor()
	{
		return InternalColor.Get();
	}

	void SetColor(FLinearColor Color)
	{
		*GetUnderlyingMutableColor() = Color;
	}

	void InitializeFromSlateColor(FSlateColor Color)
	{
		if (Color.IsColorSpecified())
		{
			*GetUnderlyingMutableColor() = Color.GetSpecifiedColor();
		}
	}

	bool Serialize(FArchive& Ar)
	{
		if (Ar.GetArchiveState().IsLoading())
		{
			FLinearColor LoadedColor;
			Ar << LoadedColor;
			SetColor(LoadedColor);
		}
		else
		{
			Ar << *GetUnderlyingMutableColor();
		}

		return true;
	}
};

template<>
struct TStructOpsTypeTraits<FModioUIColor> : public TStructOpsTypeTraitsBase2<FModioUIColor>
{
	enum
	{
		WithSerializer = true,
	};
};