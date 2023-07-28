/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/CommonComponents/ModioTagSelectorWidgetBase.h"
#include "UI/CommonComponents/ModioSelectableTag.h"
#include "UI/BaseWidgets/ModioInputBindingImage.h"
#include "UI/BaseWidgets/ModioButton.h"
#include "UI/BaseWidgets/ModioImage.h"
#include "Components/VerticalBox.h"
#include "Algo/Transform.h"
#include "Core/ModioTagOptionsUI.h"
#include "Types/ModioModTagInfo.h"

void UModioTagSelectorWidgetBase::NativeOnInitialized()
{
	bIsFocusable = false;
	TagCategoryCollapseButton->OnClicked.AddUniqueDynamic(this, &UModioTagSelectorWidgetBase::OnCategoryCollapseToggled);
}

void UModioTagSelectorWidgetBase::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	Refresh();
}

void UModioTagSelectorWidgetBase::Refresh()
{
	CachedTagInfo = Cast<UModioTagInfoUI>(DataSource);
	if (CachedTagInfo)
	{
		if (TagCategoryLabel)
		{
			UModioUISubsystem* subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (!subsystem)
			{
				TagCategoryLabel->SetText(FText::FromString(CachedTagInfo->Underlying.TagGroupName));
			}
			else
			{
				TagCategoryLabel->SetText(subsystem->GetLocalizedTag(CachedTagInfo->Underlying.TagGroupName));
			}
			
		}

		for (auto& child : CategoryVerticalBox->GetAllChildren())
		{
			CategoryVerticalBox->RemoveChild(child);
		}
		for (auto& item : CachedTagInfo->Underlying.TagGroupValues)
		{
			UModioSelectableTag* tagWidget = CreateWidget<UModioSelectableTag>(this, TagWidgetTemplate);
			CategoryVerticalBox->AddChild(tagWidget);
			tagWidget->SetTagLabel(item);
			tagWidget->OnTagStateChanged.AddUniqueDynamic(this, &UModioTagSelectorWidgetBase::OnCheckboxChecked);
		}
	}
}

void UModioTagSelectorWidgetBase::OnCheckboxChecked(UModioSelectableTag* SourceTag, bool bIsChecked)
{
	if (!CachedTagInfo || !bIsChecked)
	{
		return;
	}
	
	if (CachedTagInfo->Underlying.bAllowMultipleSelection)
	{
		return;
	}

	for (auto& child : CategoryVerticalBox->GetAllChildren())
	{
		UModioSelectableTag* tagWidget = Cast<UModioSelectableTag>(child);
		if (!tagWidget || tagWidget == SourceTag)
		{
			continue;
		}

		tagWidget->TagSelectedCheckbox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UModioTagSelectorWidgetBase::OnCategoryCollapseToggled()
{
	bCategoryCollapsed = !bCategoryCollapsed;
	FWidgetTransform transform;
	transform.Angle = bCategoryCollapsed ? 0.0f : 180.0f;
	transform.Scale = FVector2D(1.0f, 1.0f);
	TagCategoryCollapseImage->SetRenderTransform(transform);
	CategoryVerticalBox->SetVisibility(bCategoryCollapsed ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UModioTagSelectorWidgetBase::ToggleKeybindVisibility(bool bActive)
{
	TagCategoryInputHint->SetRenderOpacity(bActive ? 1.0f : 0.0f);
}

bool UModioTagSelectorWidgetBase::IsCollapsed()
{
	return bCategoryCollapsed;
}