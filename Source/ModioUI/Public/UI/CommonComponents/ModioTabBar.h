// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Styles/ModioUIColorRef.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Views/STileView.h"

#include "ModioTabBar.generated.h"
/**
 *
 */
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TArray<FText> TabNames;
	TSharedPtr<STileView<TSharedPtr<FText>>> MyListView;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FTableRowStyle TEMP_TableRowStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category="Widgets")
	FModioUIStyleRef TabButtonStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (StyleClass = "ModioButtonStyle"), Category="Widgets")
	FModioUIStyleRef InactiveTabButtonStyle;

	/// @brief Style for the tab which is selected but not hovered/focused
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FSlateBrush SelectedButtonBrush;

	UPROPERTY(EditAnywhere, meta = (ReadOnlyKeys, EditCondition = "1==0", EditConditionHides, ShowOnlyInnerProperties,
									EditFixedSize, EditFixedOrder, NoResetToDefault), Category="Widgets")
	TMap<FName, FModioUIColorRef> SerializedColors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FVector2D ItemSize;

	virtual void PostLoad() override;
};
