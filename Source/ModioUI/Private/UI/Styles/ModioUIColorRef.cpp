/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Styles/ModioUIColorRef.h"

#include "ModioUI4Subsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"

FModioUIColorRef::operator FSlateColor() const
{
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (Subsystem)
	{
		UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
		if (DefaultStyle)
		{
			return DefaultStyle->GetSlateColor(ColorName, nullptr, FSlateColor(FLinearColor::Red));
		}
	}
	return FSlateColor(FLinearColor::White);
}

void FModioUIColorRef::SetLinkedColor(FName NewColorName)
{
	ColorName = NewColorName;
}

FSlateColor FModioUIColorRef::ResolveReference(UModioUIStyleSet* OverrideStyleSet) const
{
	if (OverrideStyleSet)
	{
		return OverrideStyleSet->GetSlateColor(ColorName, nullptr, FSlateColor(FLinearColor::Red));
	}
	else
	{
		return *this;
	}
}

const FSlateBrush* FModioUIColorRef::GetBrush() const
{
	UModioUI4Subsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (Subsystem)
	{
		UModioUIStyleSet* DefaultStyle = Subsystem->GetDefaultStyleSet();
		if (DefaultStyle)
		{
			return DefaultStyle->GetSlateColorBrush(ColorName);
		}
	}
	return nullptr;
}

void FModioUIColorRef::ApplyToBrush(FSlateBrush* Target) const
{
	Target->TintColor = *this;
}

bool FModioUIColorRef::Serialize(FArchive& Ar)
{
	if (Ar.IsSaving() && !ColorName.IsValid())
	{
		FName EmptyName(NAME_None);
		Ar << EmptyName;
	}
	else
	{
		Ar << ColorName;
	}
	return true;
}
