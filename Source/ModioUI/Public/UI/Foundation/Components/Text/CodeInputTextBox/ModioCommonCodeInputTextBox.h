/*
 *  Copyright (C) 2023 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "ModioCommonCodeInputTextBoxStyle.h"
#include "Components/Widget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Text/TextLayout.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/IVirtualKeyboardEntry.h"
#include "ModioCommonCodeInputTextBox.generated.h"

enum class EVirtualKeyboardTrigger : uint8;
enum class EVirtualKeyboardDismissAction : uint8;
class STextBlock;
class SUniformGridPanel;
class SEditableText;

/**
 * Code Input Text Box Slate widget
 */
class MODIOUI_API SModioCommonCodeInputTextBox : public SCompoundWidget
{
protected:
	/**
	 * Creates a character widget for the code input text box
	 * @param WidgetIndex The index of the widget to create
	 * @return The created widget
	 */
	virtual TSharedRef<SWidget> CreateCharacterWidget(int32 WidgetIndex);

	/**
	 * Called when the user presses a key down
	 * @param MyGeometry The geometry of the code input text box
	 * @param InKeyEvent The key event
	 * @return Whether the key event was handled
	 */
	virtual FReply OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);

	/** The style of the code input text box */
	FModioCommonCodeInputTextBoxInputStyle Style;

	/** The number of children in the code input text box */
	TOptional<ETextFlowDirection> TextFlowDirection;

	/** The brush to use for the character border */
	mutable FSlateBrush CachedCharacterBrush;

	/**
	 * Rebuilds the children of the code input text box (the characters)
	 * @param NumChildren The number of children to rebuild
	 */
	virtual void RebuildChildren(uint32 NumChildren);

	//~ Begin SWidget Interface
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual bool SupportsKeyboardFocus() const override;
	//~ End SWidget Interface

	/**
	 * Called when the text in the input field changes
	 * @param NewText The new text
	 */
	virtual void OnInputTextChanged(const FText& NewText);

	/**
	 * Gets the character at the specified index
	 * @param Index The index of the character to get
	 * @return The character at the specified index
	 */
	FText GetCharacterAtIndex(int32 Index) const;

	/**
	 * Gets the border brush for the character
	 * @return The border brush for the character
	 */
	virtual const FSlateBrush* GetCharacterBorder() const;

	/**
	 * Gets the visibility of the caret
	 * @param Index The index of the character to get the caret visibility for
	 * @return The visibility of the caret
	 */
	virtual EVisibility GetCaretVisibility(uint32 Index) const;

	/**
	 * Gets the number of characters in the code input text box
	 * @return The number of characters in the code input text box
	 */
	virtual int32 GetNumOfCharacters() const;

public:
	/** Static delegate for when the user submits the code */
	DECLARE_DELEGATE(FOnCodeSubmit);

	/**
	 * Called when the user submits the code
	 */
	FOnCodeSubmit OnCodeSubmit;

	SLATE_BEGIN_ARGS(SModioCommonCodeInputTextBox) : _NumChildren(5) {}

	/** The style of the code input text box */
	SLATE_ARGUMENT(FModioCommonCodeInputTextBoxInputStyle, Style);

	/** The number of characters to display in the code input text box */
	SLATE_ARGUMENT(int32, NumChildren);

	/** Called when the user submits the code */
	SLATE_EVENT(FOnCodeSubmit, OnCodeSubmit)

	/** Text flow direction (how text is laid out) */
	SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

	/** Additional options to be used by the virtual keyboard summoned from this widget */
	SLATE_ARGUMENT(FVirtualKeyboardOptions, VirtualKeyboardOptions)

	/** The type of event that will trigger the display of the virtual keyboard */
	SLATE_ARGUMENT(EVirtualKeyboardTrigger, VirtualKeyboardTrigger)

	/** The message action to take when the virtual keyboard is dismissed by the user */
	SLATE_ARGUMENT(EVirtualKeyboardDismissAction, VirtualKeyboardDismissAction)
	
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	/**
	 * Sets the style of the code input text box
	 * @param NewStyle The style to set
	 */
	void SetStyle(const FModioCommonCodeInputTextBoxInputStyle& NewStyle);

	/**
	 * Sets the number of children in the code input text box
	 * @param NewNumChildren The number of children to set
	 */
	void SetNumChildren(int32 NewNumChildren);

	/**
	 * Gets the code string from the code input text box
	 * @return The code string
	 */
	FString GetCodeString() const;

	/**
	 * Sets the code string in the code input text box
	 * @param NewCodeString The code string to set
	 */
	void SetCodeString(const FString& NewCodeString);

protected:
	/** Character widgets */
	TArray<TSharedPtr<STextBlock>> CharacterWidgets;

	/** The grid panel that contains the character widgets */
	TSharedPtr<SUniformGridPanel> MyCharacterGrid;

	/** The hidden input field (used to capture keyboard input) */
	TSharedPtr<SEditableText> HiddenInputField;
};

/**
 * Code Input Text Box styled using the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI_API UModioCommonCodeInputTextBox : public UWidget
{
	GENERATED_BODY()

public:
	/**
	 * Sets the style of the Code Input Text Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonCodeInputTextBoxStyle> InStyle);

protected:
	/** The style of the Code Input Text Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonCodeInputTextBoxStyle> ModioStyle;

public:
	/** Text flow direction (how text is laid out) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mod.io Common UI|Code Input")
	ETextFlowDirection TextFlowDirection = ETextFlowDirection::LeftToRight;

	/** Additional options to be used by the virtual keyboard summoned from this widget */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Mod.io Common UI|Code Input")
	FVirtualKeyboardOptions VirtualKeyboardOptions;

	/** The type of event that will trigger the display of the virtual keyboard */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Mod.io Common UI|Code Input")
	EVirtualKeyboardTrigger VirtualKeyboardTrigger;

	/** What action should be taken when the virtual keyboard is dismissed? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Mod.io Common UI|Code Input")
	EVirtualKeyboardDismissAction VirtualKeyboardDismissAction;

public:
	/**
	 * Gets the code string from the code input text box
	 * @return The code string
	 */
	UFUNCTION(BlueprintCallable, Category="Widget")
	FString GetCodeString() const;

	/**
	 * Sets the code string in the code input text box
	 * @param InCodeString The code string to set
	 */
	UFUNCTION(BlueprintCallable, Category="Widget")
	void SetCodeString(FString InCodeString);

	//~ Begin UWidget Interface
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UWidget Interface

public:
	/** The number of characters to display in the code input text box */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	int32 NumberOfCharacters = 5;

	/** Static delegate for when the user submits the code. Suitable for C++ use */
	DECLARE_MULTICAST_DELEGATE(FOnCodeSubmit);
	/** Called when the user submits the code. Suitable for C++ use */
	FOnCodeSubmit OnCodeSubmit;

	/** Dynamic delegate for when the user submits the code. Suitable for Blueprint use */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCodeSubmitDynamic);
	/** Called when the user submits the code. Suitable for Blueprint use */
	UPROPERTY(BlueprintAssignable, Category = "Modio Code Input Widget")
	FOnCodeSubmitDynamic OnCodeSubmitDynamic;

protected:
	/** Vertical box that contains the code input text box */
	TSharedPtr<SVerticalBox> MyVerticalBox;

	/** The code input text box */
	TSharedPtr<SModioCommonCodeInputTextBox> MyInput;
};
