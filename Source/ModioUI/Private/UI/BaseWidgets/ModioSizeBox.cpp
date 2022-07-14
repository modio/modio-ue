// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioSizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "UI/BaseWidgets/Slate/SModioBox.h"

TSharedRef<SWidget> UModioSizeBox::RebuildWidget()
{
	// Passing aspect ratio settings through in the call to Construct because that's where we set ModioBox specific
	// versions of those variables
	MySizeBox = SNew(SModioBox).MinAspectRatio(MinAspectRatio).MaxAspectRatio(MaxAspectRatio);

	if (GetChildrenCount() > 0)
	{
		Cast<USizeBoxSlot>(GetContentSlot())->BuildSlot(MySizeBox.ToSharedRef());
	}

	MySizeBox->SetHAlign(HAlign);
	MySizeBox->SetVAlign(VAlign);
	return MySizeBox.ToSharedRef();
}
