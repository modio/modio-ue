// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidgets/ModioDefaultPopupMenuContent.h"
#include "Algo/Transform.h"
#include "Slate/SRetainerWidget.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/BaseWidgets/Slate/SModioTableRowBase.h"
#include "UI/Styles/ModioWidgetBorderStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

FOptionalSize UModioDefaultPopupMenuContent::GetDesiredItemWidth() const
{
	return DesiredSizeFromParentMenu.X;
}

FOptionalSize UModioDefaultPopupMenuContent::GetDesiredMenuHeight() const
{
	return CurrentMenuEntries.Num() * GetDesiredItemHeight();
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
	NativeGetContentCloseDelegate().ExecuteIfBound();

	for (auto It = CurrentCommandList.MappedActions.CreateIterator(); ++It;)
	{
		if (It.Key().MenuEntryLabel.EqualTo(SelectedEntry.Get()->MenuEntryLabel))
		{
			const FModioUIAction& Action = It.Value();
			if (Action.CanExecuteAction.IsBound())
			{
				if (Action.CanExecuteAction.Execute())
				{
					Action.ExecuteAction.ExecuteIfBound();
				}
			}
			else
			{
				Action.ExecuteAction.ExecuteIfBound();
			}
		}
	}
}

TSharedRef<ITableRow> UModioDefaultPopupMenuContent::GenerateMenuEntryWidget(
	TSharedPtr<FModioUIMenuEntry> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	// clang-format off
	return SNew(SModioTableRowBase<TSharedPtr<FModioUIMenuEntry>>, OwnerTable)
	.Style(FModioUIStyleRef{"DefaultPopupRowStyle"})
	.Content()
	[
		SNew(SVerticalBox) 
		+SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.Padding(FMargin(20,8)) //This really should be fetched from the style of the button instead
		[
			SNew(SModioRichTextBlock).Text(Item->MenuEntryLabel).StyleReference_UObject(this, &UModioDefaultPopupMenuContent::GetTextStyle)
		]
	];
	// clang-format on
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
								.ItemHeight_UObject(this, &UModioDefaultPopupMenuContent::GetDesiredItemHeight)
								.OnSelectionChanged_UObject(this, &UModioDefaultPopupMenuContent::OnEntrySelected)
								.OnGenerateRow_UObject(this, &UModioDefaultPopupMenuContent::GenerateMenuEntryWidget)
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
	MyBorder->RequestRender();
	return MyBox.ToSharedRef();
}
