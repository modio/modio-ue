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

#include "Algo/Transform.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/BaseWidgets/ModioWidgetBase.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/CommonComponents/ModioRichTextButton.h"
#include "UI/Dialogs/ModioDialogController.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UI/Styles/ModioButtonStyle.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/STileView.h"

#include "ModioAuthenticationMethodSelector.generated.h"

/**
* Modio UI element that provides a list of possible authentication methods available
* on the current session
**/
UCLASS()
class UModioAuthenticationMethodSelector : public UModioWidgetBase, public IModioUIDialogButtonWidget
{
	GENERATED_BODY()

protected:
	TSharedPtr<STileView<TSharedPtr<FModioUIAuthenticationProviderInfo>>> MyButtonList;
	TArray<TSharedPtr<SButton>> Buttons;
	TArray<TSharedPtr<FModioUIAuthenticationProviderInfo>> AuthMethods;
	FModioUIStyleRef ButtonStyle {"DefaultDialogButtonStyle"};
	TWeakObjectPtr<UModioDialogController> DialogController;
	TSharedPtr<SHorizontalBox> ButtonBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextButton* CancelButton;

	virtual void NativeSetDialogController(class UModioDialogController* Controller) override
	{
		DialogController = Controller;
	}

	const FModioUIStyleRef* GetButtonTextStyle() const
	{
		if (const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>())
		{
			return &ResolvedButtonStyle->RichTextStyle;
		}
		return nullptr;
	}

	virtual void NativeSetStyle(const FModioDialogStyle& Style)
	{
		if (const FModioButtonStyle* ResolvedButtonStyle = Style.ButtonStyle.FindStyle<FModioButtonStyle>())
		{
			ButtonStyle = Style.ButtonStyle;
			if (MyButtonList)
			{
				for (TSharedPtr<SButton> CurrentButton : Buttons)
				{
					CurrentButton->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
					CurrentButton->SetContentPadding(ResolvedButtonStyle->ContentPadding);
				}

				MyButtonList->Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
			}
		}
	}

	static FReply OnButtonClicked(TSharedPtr<FModioUIAuthenticationProviderInfo> ButtonInfo,
								  TWeakObjectPtr<UModioDialogController> DialogController)
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (ButtonInfo->bIsEmailAuthentication)
			{
				if (DialogController.IsValid())
				{
					DialogController->ShowEmailAuthenticationDialog();
				}
			}
			else
			{
				Subsystem->RequestExternalAuthentication(ButtonInfo->ProviderID);
			}
		}
		return FReply::Handled();
	}
	TSharedRef<class ITableRow> OnGenerateButton(TSharedPtr<FModioUIAuthenticationProviderInfo> ButtonInfo,
												 const TSharedRef<class STableViewBase>& OwnerTableView)
	{
		TSharedPtr<SButton> RowButton;
		TSharedPtr<SModioRichTextBlock> RowTextBlock;
#if UE_VERSION_NEWER_THAN(5, 0, 0)
		// UE5 changes the default FTableRowStyle to have a black background, grr
		static FTableRowStyle StyleOverride = FCoreStyle::Get().GetWidgetStyle<FTableRowStyle>("TableView.Row");
		StyleOverride.SetEvenRowBackgroundBrush(FSlateColorBrush(FSlateColor(FLinearColor::White)))
			.SetOddRowBackgroundBrush(FSlateColorBrush(FSlateColor(FLinearColor::White)))
			.SetEvenRowBackgroundHoveredBrush(FSlateColorBrush(FSlateColor(FLinearColor::White)))
			.SetOddRowBackgroundHoveredBrush(FSlateColorBrush(FSlateColor(FLinearColor::White)));
#endif
		// clang-format off
		TSharedRef<STableRow<TSharedPtr<FText>>> TableRow = SNew(STableRow<TSharedPtr<FText>>, OwnerTableView)
#if UE_VERSION_NEWER_THAN(5,0,0) 
		// Hacky, but Epic don't let you set a style on an STableRow post-construction, extra grr
		.Style(&StyleOverride)
#endif 
		.Content()
		[
			SAssignNew(RowButton, SButton)
			.OnClicked_Static(&UModioAuthenticationMethodSelector::OnButtonClicked, ButtonInfo, DialogController)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.ContentPadding(FMargin(16, 4,16, 2))
			[
				SAssignNew(RowTextBlock, SModioRichTextBlock).Text(ButtonInfo->ProviderUILabel).StyleReference_UObject(this, &UModioAuthenticationMethodSelector::GetButtonTextStyle)
			]
		];
		// clang-format on
		const FModioButtonStyle* ResolvedButtonStyle = ButtonStyle.FindStyle<FModioButtonStyle>();
		if (ResolvedButtonStyle)
		{
			RowButton->SetButtonStyle(&ResolvedButtonStyle->ButtonStyle);
			RowButton->SetContentPadding(ResolvedButtonStyle->ContentPadding);
		}
		Buttons.Add(RowButton);
		TableRow->SetPadding(16.f);
		return TableRow;
	}

	UFUNCTION()
	void CancelClicked()
	{
		DialogController->FinalizeDialog();
	}

	TSharedRef<SWidget> RebuildWidget() override;

public:
	/**
	* Clear resources in this Widget
	* @param bReleaseChildren Trigger release as well for super classes
	**/
	void ReleaseSlateResources(bool bReleaseChildren) override
	{
		Super::ReleaseSlateResources(bReleaseChildren);
		MyButtonList.Reset();
		Buttons.Empty();
	}
};