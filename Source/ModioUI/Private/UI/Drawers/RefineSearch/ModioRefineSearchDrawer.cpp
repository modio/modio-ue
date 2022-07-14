#include "UI/Drawers/RefineSearch/ModioRefineSearchDrawer.h"
#include "Types/ModioFilterParams.h"

void UModioRefineSearchDrawer::NativeOnInitialized()
{
	UModioUserWidgetBase::NativeOnInitialized();
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UModioRefineSearchDrawer::OnApplyClicked);
	}
	if (ClearButton)
	{
		ClearButton->OnClicked.AddDynamic(this, &UModioRefineSearchDrawer::OnClearClicked);
	}
	bIsFocusable = true;
}

FReply UModioRefineSearchDrawer::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (TagSelector)
	{
		TagSelector->InvalidateLayoutAndVolatility();
	}
	if (SearchInput)
	{
		return FReply::Handled().SetUserFocus(SearchInput->TakeWidget(), EFocusCause::Navigation);
	}
	else
	{
		return FReply::Handled();
	}

}

void UModioRefineSearchDrawer::OnClearClicked()
{
	if (TagSelector)
	{
		TagSelector->ClearSelectedTags();
	}
	if (SearchInput)
	{
		SearchInput->SetText(FText());
	}
}

void UModioRefineSearchDrawer::OnApplyClicked()
{
	Execute_NotifySettingsChanged(
		this,
		FModioFilterParams().NameContains(Execute_GetSearchString(this)).WithTags(Execute_GetSelectedTagValues(this)));

	GEngine->GetEngineSubsystem<UModioUISubsystem>()->CloseSearchDrawer();
}

FReply UModioRefineSearchDrawer::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}

FReply UModioRefineSearchDrawer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UModioRefineSearchDrawer::NextTagCategory()
{
	if (TagSelector)
	{
		TagSelector->DisplayNextTagCategory();
	}
}

void UModioRefineSearchDrawer::PrevTagCategory()
{
	if (TagSelector)
	{
		TagSelector->DisplayPrevTagCategory();
	}
}

void UModioRefineSearchDrawer::BuildCommandList(TSharedRef<FUICommandList> InCommandList)
{
	InCommandList->MapAction(
		FModioCommonUICommands::Get().NextPage,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::NextTagCategory)));
	InCommandList->MapAction(
		FModioCommonUICommands::Get().PreviousPage,
		FUIAction(FExecuteAction::CreateUObject(this, &UModioRefineSearchDrawer::PrevTagCategory)));
}

FString UModioRefineSearchDrawer::NativeGetSearchString()
{
	if (SearchInput)
	{
		return SearchInput->GetText().ToString();
	}
	return FString();
}

TArray<FString> UModioRefineSearchDrawer::NativeGetSelectedTagValues()
{
	if (TagSelector)
	{
		return TagSelector->GetSelectedTags();
	}
	return {};
}

void UModioRefineSearchDrawer::OnTagOptionsReceived(FModioErrorCode ec, TOptional<FModioModTagOptions> Options)
{
	if (!ec)
	{
		if (TagSelector)
		{
			TagOptions = NewObject<UModioTagOptionsUI>();
			TagOptions->Underlying = Options.GetValue();
			TagSelector->SetDataSource(TagOptions);
		}
	}
}

void UModioRefineSearchDrawer::NativeRefreshTags()
{
	if (!TagOptions)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->GetModTagOptionsAsync(
				FOnGetModTagOptionsDelegateFast::CreateUObject(this, &UModioRefineSearchDrawer::OnTagOptionsReceived));
		}
	}
	Invalidate(EInvalidateWidgetReason::LayoutAndVolatility | EInvalidateWidgetReason::Paint);
	if (TagSelector)
	{
		TagSelector->InvalidateLayoutAndVolatility();
	}
}

FOnSearchSettingsChanged& UModioRefineSearchDrawer::NativeGetOnSettingsChangedDelegate()
{
	return OnSettingsChanged;
}
