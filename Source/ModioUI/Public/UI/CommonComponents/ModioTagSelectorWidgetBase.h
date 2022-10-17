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

#include "UI/BaseWidgets/ModioRichTextBlock.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"
#include "UI/CommonComponents/ModioListViewString.h"

#include "ModioTagSelectorWidgetBase.generated.h"

UCLASS()
class UModioTagSelectorWidgetBase : public UModioUserWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	// We need to support multiple selection so we ignore the parameter, we'll manually get the list of selected items
	// in this function
	virtual void OnTagSelectionChanged(TSharedPtr<FString>);

	TArray<TSharedPtr<FString>> TagListItemSource;

	int32 CurrentTagIndex;

	virtual void NativeOnSetDataSource() override;

	void Refresh();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* TagCategoryLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioListViewString* CategoryTagList;

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ModioTagSelectorWidgetBase")
	TArray<FString> GetSelectedTags();
	UFUNCTION(BlueprintCallable, Category = "ModioTagSelectorWidgetBase")
	void ClearSelectedTags();
};
