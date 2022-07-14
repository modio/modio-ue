// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioPopupComboBox.h"
#include "Algo/Transform.h"
#include "UI/BaseWidgets/Slate/SModioCustomComboBox.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/BaseWidgets/Slate/SModioTableRowBase.h"
#include "UI/Styles/ModioCustomComboBoxStyle.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STableRow.h"

void UModioPopupComboBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (MyComboBox)
	{
		if (IsDesignTime())
		{
			if (bPreviewOpen)
			{
				MyComboBox->Open();
			}
			else
			{
				MyComboBox->Close();
			}
		}

		//TODO: @modio-ue4 support alternative placement modes
		if (PopupPlacement == EModioPopupPlacement::AlignBottomLeft)
		{
			MyComboBox->SetMenuPlacement(EMenuPlacement::MenuPlacement_MatchBottomLeft);
		}
		else
		{
			MyComboBox->SetMenuPlacement(EMenuPlacement::MenuPlacement_ComboBox);
		}

		const FModioWidgetBorderStyle* ActualStyle = RoundedCornerStyle.FindStyle<FModioWidgetBorderStyle>();
		MyComboBox->SetBorderStyle(ActualStyle ? *ActualStyle : TOptional<FModioWidgetBorderStyle> {});
	}
	UpdateBoundValues();
}

void UModioPopupComboBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyComboBox.Reset();
}

TSharedRef<ITableRow> UModioPopupComboBox::GenerateOptionWidget(TSharedPtr<FText> Item,
																const TSharedRef<STableViewBase>& OwnerTable)
{
	FFormatNamedArguments Args;
	if (BoundValues.Num() && BoundValues[0]->EqualTo(*Item))
	{
		Args.Add("Description", Description);
	}
	else
	{
		Args.Add("Description", FText::GetEmpty());
	}
	Args.Add("Value", *Item);
	// clang-format off
	return SNew(SModioTableRowBase<TSharedPtr<FText>>, OwnerTable)
		.Padding(ContentPadding)
		.Content()
		[
			SNew(SBox) 
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.HeightOverride(32)
			[
				SNew(SModioRichTextBlock).Text(FText::Format(ValueFormatText, Args)).StyleReference_UObject(this, &UModioPopupComboBox::GetTextStyle)
			]
		];
	// clang-format on
}

FSlateColor UModioPopupComboBox::GetButtonTextColor() const
{
	const FModioCustomComboBoxStyle* ComboStyle = ComboBoxStyle.FindStyle<FModioCustomComboBoxStyle>();
	if (ComboStyle)
	{
		const FModioTableRowStyle* RowStyle = ComboStyle->RowStyle.FindStyle<FModioTableRowStyle>();
		if (RowStyle)
		{
			return RowStyle->TextColor;
		}
	}
	return FSlateColor(FLinearColor::White);
}

const FModioUIStyleRef* UModioPopupComboBox::GetTextStyle() const
{
	return &TextStyle;
}

// TODO: use Rich Text and set the style from the ComboBoxStyle
TSharedRef<SWidget> UModioPopupComboBox::GenerateButtonContent(TSharedPtr<FText> Item)
{
	FFormatNamedArguments Args;
	Args.Add("Description", Description);
	Args.Add("Value", *Item);
	// clang-format off
	return SNew(SBox) 
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.HeightOverride(32)
		[
			SNew(SModioRichTextBlock).Text(FText::Format(LabelFormatText, Args)).StyleReference_UObject(this, &UModioPopupComboBox::GetTextStyle)
		];
	//TODO: @modio-ue4 bind the font delegate here too so we can actually change the size of the text etc
	// clang-format on
}

TSharedRef<SWidget> UModioPopupComboBox::RebuildWidget()
{
	UpdateBoundValues();
	const FModioWidgetBorderStyle* ActualStyle = RoundedCornerStyle.FindStyle<FModioWidgetBorderStyle>();
	const FModioCustomComboBoxStyle* ComboStyle = ComboBoxStyle.FindStyle<FModioCustomComboBoxStyle>();
	// EListItemAlignment
	return SAssignNew(MyComboBox, SModioCustomComboBox<TSharedPtr<FText>>)
		.bUseRoundedCorners(bUseRoundedCorners)
		.ContentPadding(ContentPadding)
		.OnGetButtonContent_UObject(this, &UModioPopupComboBox::GenerateButtonContent)
		.OnGenerateOptionWidget_UObject(this, &UModioPopupComboBox::GenerateOptionWidget)
		.OnSelectionChanged_UObject(this, &UModioPopupComboBox::OnSelectionChangedHandler)
		.ItemsSource(&BoundValues)
		.BackgroundColor(ComboStyle ? ComboStyle->BackgroundColor : FModioUIColorRef())
		.MenuBorderStyle(ActualStyle ? *ActualStyle : TOptional<FModioWidgetBorderStyle> {});
}

void UModioPopupComboBox::UpdateBoundValues()
{
	BoundValues.Empty();
	
	// Only bind values in this way if we are using a Command List
	if (CurrentEntries.MappedActions.Num() > 0)
	{
		Algo::Transform(
		CurrentEntries.MappedActions, BoundValues,
		[](const TPair<FModioUIMenuEntry, FModioUIAction> Elem) { return MakeShared<FText>(Elem.Key.MenuEntryLabel); });
	}
	else
	{
		Algo::Transform(OptionValues, BoundValues, [](const FText& InValue) { return MakeShared<FText>(InValue); });	
	}
}

void UModioPopupComboBox::SetOptionValues(const TArray<FText>& Values)
{
	OptionValues = Values;
	UpdateBoundValues();
	if (MyComboBox)
	{
		MyComboBox->Refresh();
	}
}

void UModioPopupComboBox::OnSelectionChangedHandler(TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectionType)
{
	if (OnSelectionChanged.IsBound())
	{
		OnSelectionChanged.Execute(NewSelection ? *NewSelection : FText::GetEmpty(), SelectionType);
	}

	if (OnSelectionChangedWithDelegate.IsBound())
	{
		for(auto &Entry : CurrentEntries.MappedActions)
		{
			if (Entry.Key.MenuEntryLabel.EqualTo(*NewSelection))
			{
				OnSelectionChangedWithDelegate.Execute(Entry.Value, SelectionType);		
			}
		}
		
	}
}