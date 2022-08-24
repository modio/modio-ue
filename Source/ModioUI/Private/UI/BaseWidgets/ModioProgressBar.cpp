// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioProgressBar.h"
#include "Core/ModioNewPropertyHelpers.h"
#include "ModioUI.h"
#include "PropertyPathHelpers.h"
#include "UI/Styles/ModioProgressBarStyle.h"


void UModioProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (const FModioProgressBarStyle* ResolvedStyle = StyleRef.FindStyle<FModioProgressBarStyle>())
	{
		if (MyProgressBar)
		{
			MyProgressBar->SetStyle(ResolvedStyle);
		}
	}
}

#if UE_VERSION_NEWER_THAN(5, 0, 0)
void UModioProgressBar::PreSave(FObjectPreSaveContext SaveContext)
#else
void UModioProgressBar::PreSave(const class ITargetPlatform* TargetPlatform)
#endif
{
	for (FPropertyValueIterator It = FPropertyValueIterator(FStructProperty::StaticClass(), GetClass(), this); It; ++It)
	{
		const FStructProperty* Prop = CastField<FStructProperty>(It->Key);

		const void* Value = It->Value;
		if (Prop->Struct == FSlateColor::StaticStruct())
		{
			FSlateColor* ActualColor = const_cast<FSlateColor*>(reinterpret_cast<const FSlateColor*>(Value));
			if (!FModioSlateColorInspector(*ActualColor).IsUnlinked())
			{
				UE_LOG(LogModioUI, Display, TEXT("Resetting color %s in %s"), *Prop->GetAuthoredName(),
					   *GetFName().ToString());
				*ActualColor = FSlateColor();
			}
		}
	}
#if UE_VERSION_NEWER_THAN(5, 0, 0)
	Super::PreSave(SaveContext);
#else
	Super::PreSave(TargetPlatform);
#endif
}

void UModioProgressBar::SetStyleRef(FModioUIStyleRef NewStyle)
{
	StyleRef = NewStyle;
	if (const FModioProgressBarStyle* ResolvedStyle = StyleRef.FindStyle<FModioProgressBarStyle>())
	{
		if (MyProgressBar)
		{
			MyProgressBar->SetStyle(ResolvedStyle);
		}
	}
}
