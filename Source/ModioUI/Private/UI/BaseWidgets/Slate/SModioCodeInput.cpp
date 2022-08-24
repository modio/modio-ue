#include "UI/BaseWidgets/Slate/SModioCodeInput.h"
#include "Brushes/SlateNoResource.h"
#include "Core/ModioUIHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Misc/EngineVersionComparison.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SUniformGridPanel.h"

TSharedRef<SWidget> SModioCodeInput::CreateCharacterWidget(int32 WidgetIndex)
{
	TSharedRef<STextBlock> Widget = SNew(STextBlock)
										.Justification(ETextJustify::Center)
										.TextStyle(Style)
										.MinDesiredWidth(50)
										.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateSP(
											this, &SModioCodeInput::GetCharacterAtIndex, WidgetIndex)));
	CharacterWidgets.Add(Widget);
	return Widget;
}
FReply SModioCodeInput::OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Left || InKeyEvent.GetKey() == EKeys::Right || InKeyEvent.GetKey() == EKeys::Tab)
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
void SModioCodeInput::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(MyCharacterGrid, SUniformGridPanel)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0,0,0,24))
		[
			SNew(SBox)
			.WidthOverride(1)
			.HeightOverride(1)
			[
				SAssignNew(HiddenInputField, SEditableText)
				.CaretImage(&NullBrush)
				.OnKeyDownHandler(this, &SModioCodeInput::OnKeyDownHandler)
				.OnTextChanged(this, &SModioCodeInput::OnInputTextChanged)
			]
		]
	];
	// clang-format on
	Style = InArgs._Style;
	if (Style)
	{
		MyCharacterGrid->SetSlotPadding(Style->CharacterSpacing);
	}
	RebuildChildren(InArgs._NumChildren);
}
void SModioCodeInput::OnInputTextChanged(const FText& NewText)
{
	FString TruncatedText = FString(NewText.ToString().Mid(0, 5)).ToUpper();
	if (CurrentText != TruncatedText)

	{
		CurrentText = TruncatedText;
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}
	else if (NewText.ToString().Len() > 5)
	{
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}
}

FText SModioCodeInput::GetCharacterAtIndex(int32 Index) const
{
	if (Index < CurrentText.Len())
	{
		return FText::FromString(CurrentText.Mid(Index, 1));
	}
	return {};
}

const FSlateBrush* SModioCodeInput::GetCharacterBorder() const
{
	CachedCharacterBrush = Style->CharacterBrush;
	CachedCharacterBrush.Margin = FMargin(0, 0, 0, 3);
	if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
	{
		CachedCharacterBrush.TintColor = Style->ColorWhenFocused.ResolveReference();
	}
	return &CachedCharacterBrush;
}

EVisibility SModioCodeInput::GetCaretVisibility(uint32 Index) const
{
	if (Index == CurrentText.Len())
	{
		if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
		{
			return EVisibility::Visible;
		}
	}
	return EVisibility::Hidden;
}

void SModioCodeInput::RebuildChildren(uint32 NumChildren)
{
	FakeCaretBrush.SetResourceObject(Style->FakeCaretMaterial.ResolveReference());
	FakeCaretBrush.TintColor = FModioUIColorRef(FName("Accent")).ResolveReference();
	FakeCaretBrush.SetImageSize(FVector2D(45, 45));
	FakeCaretBrush.DrawAs = ESlateBrushDrawType::Image;
	FakeCaretBrush.ImageType = ESlateBrushImageType::FullColor;

	CharacterWidgets.Empty();
	MyCharacterGrid->ClearChildren();
	// Display widgets need to be looking at the reverse index when we are in a culture which is RTL
	if (ComputeFlowDirection() == EFlowDirection::RightToLeft)
	{
		for (int32 ChildIndex = NumChildren - 1; ChildIndex >= 0; ChildIndex--)
		{
			SUniformGridPanel::FSlot& CurrentSlot = *ModioUIHelpers::AddSlot(MyCharacterGrid.Get(), ChildIndex, 0);
			ModioUIHelpers::SetHorizontalAlignment(CurrentSlot, HAlign_Center);
			ModioUIHelpers::SetVerticalAlignment(CurrentSlot, VAlign_Center);
			CurrentSlot.AttachWidget(SNew(SBorder).BorderImage(
				this, &SModioCodeInput::GetCharacterBorder)[CreateCharacterWidget(ChildIndex)]);
		}
	}
	else
	{
		for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ChildIndex++)
		{
			SUniformGridPanel::FSlot& CurrentSlot = *ModioUIHelpers::AddSlot(MyCharacterGrid.Get(), ChildIndex, 0);
			ModioUIHelpers::SetHorizontalAlignment(CurrentSlot, HAlign_Center);
			ModioUIHelpers::SetVerticalAlignment(CurrentSlot, VAlign_Center);
			CurrentSlot.AttachWidget(
				SNew(SBorder).BorderImage(this, &SModioCodeInput::GetCharacterBorder)
					[SNew(SOverlay) +
					 SOverlay::Slot()[SNew(SImage)
										  .Image(&FakeCaretBrush)
										  .Visibility(this, &SModioCodeInput::GetCaretVisibility, ChildIndex)] +
					 SOverlay::Slot()[CreateCharacterWidget(ChildIndex)]]);
		}
	}
	Invalidate(EInvalidateWidgetReason::Layout);
}

FReply SModioCodeInput::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FSlateApplication::Get().SetAllUserFocus(HiddenInputField);
	return FReply::Handled();
}

void SModioCodeInput::SetStyle(const FModioCodeInputStyle* NewStyle)
{
	Style = NewStyle;
	if (MyCharacterGrid && Style)
	{
		MyCharacterGrid->SetSlotPadding(Style->CharacterSpacing);
	}

	for (const TSharedPtr<STextBlock>& Child : CharacterWidgets)
	{
		if (Child)
		{
			Child->SetTextStyle(Style);
		}
	}
	Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
}

void SModioCodeInput::SetNumChildren(int32 NewNumChildren)
{
	if (NewNumChildren != CharacterWidgets.Num())
	{
		RebuildChildren(NewNumChildren);
	}
}

FString SModioCodeInput::GetCodeString()
{
	return CurrentText;
}

/*
TSharedPtr<SObjectWidget> SafeGCWidget = MyGCWidget.Pin();
	if ( SafeGCWidget.IsValid() )
	{
		const bool bDecendantNewlyFocused = NewWidgetPath.ContainsWidget(SafeGCWidget.ToSharedRef());
		if ( bDecendantNewlyFocused )
		{
			const bool bDecendantPreviouslyFocused = PreviousFocusPath.ContainsWidget(SafeGCWidget.ToSharedRef());
			if ( !bDecendantPreviouslyFocused )
			{
				NativeOnAddedToFocusPath( InFocusEvent );
			}
		}
		else
		{
			NativeOnRemovedFromFocusPath( InFocusEvent );
		}
	}

*/