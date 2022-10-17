/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/BaseWidgets/ModioUIAsyncLoadingOverlay.h"

#include "UI/BaseWidgets/ModioUIAsyncLoader.h"
#include "UI/Styles/ModioDialogStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSpacer.h"

void UModioUIAsyncLoadingOverlay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UModioUIAsyncLoadingOverlay::Close);
	}
}

TSharedRef<SWidget> UModioUIAsyncLoadingOverlay::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	return Widget;
}

void UModioUIAsyncLoadingOverlay::Close()
{
	OwningDialog->HideLoadingSpinner();
}

void UModioUIAsyncLoadingOverlay::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	ApplyStyling();
}

void UModioUIAsyncLoadingOverlay::ApplyStyling()
{
	const FModioDialogStyle* ResolvedStyle = DialogStyle.FindStyle<FModioDialogStyle>();
	if (ResolvedStyle)
	{
		if (BackgroundImage)
		{
			BackgroundImage->SetBrush(ResolvedStyle->BackgroundBrush);
		}
	}
}
