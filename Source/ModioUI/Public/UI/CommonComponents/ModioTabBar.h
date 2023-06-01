/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Views/STileView.h"

#include "ModioTabBar.generated.h"

/**
* Modio UI element that represents a tab bar, in other words,
* a list of horizontal elements with names and selected properties
**/
UCLASS(meta = (ColorSerialize = "SerializedColors"))
class MODIOUI_API UModioTabBar : public UWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

	TSharedRef<class ITableRow> OnGenerateTabButton(TSharedPtr<FText> TabName,
													const TSharedRef<class STableViewBase>& OwningTable);
	void ApplyStyle(const FModioUIStyleRef& ModioUIStyle, TSharedPtr<SModioRichTextBlock> RichTextBox,
					TSharedPtr<SButton> RowButton);
	FReply OnTabClicked(TSharedPtr<FText> TabName);

	void UpdateBoundValues();

	TArray<TSharedPtr<FText>> BoundValues;
	TArray<TSharedPtr<SButton>> GeneratedButtons;
	TArray<TSharedPtr<SModioRichTextBlock>> GeneratedButtonTexts;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	TArray<FText> TabNames;
	TSharedPtr<STileView<TSharedPtr<FText>>> MyListView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FTableRowStyle TEMP_TableRowStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category = "Widgets")
	FModioUIStyleRef TabButtonStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category = "Widgets")
	FModioUIStyleRef InactiveTabButtonStyle;

	/// @brief Style for the tab which is selected but not hovered/focused
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets")
	FSlateBrush SelectedButtonBrush;

	UPROPERTY(EditAnywhere,
			  meta = (ReadOnlyKeys, EditCondition = "1==0", EditConditionHides, ShowOnlyInnerProperties, EditFixedSize,
					  EditFixedOrder, NoResetToDefault),
			  Category = "Widgets")
	TMap<FName, FModioUIColorRef> SerializedColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widgets")
	FVector2D ItemSize;

	UFUNCTION()
	void OnTabIndexChanged(int TabIndex);

	virtual void PostLoad() override;

	
	bool NativeSupportsKeyboardFocus() const
	{
		return false;
	}
};
