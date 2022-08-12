/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioModUninstallHeaderWidget.h"

#include "Libraries/ModioSDKLibrary.h"
#include "ModioSubsystem.h"
#include "UI/BaseWidgets/Slate/SModioRichTextBlock.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Styles/ModioDialogStyle.h"
#include "UI/Styles/ModioRichTextStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SGridPanel.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioModUninstallHeaderWidget::SynchronizeProperties()
{
	UWidget::SynchronizeProperties();

	const FModioDialogStyle* ResolvedStyle = Style.FindStyle<FModioDialogStyle>();

	if (ResolvedStyle)
	{
		const FModioRichTextStyle* ResolvedTitleTextStyle =
			ResolvedStyle->ContentTextStyle.FindStyle<FModioRichTextStyle>();

		if (ResolvedTitleTextStyle)
		{
			if (ModNameText.IsValid())
			{
				ModNameText->StyleReference = &ResolvedStyle->ContentTextStyle;
				ModNameText->ReapplyStyle();
			}

			if (ModSizeText.IsValid())
			{
				ModSizeText->StyleReference = &ResolvedStyle->ContentTextStyle;
				ModSizeText->ReapplyStyle();
			}
		}
	}
}

TSharedRef<SWidget> UModioModUninstallHeaderWidget::RebuildWidget()
{
	FString ModName = "Mod Name";
	FText ModDesc = FText::FromString("This will be removed from your collection");

	if (DataSource)
	{
		if (DataSource->Implements<UModioModInfoUIDetails>())
		{
			FModioModInfo CurrentModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
			ModName = FString::Printf(TEXT("<override bold=\"true\">'%s'</>"), *CurrentModInfo.ProfileName);

			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				TMap<FModioModID, FModioModCollectionEntry> SystemMods = Subsystem->QuerySystemInstallations();
				if (SystemMods.Contains(CurrentModInfo.ModId))
				{
					FModioModCollectionEntry& CurrentModEntry = SystemMods[CurrentModInfo.ModId];
					ModDesc =
						LOCTEXT("ModUninstallSize", "This will be removed from the System Collection and free up {Size}.");

					const uint64 NumBytes = CurrentModEntry.GetSizeOnDisk().Underlying;

					FFormatNamedArguments Args;
					Args.Add("Size", UModioSDKLibrary::Filesize_ToString(NumBytes, NumBytes < GB ? 0 : 1));
					ModDesc = FText::Format(ModDesc, Args);
				}
			}
		}
	}
	// clang-format off
	return SAssignNew(MyGrid, SGridPanel) 
		+SGridPanel::Slot(0, 0)
		.HAlign(HAlign_Center)
		[
			SAssignNew(ModNameText, SModioRichTextBlock).Text(FText::FromString(ModName))
		]
		+SGridPanel::Slot(0,1)
		.HAlign(HAlign_Center)
		.Padding(0,24,0,0)
		[
			SAssignNew(ModSizeText, SModioRichTextBlock).Text(ModDesc)
		];
	// clang-format on
}

	void UModioModUninstallHeaderWidget::ReleaseSlateResources(bool bReleaseChildren)
	{
		Super::ReleaseSlateResources(bReleaseChildren);
		MyGrid.Reset();
		ModNameText.Reset();
		ModSizeText.Reset();
	}

#include "Loc/EndModioLocNamespace.h"