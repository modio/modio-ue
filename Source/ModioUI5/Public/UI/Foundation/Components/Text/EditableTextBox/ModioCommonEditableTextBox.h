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
#include "Components/EditableTextBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Templates/SubclassOf.h"
#include "Styling/SlateTypes.h"
#include "Templates/SharedPointer.h"
#include "Layout/WidgetPath.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "ModioCommonEditableTextBox.generated.h"

class UModioCommonEditableTextBoxStyle;
class SModioCommonEditableTextBox;

class MODIOUI5_API SModioCommonFocusableVerticalBox : public SVerticalBox
{
	static TArray<TSharedRef<SWidget>> FindNestedChildrenIf(TSharedRef<SWidget> ParentWidget, TFunctionRef<bool(TSharedRef<const SWidget>)> ComparePredicate)
	{
		TArray<TSharedRef<SWidget>> FoundChildren;
		ParentWidget->GetChildren()->ForEachWidget([&ComparePredicate, &FoundChildren](SWidget& ChildWidget) {
			TSharedRef<SWidget> ChildWidgetRef = SharedThis(&ChildWidget);
			if (ComparePredicate(ChildWidgetRef))
			{
				FoundChildren.Add(ChildWidgetRef);
			}
			FoundChildren.Append(FindNestedChildrenIf(ChildWidgetRef, ComparePredicate));
		});
		return FoundChildren;
	}
	
	virtual bool SupportsKeyboardFocus() const override
	{
		const bool bAnyChildHasKeyboardFocus = FindNestedChildrenIf(SharedThis(const_cast<SModioCommonFocusableVerticalBox*>(this)), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->SupportsKeyboardFocus();
		}).Num() > 0;
		return bAnyChildHasKeyboardFocus;
	}
	virtual bool HasKeyboardFocus() const override
	{
		const bool bAnyChildHasKeyboardFocus = FindNestedChildrenIf(SharedThis(const_cast<SModioCommonFocusableVerticalBox*>(this)), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->HasKeyboardFocus();
		}).Num() > 0;
		return bAnyChildHasKeyboardFocus;
	}
	
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		TArray<TSharedRef<SWidget>> FoundChildren = FindNestedChildrenIf(SharedThis(this), [](TSharedRef<const SWidget> ChildWidget) {
			return ChildWidget->SupportsKeyboardFocus();
		});

		for (TSharedRef<SWidget> ChildWidget : FoundChildren)
		{
			FWidgetPath WidgetToFocusPath;
			FSlateApplication::Get().GeneratePathToWidgetUnchecked(ChildWidget, WidgetToFocusPath);
			FSlateApplication::Get().SetKeyboardFocus(WidgetToFocusPath, InFocusEvent.GetCause());
		}
		return FReply::Handled();
	}
};

class MODIOUI5_API SModioCommonEditableTextBox : public SEditableTextBox
{
public:
	virtual bool SupportsKeyboardFocus() const override { return true; }
};

/**
 * @brief Editable Text Box styled using the Mod.io Common UI styling system
 */
UCLASS(ClassGroup = "UI", meta = (Category = "Mod.io Common UI"))
class MODIOUI5_API UModioCommonEditableTextBox : public UEditableTextBox
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the style of the Editable Text Box within the Mod.io Common UI styling system
	 * @param InStyle The style to set
	 */
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Style")
	void SetStyle(UPARAM(DisplayName = "Style") TSubclassOf<UModioCommonEditableTextBoxStyle> InStyle);

protected:
	/** @brief The style of the Editable Text Box within the Mod.io Common UI styling system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), DisplayName = "Style", Category = "Mod.io Common UI")
	TSubclassOf<UModioCommonEditableTextBoxStyle> ModioStyle;

public:
	UFUNCTION(BlueprintCallable, Category = "Mod.io Common UI|Editable Text Box")
	bool IsTextBoxFocused() const;

	/** Called whenever the text is changed programmatically or interactively by the user */
	UPROPERTY(BlueprintAssignable, Category="Widget Event", meta=(DisplayName="OnTextChanged (Multi-Line Text Box)"))
	FOnEditableTextBoxChangedEvent OnEditableTextChanged;
	
public:
	virtual void SynchronizeProperties() override;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	const FSlateBrush* GetHintBrush() const;
	EVisibility GetHintBrushVisibility() const;

	virtual bool HasValidationError() const { return false; }
	
	const FSlateBrush* GetBorderImage() const;
	virtual void HandleOnTextChanged(const FText& InText) override;

	TSharedPtr<SVerticalBox> MyVerticalBox;
};
