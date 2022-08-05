// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "Containers/UnrealString.h"
#include "CoreMinimal.h"
#include "UI/BaseWidgets/Slate/SModioCustomComboBox.h"
#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UI/Styles/ModioUIStyleRef.h"
#include "Widgets/Views/STableViewBase.h"

#include "ModioPopupComboBox.generated.h"

UENUM(BlueprintType)
enum class EModioPopupPlacement : uint8
{
	AlignBottomLeft,
	AlignTopLeft
};
/**
 * 
 */
UCLASS()
class MODIOUI_API UModioPopupComboBox : public UWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY()
	FModioUIMenuCommandList CurrentEntries;
	
	using FOnSelectionChanged = typename TSlateDelegates<FText>::FOnSelectionChanged;
	using FOnSelectionChangedWithDelegate = typename TSlateDelegates<FModioUIAction>::FOnSelectionChanged;

	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual TSharedRef<ITableRow> GenerateOptionWidget(TSharedPtr<FText> Item,
													   const TSharedRef<STableViewBase>& OwnerTable);

	FSlateColor GetButtonTextColor() const;
	
	const FModioUIStyleRef* GetTextStyle() const;

	void OnSelectionChangedHandler(TSharedPtr<FText> NewSelection, ESelectInfo::Type SelectionType);

	virtual TSharedRef<SWidget> GenerateButtonContent(TSharedPtr<FText> Item);

	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SModioCustomComboBox<TSharedPtr<FText>>> MyComboBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText LabelFormatText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FText ValueFormatText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bUseRoundedCorners = true;

	//Possibly this can just reference ComboBoxStyle
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef RoundedCornerStyle = FModioUIStyleRef {"DefaultComboBoxBorderStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef ComboBoxStyle = FModioUIStyleRef {"DefaultComboBoxStyle"};

	//TODO: @modio-ue4 move into the ComboBoxStyle
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FModioUIStyleRef TextStyle = FModioUIStyleRef {"DefaultRichTextStyle"};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	FMargin ContentPadding = FMargin(20.0f, 8);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	bool bPreviewOpen = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets")
	TArray<FText> OptionValues;

	TArray<TSharedPtr<FText>> BoundValues;

	void UpdateBoundValues();

	FOnSelectionChanged OnSelectionChanged;
	FOnSelectionChangedWithDelegate OnSelectionChangedWithDelegate;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category="Widgets")
	EModioPopupPlacement PopupPlacement = EModioPopupPlacement::AlignTopLeft;


public:
	void SetOptionValues(const TArray<FText>& Values);

	void SetComboBoxEntries(FModioUIMenuCommandList Entries)
	{
		CurrentEntries = Entries;
	}

	virtual FOnSelectionChanged& GetOnSelectionChanged()
	{
		return OnSelectionChanged;
	}

	virtual void SetOnSelectionChangedWithDelegate(FOnSelectionChangedWithDelegate Delegate)
	{
		OnSelectionChangedWithDelegate = Delegate;
	}

	virtual int32 GetSelectedItemIndex()
	{
		if (MyComboBox)
		{
			return MyComboBox->GetSelectedItemIndex();
		}
		else
		{
			return -1;
		}
	}
	void SetSelectedItemIndex(int32 Index)
	{
		if (MyComboBox)
		{
			MyComboBox->SetSelectedItemIndex(Index);
		}
	}
};
