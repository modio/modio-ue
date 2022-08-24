#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "Core/ModioModInfoUI.h"
#include "UI/BaseWidgets/ModioRichTextBlock.h"

#include "ModioDownloadListWidgetBase.generated.h"

UCLASS()
class MODIOUI_API UModioDownloadListWidgetBase : public UModioUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioListView* QueueList;

	void SetStatusLabelVisibility(ESlateVisibility StatusLabelVisibility)
	{
		if (StatusString)
		{
			StatusString->SetVisibility(StatusLabelVisibility);
		}
	}

	void UpdateListItems(TArray<UModioModInfoUI*> ModInfoList)
	{
		QueueList->SetListItems(ModInfoList);
		QueueList->RequestRefresh();
		
		if (ModInfoList.Num() == 0)
		{
			SetStatusLabelVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SetStatusLabelVisibility(ESlateVisibility::Collapsed);
		}
		
	}
	
protected:
	virtual void NativeOnSetDataSource() override;
	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* QueueTypeLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Widgets", meta = (BindWidget))
	UModioRichTextBlock* StatusString;

	TArray<TSharedPtr<UModioModInfoUI*>> ListSource;
	
	void Refresh();
};
