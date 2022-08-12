// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "UI/BaseWidgets/Slate/SModioListView.h"
#include "UI/Commands/ModioUIMenuEntry.h"
#include "UI/Interfaces/IModioUIPopupMenuContentWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "UI/Styles/ModioUIBrushRef.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "Materials/MaterialInterface.h"
#include "Blueprint/UserWidget.h"

#include "ModioDefaultPopupMenuContent.generated.h"

/**
 *
 */
UCLASS()
class MODIOUI_API UModioDefaultPopupMenuContent : public UUserWidget, public IModioUIPopupMenuContentWidget
{
	GENERATED_BODY()
protected:
	FVector2D DesiredSizeFromParentMenu = FVector2D(32,32);

	FOptionalSize GetDesiredItemWidth() const;
	FOptionalSize GetDesiredMenuHeight() const;
	float GetDesiredItemHeight() const;
	TSharedPtr<SBox> MyBox;
	TSharedPtr<SModioListView<TSharedPtr<FModioUIMenuEntry>>> MyList;
	TSharedPtr<class SRetainerWidget> MyBorder;
	TArray<TSharedPtr<FModioUIMenuEntry>> CurrentMenuEntries;
	FModioUIMenuCommandList CurrentCommandList;
	virtual void SynchronizeProperties() override;
	virtual void UpdateBoundValues();
	virtual void OnEntrySelected(TSharedPtr<FModioUIMenuEntry> SelectedEntry, ESelectInfo::Type SelectionType);
	const FModioUIStyleRef* GetTextStyle() const
	{
		return &TextStyle;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef BorderStyle = FModioUIStyleRef {"DefaultPopupBorderStyle"};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widgets")
	FModioUIMaterialRef RetainerMaterial = FModioUIMaterialRef {"RoundedRectangle"};

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	int32 PreviewItemCount = 5;
#endif

	TSharedRef<ITableRow> GenerateMenuEntryWidget(TSharedPtr<FModioUIMenuEntry> Item,
												  const TSharedRef<STableViewBase>& OwnerTable);

	void NativeSetMenuEntries(const struct FModioUIMenuCommandList& Entries) override;
	void NativeSetDesiredSize(const FVector2D& DesiredSize) override;

	void ReleaseSlateResources(bool bReleaseChildren);
	TSharedRef<SWidget> RebuildWidget() override;
};
