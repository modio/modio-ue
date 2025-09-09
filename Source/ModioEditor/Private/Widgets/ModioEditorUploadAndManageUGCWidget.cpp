/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/ModioEditorUploadAndManageUGCWidget.h"
#include "ModioSubsystem.h"
#include "SlateOptMacros.h"
#include "Widgets/ModioEditorAddModFileWidget.h"
#include "Widgets/ModioEditorAddSourceFileWidget.h"
#include "Widgets/ModioEditorBrowseModsWidget.h"
#include "Widgets/ModioEditorCreateModProfileWidget.h"
#include "Widgets/ModioEditorEditModProfileWidget.h"

struct FModioFilterParams;

#define LOCTEXT_NAMESPACE "UploadAndManageUGCWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorUploadAndManageUGCWidget::Construct(const FArguments& InArgs)
{
	ParentWindow = InArgs._ParentWindow;

	ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();

	// clang-format off
	ChildSlot
	[
		SAssignNew(WidgetContent, SBox)
		.MinDesiredWidth(900)
		.MinDesiredHeight(500)
	];
	// clang-format on

	DrawContent();
}

void SModioEditorUploadAndManageUGCWidget::DrawContent()
{
	// clang-format off

	WidgetContent->SetContent(GetContent());

	// clang-format on
}

const TSharedRef<SWidget> SModioEditorUploadAndManageUGCWidget::GetContent()
{
	switch (UploadAndEditUIState)
	{
		case BrowseMods:
			return SNew(SModioEditorBrowseModsWidget, ModioSubsystem)
				.UploadAndManageWidget(this)
				.ParentWindow(ParentWindow)
				.ModSelectedHandler(this, &SModioEditorUploadAndManageUGCWidget::ModSelectionChanged)
				.NavigationHandler(this, &SModioEditorUploadAndManageUGCWidget::HandleNavigation);
		case CreateMod:
			return SNew(SModioEditorCreateModProfileWidget, ModioSubsystem)
				.ParentWindow(ParentWindow)
				.NavigationHandler(this, &SModioEditorUploadAndManageUGCWidget::HandleNavigation);
		case EditMod_EditProfile:
			return SNew(SModioEditorEditModProfileWidget, ModioSubsystem)
				.ParentWindow(ParentWindow)
				.ModInfo(*CurrentModInfo)
				.NavigationHandler(this, &SModioEditorUploadAndManageUGCWidget::HandleNavigation);
		case EditMod_AddNewModFile:
			return SNew(SModioEditorAddModFileWidget, ModioSubsystem)
				.ParentWindow(ParentWindow)
				.ModInfo(*CurrentModInfo)
				.NavigationHandler(this, &SModioEditorUploadAndManageUGCWidget::HandleNavigation);
		case EditMod_AddNewSourceModFile:
			return SNew(SModioEditorAddSourceFileWidget, ModioSubsystem)
				.ParentWindow(ParentWindow)
				.ModInfo(*CurrentModInfo)
				.NavigationHandler(this, &SModioEditorUploadAndManageUGCWidget::HandleNavigation);
		default:
			return SNew(SModioEditorBrowseModsWidget, ModioSubsystem);
	}
}

void SModioEditorUploadAndManageUGCWidget::HandleNavigation(EUploadAndManageUIState ToState)
{
	UploadAndEditUIState = ToState;

	DrawContent();
}

void SModioEditorUploadAndManageUGCWidget::ModSelectionChanged(TSharedPtr<FModioModInfo> ModInfo)
{
	CurrentModInfo = *ModInfo;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE