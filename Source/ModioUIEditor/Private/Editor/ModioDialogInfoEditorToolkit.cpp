/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE4 Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *   
 */

#include "Editor/ModioDialogInfoEditorToolkit.h"
#include "Framework/Docking/TabManager.h"
#include "PropertyEditorModule.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SOverlay.h"

void FModioDialogEditorToolkit::CreateInternalWidgets()
{
	DialogWidget = TStrongObjectPtr<UModioDialogBaseInternal>(NewObject<UModioDialogBaseInternal>());
	
	DialogPreview = SNew(SOverlay)
	+SOverlay::Slot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SImage)
		.ColorAndOpacity(FLinearColor::Black)
	]
	+ SOverlay::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		SNew(SBox)
		.MaxDesiredWidth(1280)
		.MaxDesiredHeight(720)
		[
			SNew(SScaleBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Stretch(EStretch::ScaleToFit)
			[
				DialogWidget ? DialogWidget->TakeWidget() : SNullWidget::NullWidget
			]
		]
	];

	FPropertyEditorModule& PropertyEditorModule =
		FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.ColumnWidth = 0.5f;
	DialogPropertyEditor = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DialogPropertyEditor->SetObject(ObjectBeingEdited, true);
	DialogPropertyEditor->OnFinishedChangingProperties().AddRaw(this, &FModioDialogEditorToolkit::UpdatePreview);
	//Calling after TakeWidget above so we've got the widget rebuilt at this point
	if (DialogWidget)
	{
		UModioDialogInfo* EditObj = Cast<UModioDialogInfo>(ObjectBeingEdited);
		if (EditObj)
		{
			DialogWidget->InitializeFromDialogInfo(EditObj, true);
		}
	}
}

void FModioDialogEditorToolkit::UpdatePreview(const FPropertyChangedEvent&)
{
	UModioDialogInfo* EditObj = Cast<UModioDialogInfo>(ObjectBeingEdited);
	DialogWidget->InitializeFromDialogInfo(EditObj, true);
}

TSharedRef<SDockTab> FModioDialogEditorToolkit::HandleTabSpawnerSpawnPreview(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab).Label(FText::FromString("Preview"))[DialogPreview.ToSharedRef()];
}

TSharedRef<SDockTab> FModioDialogEditorToolkit::HandleTabSpawnerSpawnProperties(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab).Label(FText::FromString("Properties"))[
		SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1)
			[
				DialogPropertyEditor.ToSharedRef()
			]
			+SVerticalBox::Slot()
			.AutoHeight().HAlign(HAlign_Right)

			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([this]() {UpdatePreview(FPropertyChangedEvent{ nullptr }); return FReply::Handled(); })
				[
					SNew(STextBlock)
					.Text(FText::FromString("Update Preview"))
				]
			]
	];
}

void FModioDialogEditorToolkit::InitAssetEditor(
	const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	ObjectBeingEdited = ObjectToEdit;
	CreateInternalWidgets();
	// clang-format off
	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout =
		FTabManager::NewLayout("Standalone_ModioDialogEditor_Layout")
			->AddArea(
				FTabManager::NewPrimaryArea()
					->SetOrientation(Orient_Horizontal)
					->Split(
						FTabManager::NewStack()
						->SetSizeCoefficient(2.f)
						->AddTab(DialogPreviewTabId, ETabState::OpenedTab)
						->SetHideTabWell(true)
					)
					->Split(
						FTabManager::NewStack()
								->AddTab(DialogPropertiesTabId, ETabState::OpenedTab)
								->SetHideTabWell(true)
					)
			);
	// clang-format on
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, DialogEditorAppId, StandaloneDefaultLayout, true, true,
										 ObjectToEdit);
	RegenerateMenusAndToolbars();
}

void FModioDialogEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(FText::FromString("Modio Dialog Editor"));
	TSharedRef<FWorkspaceItem> CategoryRef = WorkspaceMenuCategory.ToSharedRef();
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	InTabManager
		->RegisterTabSpawner(DialogPreviewTabId,
							 FOnSpawnTab::CreateSP(this, &FModioDialogEditorToolkit::HandleTabSpawnerSpawnPreview))
		.SetDisplayName(FText::FromString("Preview"))
		.SetGroup(CategoryRef);
	InTabManager
		->RegisterTabSpawner(DialogPropertiesTabId,
							 FOnSpawnTab::CreateSP(this, &FModioDialogEditorToolkit::HandleTabSpawnerSpawnProperties))
		.SetDisplayName(FText::FromString("Properties"))
		.SetGroup(CategoryRef);
}

void FModioDialogEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(DialogPreviewTabId);
	InTabManager->UnregisterTabSpawner(DialogPropertiesTabId);
}

FName FModioDialogEditorToolkit::GetToolkitFName() const
{
	return FName("Modio Dialog Info Editor");
}

FText FModioDialogEditorToolkit::GetBaseToolkitName() const
{
	return FText::FromName(DialogEditorAppId);
}

FString FModioDialogEditorToolkit::GetWorldCentricTabPrefix() const
{
	return "Dialog ";
}


FLinearColor FModioDialogEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}
