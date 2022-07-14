#pragma once
#include "Widgets/DeclarativeSyntaxSupport.h"
template<typename OptionType>
class SModioComboBox : public SComboBox<OptionType>
{
public:
	using FOnSelectionChanged = typename SComboBox<OptionType>::FOnSelectionChanged;
	using FOnGenerateWidget = typename SComboBox<OptionType>::FOnGenerateWidget;
	using NullableOptionType = typename SComboBox<OptionType>::NullableOptionType;

	SLATE_BEGIN_ARGS(SModioComboBox)
		: _Content(),
		  _ComboBoxStyle(&FCoreStyle::Get().GetWidgetStyle<FComboBoxStyle>("ComboBox")),
		  _ButtonStyle(nullptr),
		  _ItemStyle(&FCoreStyle::Get().GetWidgetStyle<FTableRowStyle>("TableView.Row")),
		  _ContentPadding(FMargin(4.0, 2.0)),
		  _ForegroundColor(FCoreStyle::Get().GetSlateColor("InvertedForeground")),
		  _OptionsSource(),
		  _OnSelectionChanged(),
		  _OnGenerateWidget(),
		  _InitiallySelectedItem(nullptr),
		  _Method(),
		  _MaxListHeight(450.0f),
		  _HasDownArrow(true),
		  _EnableGamepadNavigationMode(false),
		  _IsFocusable(true)
	{}

	/** Slot for this button's content (optional) */
	SLATE_DEFAULT_SLOT(FArguments, Content)

	SLATE_STYLE_ARGUMENT(FComboBoxStyle, ComboBoxStyle)

	/** The visual style of the button part of the combo box (overrides ComboBoxStyle) */
	SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)

	SLATE_STYLE_ARGUMENT(FTableRowStyle, ItemStyle)

	SLATE_ATTRIBUTE(FMargin, ContentPadding)
	SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)

	SLATE_ARGUMENT(const TArray<OptionType>*, OptionsSource)
	SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)
	SLATE_EVENT(FOnGenerateWidget, OnGenerateWidget)

	/** Called when combo box is opened, before list is actually created */
	SLATE_EVENT(FOnComboBoxOpening, OnComboBoxOpening)

	/** The custom scrollbar to use in the ListView */
	SLATE_ARGUMENT(TSharedPtr<SScrollBar>, CustomScrollbar)

	/** The option that should be selected when the combo box is first created */
	SLATE_ARGUMENT(NullableOptionType, InitiallySelectedItem)

	SLATE_ARGUMENT(TOptional<EPopupMethod>, Method)

	/** The max height of the combo box menu */
	SLATE_ARGUMENT(float, MaxListHeight)

	/** The sound to play when the button is pressed (overrides ComboBoxStyle) */
	SLATE_ARGUMENT(TOptional<FSlateSound>, PressedSoundOverride)

	/** The sound to play when the selection changes (overrides ComboBoxStyle) */
	SLATE_ARGUMENT(TOptional<FSlateSound>, SelectionChangeSoundOverride)

	/**
	 * When false, the down arrow is not generated and it is up to the API consumer
	 * to make their own visual hint that this is a drop down.
	 */
	SLATE_ARGUMENT(bool, HasDownArrow)

	/**
	 *  When false, directional keys will change the selection. When true, ComboBox
	 *	must be activated and will only capture arrow input while activated.
	 */
	SLATE_ARGUMENT(bool, EnableGamepadNavigationMode)

	/** When true, allows the combo box to receive keyboard focus */
	SLATE_ARGUMENT(bool, IsFocusable)

	SLATE_ARGUMENT(EMenuPlacement, Placement)

	SLATE_END_ARGS()

	/**
	 * Construct the widget from a declaration
	 *
	 * @param InArgs   Declaration from which to construct the combo box
	 */
	void Construct(const FArguments& InArgs)
	{
		// this needs to copy the implementation rather than invoke directly, so we can inject another object than
		// STableRow
		SComboBox<OptionType>::Construct(typename SComboBox<OptionType>::FArguments()
											 .ComboBoxStyle(InArgs._ComboBoxStyle)
											 .ButtonStyle(InArgs._ButtonStyle)
											 .ItemStyle(InArgs._ItemStyle)
											 .ContentPadding(InArgs._ContentPadding)
											 .ForegroundColor(InArgs._ForegroundColor)
											 .OptionsSource(InArgs._OptionsSource)
											 .OnSelectionChanged(InArgs._OnSelectionChanged)
											 .OnGenerateWidget(InArgs._OnGenerateWidget)
											 .OnComboBoxOpening(InArgs._OnComboBoxOpening)
											 .CustomScrollbar(InArgs._CustomScrollbar)
											 .InitiallySelectedItem(InArgs._InitiallySelectedItem)
											 .Method(InArgs._Method)
											 .MaxListHeight(InArgs._MaxListHeight)
											 .PressedSoundOverride(InArgs._PressedSoundOverride)
											 .SelectionChangeSoundOverride(InArgs._SelectionChangeSoundOverride)
											 .HasDownArrow(InArgs._HasDownArrow)
											 .EnableGamepadNavigationMode(InArgs._EnableGamepadNavigationMode)
											 .IsFocusable(InArgs._IsFocusable)[InArgs._Content.Widget]);
		SMenuAnchor::Placement = InArgs._Placement;
	}
	TSharedPtr<SWidget> GetUnderlyingMenuContent()
	{
		if (SMenuAnchor::OnGetMenuContent.IsBound())
		{
			SComboButton::SetMenuContent(SMenuAnchor::OnGetMenuContent.Execute());
		}
		return SMenuAnchor::MenuContent;
	}
};