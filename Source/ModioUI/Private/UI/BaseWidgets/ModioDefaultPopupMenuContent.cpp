/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioDefaultPopupMenuContent.h"
#include "Algo/Transform.h"
#include "Slate/SRetainerWidget.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/BaseWidgets/Slate/SModioTableRowBase.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

void UModioDefaultPopupMenuContent::NativeConstruct() 
{
	Super::NativeConstruct();

	bIsFocusable = true;
}
void UModioDefaultPopupMenuContent::SelectListIndex(int index) 
{
	if (!CurrentMenuEntries.IsValidIndex(index)) 
	{
		return;
	}

	MyList->SetSelection(CurrentMenuEntries[index], ESelectInfo::OnKeyPress);
}

void UModioDefaultPopupMenuContent::SelectCurrentIndex() 
{
	TArray<TSharedPtr<FModioUIMenuEntry>> items = MyList->GetSelectedItems();
	if (!items.IsValidIndex(0))
	{
		return;
	}
	MyList->SetSelection(items[0], ESelectInfo::OnKeyPress);
}

FOptionalSize UModioDefaultPopupMenuContent::GetDesiredItemWidth() const
{
	return DesiredSizeFromParentMenu.X;
}

FOptionalSize UModioDefaultPopupMenuContent::GetDesiredMenuHeight() const
{
	return CurrentMenuEntries.Num() > 0 ? CurrentMenuEntries.Num() * GetDesiredItemHeight() : GetDesiredItemHeight(); 
}

float UModioDefaultPopupMenuContent::GetDesiredItemHeight() const
{
	return DesiredSizeFromParentMenu.Y;
}

void UModioDefaultPopupMenuContent::SynchronizeProperties()
{
	UWidget::SynchronizeProperties();
	if (MyBorder)
	{
		if (const FModioWidgetBorderStyle* ResolvedStyle = BorderStyle.FindStyle<FModioWidgetBorderStyle>())
		{
			if (UMaterialInstanceDynamic* CurrentMaterial = MyBorder->GetEffectMaterial())
			{
				ApplyModioBorderStyle(CurrentMaterial, ResolvedStyle);
				if (ResolvedStyle->bRelativeRadius)
				{
					if (MyList)
					{
						CurrentMaterial->SetScalarParameterValue(
							"Radius",
							ResolvedStyle->CornerRadius /
								(MyList->GetNumItemsBeingObserved() ? MyList->GetNumItemsBeingObserved() : 1));
					}
				}
			}
		}

		MyBorder->RequestRender();
	}
	UpdateBoundValues();
}

void UModioDefaultPopupMenuContent::UpdateBoundValues()
{
	CurrentMenuEntries.Empty();
	Algo::Transform(
		CurrentCommandList.MappedActions, CurrentMenuEntries,
		[](const TPair<FModioUIMenuEntry, FModioUIAction> Elem) { return MakeShared<FModioUIMenuEntry>(Elem.Key); });
	if (MyList)
	{
		MyList->SetListItemsSource(CurrentMenuEntries);
		MyList->RequestListRefresh();
		MyList->RebuildList();
	}
	if (MyBorder)
	{
		MyBorder->RequestRender();
	}
	if (MyBox)
	{
		MyBox->Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
	}
}

void UModioDefaultPopupMenuContent::OnEntrySelected(TSharedPtr<FModioUIMenuEntry> SelectedEntry,
													ESelectInfo::Type SelectionType)
{

	if (SelectionType == ESelectInfo::Type::OnNavigation || SelectionType == ESelectInfo::Type::Direct) 
	{
		FSlateApplication::Get().PlaySound(ItemHoverSound);
		return;
	}
	NativeGetContentCloseDelegate().ExecuteIfBound();

	for (auto It = CurrentCommandList.MappedActions.CreateIterator(); It; ++It)
	{
		if (It.Key().MenuEntryLabel.EqualToCaseIgnored(SelectedEntry.Get()->MenuEntryLabel))
		{
			const FModioUIAction& Action = It.Value();
			if (Action.CanExecuteAction.IsBound())
			{
				if (Action.CanExecuteAction.Execute())
				{
					Action.ExecuteAction.ExecuteIfBound();
					FSlateApplication::Get().PlaySound(ItemPressedSound); 
				}
			}
			else
			{
				Action.ExecuteAction.ExecuteIfBound();
				FSlateApplication::Get().PlaySound(ItemPressedSound);
			}
		}
	}
}

