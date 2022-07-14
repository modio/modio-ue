// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioTabBar.h"
#include "Algo/Transform.h"
#include "PropertyPathHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/BaseWidgets/Slate/SModioTileView.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STileView.h"

TSharedRef<SWidget> UModioTabBar::RebuildWidget()
{
	UpdateBoundValues();
	return SAssignNew(MyListView, SModioTileView<TSharedPtr<FText>>)
		.ListItemsSource(&BoundValues)
		.ItemHeight(ItemSize.Y)
		.ItemWidth(ItemSize.X)
		.ScrollbarVisibility(EVisibility::Collapsed)
		.Orientation(EOrientation::Orient_Horizontal)
		.OnGenerateTile_UObject(this, &UModioTabBar::OnGenerateTabButton);
}

void UModioTabBar::ReleaseSlateResources(bool bReleaseChildren)
{
	MyListView.Reset();
	
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UModioTabBar::SynchronizeProperties()
{
	UpdateBoundValues();
	Super::SynchronizeProperties();
	MyListView->SetItemHeight(ItemSize.Y);
	MyListView->SetItemWidth(ItemSize.X);
}

TSharedRef<ITableRow> UModioTabBar::OnGenerateTabButton(TSharedPtr<FText> TabName,
														const TSharedRef<STableViewBase>& OwningTable)
{
	TSharedPtr<SButton> RowButton;
	TSharedPtr<SModioRichTextBlock> RowTextBlock;
	// clang-format off
	TSharedRef<STableRow<TSharedPtr<FText>>> TableRow = SNew(STableRow<TSharedPtr<FText>>, OwningTable)
		.ShowSelection(false)
		.Style(&TEMP_TableRowStyle)
		.Content()
		[
			SAssignNew(RowButton, SButton)
			.OnClicked_UObject(this, &UModioTabBar::OnTabClicked, TabName)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			[
				SAssignNew(RowTextBlock, SModioRichTextBlock).Text(*TabName.Get())
			]
		];
	// clang-format on

	if (GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		int ActiveTabIndex = GEngine->GetEngineSubsystem<UModioUISubsystem>()->GetActiveTabIndex();

		for(int i = 0; i < TabNames.Num(); i++)
		{
			if (!TabNames[i].EqualTo(*TabName.Get()))
				continue;
			
			if (i == ActiveTabIndex)
			{
				ApplyStyle(TabButtonStyle,RowTextBlock, RowButton);
				break;
			}

			ApplyStyle(InactiveTabButtonStyle,RowTextBlock, RowButton);
		}
		
	}
	TableRow->SetPadding(FMargin(24,24,24,24));
	
	return TableRow;
}

FReply UModioTabBar::OnTabClicked(TSharedPtr<FText> TabName)
{
	const FText Tab = TabName.IsValid() ? *TabName: FText::GetEmpty();

	for(int i = 0; i < TabNames.Num(); i++)
	{
		if (TabNames[i].EqualTo(Tab))
		{
			if (GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				GEngine->GetEngineSubsystem<UModioUISubsystem>()->SetActiveTabIndex(i);

				return FReply::Handled();
			}
		}
	}
	
	return FReply::Handled();
}

void UModioTabBar::ApplyStyle(const FModioUIStyleRef& Style, TSharedPtr<SModioRichTextBlock> RichTextBox, TSharedPtr<SButton> RowButton)
{
	const FModioButtonStyle* ResolvedButtonStyle = Style.FindStyle<FModioButtonStyle>();
	if (ResolvedButtonStyle)
	{
		const FModioRichTextStyle* ResolvedTextStyle =
			ResolvedButtonStyle->RichTextStyle.FindStyle<FModioRichTextStyle>();
		if (ResolvedTextStyle)
		{
			TSharedPtr<FSlateStyleSet> ResolvedRichTextStyleSet = ResolvedTextStyle->GetStyleSet();
			RichTextBox.Get()->SetDecoratorStyleSet(ResolvedRichTextStyleSet);
			if (ResolvedRichTextStyleSet->HasWidgetStyle<FTextBlockStyle>(FName("Default")))
			{
				RichTextBox.Get()->SetTextStyle(ResolvedRichTextStyleSet->GetWidgetStyle<FTextBlockStyle>(FName("Default")));
			}
			RichTextBox.Get()->ReapplyStyle();
		}
		
		RowButton->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
	}
}


void UModioTabBar::UpdateBoundValues()
{
	BoundValues.Empty();
	Algo::Transform(TabNames, BoundValues, [](const FText& InValue) { return MakeShared<FText>(InValue); });
}

void UModioTabBar::PostLoad()
{
	Super::PostLoad();
	for (const TPair<FName, FModioUIColorRef>& SerializedColor : SerializedColors)
	{
		TArray<FString> PropertyPathElems;
		PropertyPathHelpers::SetPropertyValue(this, SerializedColor.Key.ToString(),
											  SerializedColor.Value.ResolveReference());
	}
}
