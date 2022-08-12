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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Styles/ModioUIStyleRef.h"

#include "ModioModUninstallHeaderWidget.generated.h"

UCLASS()
class MODIOUI_API UModioModUninstallHeaderWidget : public UWidget, public IModioUIDataSourceWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UObject* DataSource;

protected:
	virtual void NativeSetDataSource(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	TSharedPtr<class SModioRichTextBlock> ModNameText;
	TSharedPtr<class SModioRichTextBlock> ModSizeText;
	TSharedPtr<class SGridPanel> MyGrid;

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (StyleClass = "DefaultDialogStyle"),
			  Category = "ModioModUninstallHeaderWidget")
	FModioUIStyleRef Style = FModioUIStyleRef {"DefaultDialogStyle"};

public:
	void ReleaseSlateResources(bool bReleaseChildren = false) override;
};
