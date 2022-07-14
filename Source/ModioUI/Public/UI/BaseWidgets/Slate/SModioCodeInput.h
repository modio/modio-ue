#pragma once

#include "Brushes/SlateNoResource.h"
#include "Input/Reply.h"
#include "UI/Styles/ModioCodeInputStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class SModioCodeInput : public SCompoundWidget
{
protected:
	TArray<TSharedPtr<class STextBlock>> CharacterWidgets;
	TSharedPtr<class SUniformGridPanel> MyCharacterGrid;
	TSharedPtr<class SEditableText> HiddenInputField;
	virtual TSharedRef<SWidget> CreateCharacterWidget(int32 WidgetIndex);
	FReply OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
	const FModioCodeInputStyle* Style;
	FString CurrentText;
	virtual void RebuildChildren(uint32 NumChildren);
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	void OnInputTextChanged(const FText& NewText);
	FText GetCharacterAtIndex(int32 Index) const;
	FSlateNoResource NullBrush;
	FSlateBrush FakeCaretBrush;
	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	};
	virtual const FSlateBrush* GetCharacterBorder() const;
	virtual EVisibility GetCaretVisibility(uint32 Index) const;
	// Grrr getters always const
	mutable FSlateBrush CachedCharacterBrush;

public:
	SLATE_BEGIN_ARGS(SModioCodeInput) : _Style(&FModioCodeInputStyle::GetDefault()), _NumChildren(5) {}
	SLATE_ARGUMENT(const FModioCodeInputStyle*, Style);
	SLATE_ARGUMENT(int32, NumChildren);
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	void SetStyle(const FModioCodeInputStyle* NewStyle);
	void SetNumChildren(int32 NewNumChildren);
	FString GetCodeString();
};