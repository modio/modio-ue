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

#include "Brushes/SlateColorBrush.h"
#include "Engine/Engine.h"
#include "Math/Color.h"
#include "ModioUISubsystem.h"
#include "UObject/NameTypes.h"

#include "ModioUIColor.generated.h"

/**
* Strong type struct to represent a UI color
**/
USTRUCT(BlueprintType)
struct FModioUIColor
{
	GENERATED_BODY()
protected:
	TSharedPtr<FLinearColor> InternalColor;
	TSharedPtr<FSlateColorBrush> CachedBrush;

public:
	/**
	* Constructor without parameters that starts as a white color
	**/
	FModioUIColor() : FModioUIColor(FLinearColor::White) {};

	/**
	* Default constructor that copies parameters from another ModioUIColor
	* @param Other The ModioUIColor instance to copy parameters into this instance
	**/
	FModioUIColor(const FModioUIColor& Other) = default;

	/**
	* Convenience constructor that makes a shared pointer with the color parameter
	* @param Color The linear color to create this instance
	**/
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

	/**
	* Retrieve the color as a slate brush
	* @return FSlateColorBrush pointer of the stored brush
	**/
	const FSlateColorBrush* GetBrush()
	{
		if (!CachedBrush)
		{
			CachedBrush = MakeShared<FSlateColorBrush>(InternalColor.ToSharedRef());
		}
		return CachedBrush.Get();
	}

	/**
	* Retrieve the baseline color
	* @return FLinearColor pointer of the stored value
	**/
	FLinearColor* GetUnderlyingMutableColor()
	{
		return InternalColor.Get();
	}

	/**
	* Update the baseline color
	* @param Color FLinearColor to copy into this instance
	**/
	void SetColor(FLinearColor Color)
	{
		*GetUnderlyingMutableColor() = Color;
	}

	/**
	* Transform a slate color to a Modio UI color
	* @param Color FSlateColor to copy into this instance
	**/
	void InitializeFromSlateColor(FSlateColor Color)
	{
		if (Color.IsColorSpecified())
		{
			*GetUnderlyingMutableColor() = Color.GetSpecifiedColor();
		}
	}

	/**
	* Store this instance into an archive
	* @param Ar The archive class that receives information
	* @return Always true when the ModioUIColor is forwarded to Ar
	**/
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