TSharedRef<ITableRow> UModioDefaultPopupMenuContent::GenerateMenuEntryWidget(
	TSharedPtr<FModioUIMenuEntry> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	// clang-format off

	auto Row = SNew(SModioTableRowBase<TSharedPtr<FModioUIMenuEntry>>, OwnerTable)
	.Style(FModioUIStyleRef{"DefaultPopupRowStyle"})
	.Content()
	[
		SNew(SVerticalBox) 
		+SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(FMargin(20,8)) //This really should be fetched from the style of the button instead
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
            .Padding(0.f)
			.Content()
            [    
				SNew(SModioRichTextBlock).Text(Item->MenuEntryLabel).StyleReference(&TextStyle)
			]
		]
	];
	// clang-format on

	Row->SetOnMouseEnter(
		FNoReplyPointerEventHandler::CreateUObject(this, &UModioDefaultPopupMenuContent::PlayHoveredSound));

	return Row;
}

void UModioDefaultPopupMenuContent::NativeSetMenuEntries(const struct FModioUIMenuCommandList& Entries)
{
	CurrentCommandList = Entries;
	UpdateBoundValues();
	if (const FModioWidgetBorderStyle* ResolvedStyle = BorderStyle.FindStyle<FModioWidgetBorderStyle>())
	{
		if (ResolvedStyle->bRelativeRadius)
		{
			if (MyBorder)
			{
				if (UMaterialInstanceDynamic* CurrentMaterial = MyBorder->GetEffectMaterial())
				{
					if (MyList)
						CurrentMaterial->SetScalarParameterValue(
							"Radius",
							ResolvedStyle->CornerRadius /
								(MyList->GetNumItemsBeingObserved() ? MyList->GetNumItemsBeingObserved() : 1));
				}
			}
		}
	}
}

void UModioDefaultPopupMenuContent::NativeSetDesiredSize(const FVector2D& DesiredSize)
{
	DesiredSizeFromParentMenu = DesiredSize;
}

void UModioDefaultPopupMenuContent::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyBorder.Reset();
	MyList.Reset();
	MyBox.Reset();
}

TSharedRef<SWidget> UModioDefaultPopupMenuContent::RebuildWidget()
{
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		FModioUIMenuCommandList DesignPreviewCommands;
		for (int32 PreviewItemIndex = 0; PreviewItemIndex < PreviewItemCount; PreviewItemIndex++)
		{
			FModioUIMenuEntry NewEntry;
			NewEntry.MenuEntryLabel = FText::Format(FTextFormat(FText::FromString("Menu Entry {0}")), PreviewItemIndex);

			DesignPreviewCommands.MappedActions.Add(NewEntry, FModioUIAction {});
		}
		NativeSetMenuEntries(DesignPreviewCommands);
	}

#endif

	// clang-format off
	MyBox = SNew(SBox)
		.WidthOverride_UObject(this, &UModioDefaultPopupMenuContent::GetDesiredItemWidth)
		.HeightOverride_UObject(this, &UModioDefaultPopupMenuContent::GetDesiredMenuHeight)
		[
			SAssignNew(MyBorder, SRetainerWidget)
			.Phase(0)
			.PhaseCount(1)
			.RenderOnPhase(true)
			.Content()
			[
				SAssignNew(MyList, SModioListView<TSharedPtr<FModioUIMenuEntry>>)
								.ListItemsSource(&CurrentMenuEntries)
								.SelectionMode(ESelectionMode::Single)
								.ItemHeight(DesiredSizeFromParentMenu.Y)
								.OnSelectionChanged_UObject(this, &UModioDefaultPopupMenuContent::OnEntrySelected)
								.OnGenerateRow_UObject(this, &UModioDefaultPopupMenuContent::GenerateMenuEntryWidget)
								.AllowOverscroll(EAllowOverscroll::No)
								.HandleDirectionalNavigation(true)
								.HandleGamepadEvents(true)
								.IsFocusable(true)
			]
		];
		MyList->SetScrollbarVisibility(EVisibility::Collapsed);
	// clang-format on

	MyBorder->SetRetainedRendering(true);
	// pull this from the border style instead?
	if (UMaterialInterface* ResolvedMaterial = RetainerMaterial.ResolveReference())
	{
		MyBorder->SetEffectMaterial(ResolvedMaterial);
		MyBorder->SetTextureParameter(FName("WidgetTexture"));
	}
	MyList->RebuildList();
	return MyBox.ToSharedRef();
}

FReply UModioDefaultPopupMenuContent::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FSlateApplication::Get().SetUserFocus(0, MyList, EFocusCause::SetDirectly);
	if (CurrentMenuEntries.IsValidIndex(0)) 
	{
		MyList->SetSelection(CurrentMenuEntries[0]);
	}
	
	return FReply::Handled();
}