// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidgets/ModioLogoWidget.h"

#include "ModioUISubsystem.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "UI/Styles/ModioUIStyleSet.h"

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