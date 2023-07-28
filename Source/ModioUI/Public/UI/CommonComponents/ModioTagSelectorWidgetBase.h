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
	
	bool bCategoryCollapsed = false;
	int32 CurrentTagIndex;

	UPROPERTY()
	class UModioTagInfoUI* CachedTagInfo;

	virtual void NativeOnSetDataSource() override;
	virtual void NativeOnInitialized() override;

	void Refresh();

	UFUNCTION()
	void OnCheckboxChecked(class UModioSelectableTag* SourceTag, bool bIsChecked);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioRichTextBlock* TagCategoryLabel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioButton* TagCategoryCollapseButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioImage* TagCategoryCollapseImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	class UModioInputBindingImage* TagCategoryInputHint;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CategoryVerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UModioSelectableTag> TagWidgetTemplate;

	void ToggleKeybindVisibility(bool bActive);
	bool IsCollapsed();

	UFUNCTION()
	void OnCategoryCollapseToggled();
};
