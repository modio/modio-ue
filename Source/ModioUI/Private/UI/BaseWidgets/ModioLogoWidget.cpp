/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioLogoWidget.h"

#include "ModioUISubsystem.h"
#include "UI/Styles/ModioUIStyleSet.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void UModioLogoWidget::SynchronizeProperties()
{
	UWidget::SynchronizeProperties();
}

TSharedRef<SWidget> UModioLogoWidget::RebuildWidget()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		UModioUIStyleSet* DefaultStyleSet = Subsystem->GetDefaultStyleSet();
		if (DefaultStyleSet)
		{
			const TSharedPtr<FSlateBrush> CogBrush =
				DefaultStyleSet->GetMaterialBrush(FName("CogGlyphMaterial"), {}, FVector2D(64, 64));

			const FTextBlockStyle* const ResolvedStyle = StyleRef.FindStyle<FTextBlockStyle>();

			// clang-format off
			return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
			[
				SNew(SImage).Image(CogBrush.Get())
			]
			+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
			[
				SNew(STextBlock).Text(FText::FromString("mod.io"))
				.TextStyle(ResolvedStyle)
			];
			// clang-format on
		}
	}

	return SAssignNew(MyImage, SImage);
}