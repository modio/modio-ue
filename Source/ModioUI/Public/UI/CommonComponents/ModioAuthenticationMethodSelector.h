#pragma once

#include "Algo/Transform.h"
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
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
		// clang-format off
		TSharedRef<STableRow<TSharedPtr<FText>>> TableRow = SNew(STableRow<TSharedPtr<FText>>, OwnerTableView)
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
	void ReleaseSlateResources(bool bReleaseChildren) override
	{
		Super::ReleaseSlateResources(bReleaseChildren);
		MyButtonList.Reset();
		Buttons.Empty();
	}
};