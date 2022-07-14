#pragma once
#include "Styling/SlateStyle.h"
#include "UI/BaseWidgets/ModioRichTextBlockDecorator.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "UI/Styles/ModioRichTextStyle.h"

DECLARE_DELEGATE_RetVal(const FModioRichTextStyle&, FOnGetRichTextStyle);

class SModioRichTextBlock : public SRichTextBlock
{
public:
	/// Arguments mostly taken from SRichTextBlock

	SLATE_BEGIN_ARGS(SModioRichTextBlock)
		: _Text(),
		  _HighlightText(),
		  _WrapTextAt(0.0f),
		  _AutoWrapText(false),
		  _WrappingPolicy(ETextWrappingPolicy::DefaultWrapping),
		  _Marshaller(),
		  _DecoratorStyleSet(&FCoreStyle::Get()),
		  _TextStyle(nullptr),
		  _Margin(FMargin()),
		  _LineHeightPercentage(1.0f),
		  _Justification(ETextJustify::Left),
		  _TextShapingMethod(),
		  _TextFlowDirection(),
		  _Parser(),
		  _MinDesiredWidth(),
		  _Decorators()
	{
		_Clipping = EWidgetClipping::OnDemand;
		UModioRichTextBlockDecorator* DefaultDecorator = NewObject<UModioRichTextBlockDecorator>();
		if (DefaultDecorator)
		{
			_Decorators.Add(DefaultDecorator->CreateDecorator(nullptr).ToSharedRef());
		}
	}
	/** The text displayed in this text block */
	SLATE_ATTRIBUTE(FText, Text)

	/** Highlight this text in the text block */
	SLATE_ATTRIBUTE(FText, HighlightText)

	/** Whether text wraps onto a new line when it's length exceeds this width; if this value is zero or negative, no
	 * wrapping occurs. */
	SLATE_ATTRIBUTE(float, WrapTextAt)

	/** Whether to wrap text automatically based on the widget's computed horizontal space.  IMPORTANT: Using automatic
	   wrapping can result in visual artifacts, as the the wrapped size will computed be at least one frame late!
	   Consider using WrapTextAt instead.  The initial
		desired size will not be clamped.  This works best in cases where the text block's size is not affecting other
	   widget's layout. */
	SLATE_ATTRIBUTE(bool, AutoWrapText)

	/** The wrapping policy to use */
	SLATE_ATTRIBUTE(ETextWrappingPolicy, WrappingPolicy)

	/** The marshaller used to get/set the raw text to/from the text layout. */
	SLATE_ARGUMENT(TSharedPtr<class FRichTextLayoutMarshaller>, Marshaller)

	/** Delegate used to create text layouts for this widget. If none is provided then FSlateTextLayout will be used. */
	SLATE_EVENT(FCreateSlateTextLayout, CreateSlateTextLayout)

	/** The style set used for looking up styles used by decorators*/
	SLATE_ARGUMENT(const ISlateStyle*, DecoratorStyleSet)

	/** Reference to a style defined on the default style sheet. Will override DecoratorStyleSet and TextStyle if set.
	 */
	SLATE_ATTRIBUTE(const FModioUIStyleRef*, StyleReference)

	/** The style of the text block, which dictates the default font, color, and shadow options. */
	SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)

	/** The amount of blank space left around the edges of text area. */
	SLATE_ATTRIBUTE(FMargin, Margin)

	/** The amount to scale each lines height by. */
	SLATE_ATTRIBUTE(float, LineHeightPercentage)

	/** How the text should be aligned with the margin. */
	SLATE_ATTRIBUTE(ETextJustify::Type, Justification)

	/** Which text shaping method should we use? (unset to use the default returned by GetDefaultTextShapingMethod) */
	SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)

	/** Which text flow direction should we use? (unset to use the default returned by GetDefaultTextFlowDirection) */
	SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

	/** The parser used to resolve any markup used in the provided string. */
	SLATE_ARGUMENT(TSharedPtr<class IRichTextMarkupParser>, Parser)

	/** Minimum width that this text block should be */
	SLATE_ATTRIBUTE(float, MinDesiredWidth)

	/** Any decorators that should be used while parsing the text. */
	SLATE_ARGUMENT(TArray<TSharedRef<class ITextDecorator>>, Decorators)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TAttribute<const FModioUIStyleRef*> StyleReference;

	void ReapplyStyle();
	void SetDecoratorStyleSet(TSharedPtr<FSlateStyleSet> NewStyleSet)
	{
		StyleSetOverride = NewStyleSet;
		SRichTextBlock::SetDecoratorStyleSet(NewStyleSet.Get());
	}

protected:
	TSharedPtr<FSlateStyleSet> StyleSetOverride;
	virtual bool CustomPrepass(float InScale) override;
};