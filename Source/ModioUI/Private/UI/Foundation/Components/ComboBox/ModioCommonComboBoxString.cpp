/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Foundation/Components/ComboBox/ModioCommonComboBoxString.h"

#include "UI/Foundation/Components/ComboBox/ModioCommonComboBoxStringStyle.h"
#include "Misc/EngineVersionComparison.h"

void UModioCommonComboBoxString::SetStyle(TSubclassOf<UModioCommonComboBoxStringStyle> InStyle)
{
	if (InStyle && InStyle != ModioStyle)
	{
		ModioStyle = InStyle;
		SynchronizeProperties();
	}
}

void UModioCommonComboBoxString::SynchronizeProperties()
{
	if (ModioStyle)
	{
		if (UModioCommonComboBoxStringStyle* StyleCDO = ModioStyle.GetDefaultObject())
		{
#if UE_VERSION_NEWER_THAN(5, 2, 0)
			SetWidgetStyle(StyleCDO->WidgetStyle);
#else
			WidgetStyle = StyleCDO->WidgetStyle;
#endif
		}
	}
	Super::SynchronizeProperties();
}

TSharedRef<SWidget> UModioCommonComboBoxString::RebuildWidget()
{
	UModioCommonComboBoxStringStyle* StyleCDO = ModioStyle.GetDefaultObject();
	if (!StyleCDO)
	{
		return Super::RebuildWidget();
	}
	
	MyComboBox = SNew(SComboBox<TSharedPtr<FString>>)
						  .ComboBoxStyle(&StyleCDO->WidgetStyle)
						  .ItemStyle(&StyleCDO->ItemStyle)
						  .ForegroundColor(StyleCDO->ForegroundColor)
						  .OptionsSource(&Options)
						  .InitiallySelectedItem(CurrentOptionPtr)
						  .ContentPadding(StyleCDO->ContentPadding)
						  .MaxListHeight(StyleCDO->MaxListHeight)
						  .HasDownArrow(StyleCDO->HasDownArrow)
						  .EnableGamepadNavigationMode(StyleCDO->EnableGamepadNavigationMode)
						  .OnGenerateWidget(SComboBox<TSharedPtr<FString>>::FOnGenerateWidget::CreateUObject(
							  this, &UModioCommonComboBoxString::HandleGenerateWidget))
						  .OnSelectionChanged(BIND_UOBJECT_DELEGATE(
							  SComboBox<TSharedPtr<FString>>::FOnSelectionChanged, HandleSelectionChanged))
						  .OnComboBoxOpening(BIND_UOBJECT_DELEGATE(FOnComboBoxOpening, HandleOpening))
						  /*.Placement(Placement)*/
						  /*.Method(bCreateNewWindow ? EPopupMethod::CreateNewWindow : EPopupMethod::UseCurrentWindow)*/
						  .IsFocusable(
#if UE_VERSION_NEWER_THAN(5, 2, 0)
						 IsFocusable()
#else
						 bIsFocusable
#endif	
						  )[SAssignNew(ComboBoxContent, SBox)];

	return MyComboBox.ToSharedRef();
}

TSharedRef<SWidget> UModioCommonComboBoxString::HandleGenerateWidget(TSharedPtr<FString> Item) const
{
	FString StringItem = Item.IsValid() ? *Item : FString();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if (OnGenerateWidgetEvent.IsBound())
	{
		UWidget* Widget = OnGenerateWidgetEvent.Execute(StringItem);
		if (Widget != nullptr)
		{
			return Widget->TakeWidget();
		}
	}

	// If a row wasn't generated just create the default one, a simple text block of the item's name.
	return SNew(SBox).Padding(
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		GetContentPadding()
#else
		ContentPadding
#endif	
	)[SNew(STextBlock).Text(FText::FromString(StringItem)).Font(
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		GetFont()
#else
		Font
#endif
	)];
}
