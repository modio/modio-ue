#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleSet.h"

FModioUIColorRef::operator FSlateColor() const
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
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
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
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
