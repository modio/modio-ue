// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR

	#include "Editor/ModioModInfoUIFactory.h"
	#include "AssetRegistryModule.h"
	#include "Core/ModioModInfoUI.h"
	#include "Core/ModioUIHelpers.h"
	#include "Widgets/DeclarativeSyntaxSupport.h"
	#include "Widgets/SWindow.h"
	#include "ModioSubsystem.h"
	#include "Libraries/ModioErrorConditionLibrary.h"
	#include "Libraries/ModioSDKLibrary.h"
	#include "Algo/Transform.h"
	#include "Widgets/Input/SButton.h"
	#include "Widgets/Text/STextBlock.h"
	#include "Widgets/Views/STableRow.h"
	#include "Widgets/Views/SListView.h"

UModioModInfoUIFactory::UModioModInfoUIFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UModioModInfoUI::StaticClass();
}

bool UModioModInfoUIFactory::CanCreateNew() const
{
	return true;
}

UObject* UModioModInfoUIFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
												  UObject* Context, FFeedbackContext* Warn)
{
	UModioModInfoUI* NewModInfo = NewObject<UModioModInfoUI>(InParent, InClass, InName, Flags);
	checkf(SelectedItem, TEXT("SelectedItem should never be nullptr"));
	NewModInfo->Underlying = *SelectedItem;
	FAssetRegistryModule::AssetCreated(NewModInfo);
	UPackage* InPackage = Cast<UPackage>(InParent);

	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	ModioUIHelpers::SavePackage(InPackage, NewModInfo, RF_Standalone | RF_Public, *InPackagePath);
	return NewModInfo;
}

FText UModioModInfoUIFactory::GetDisplayName() const
{
	return FText::FromString("Preview ModioModInfoUI");
}

bool UModioModInfoUIFactory::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(UModioModInfoUI::StaticClass());
}

bool UModioModInfoUIFactory::ConfigureProperties()
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		Subsystem->InitializeAsync(UModioSDKLibrary::GetProjectInitializeOptions(),
								   FOnErrorOnlyDelegateFast::CreateLambda([this](FModioErrorCode ec) {
									   bOperationInProgress = false;
									   if (!ec)
									   {
										   bRequiresSDKShutdown = true;
									   }
								   }));

		while (bOperationInProgress)
		{
			Subsystem->RunPendingHandlers();
		}

		bOperationInProgress = true;
		Subsystem->ListAllModsAsync(FModioFilterParams(), FOnListAllModsDelegateFast::CreateUObject(
															  this, &UModioModInfoUIFactory::OnModListRetrieved));
		while (bOperationInProgress)
		{
			Subsystem->RunPendingHandlers();
		}
		if (bRequiresSDKShutdown)
		{
			bOperationInProgress = true;
			Subsystem->ShutdownAsync(
				FOnErrorOnlyDelegateFast::CreateLambda([this](FModioErrorCode) { bOperationInProgress = false; }));
			while (bOperationInProgress)
			{
				Subsystem->RunPendingHandlers();
			}
		}
	}
	else
	{
		return false;
	}
	// clang-format off
	SAssignNew(ImportWindow, SWindow)
	.Title(FText::FromString("Import Preview Mod Info"))
	.ClientSize(FVector2D(500, 600))
	.HasCloseButton(true)
	.SupportsMinimize(false)
	.SupportsMaximize(false)
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SAssignNew(AvailableModInfoList, SListView<TSharedPtr<FModioModInfo>>)
					.ListItemsSource(&ModInfos)
					.Visibility_UObject(this, &UModioModInfoUIFactory::IsListVisible)
					.OnSelectionChanged_UObject(this, &UModioModInfoUIFactory::OnSelectionChanged)
					.OnGenerateRow_UObject(this, &UModioModInfoUIFactory::OnGenerateRowForList)
			] 
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Bottom)
					.OnClicked_UObject(this, &UModioModInfoUIFactory::OnConfirmClicked)
					.IsEnabled_UObject(this, &UModioModInfoUIFactory::IsModInfoSelected)
					[
						SNew(STextBlock)
						.Text(FText::FromString("OK"))
					]
				]
			]
		]
	];
	// clang-format on
	GEditor->EditorAddModalWindow(ImportWindow.ToSharedRef());
	ImportWindow.Reset();

	return bConfirmClicked;
}

void UModioModInfoUIFactory::OnModListRetrieved(FModioErrorCode ec, TOptional<FModioModInfoList> ModList)
{
	ModInfos.Empty();
	if (!ec)
	{
		Algo::Transform(ModList->GetRawList(), ModInfos,
						[](const FModioModInfo& Info) { return MakeShareable(new FModioModInfo(Info)); });
	}
	bOperationInProgress = false;
}

FReply UModioModInfoUIFactory::OnConfirmClicked()
{
	bConfirmClicked = true;
	ImportWindow->RequestDestroyWindow();
	return FReply::Handled();
}

EVisibility UModioModInfoUIFactory::IsListVisible() const
{
	return ModInfos.Num() ? EVisibility::Visible : EVisibility::Hidden;
}

bool UModioModInfoUIFactory::IsModInfoSelected() const
{
	return SelectedItem != nullptr;
}

void UModioModInfoUIFactory::OnSelectionChanged(TSharedPtr<FModioModInfo> Item, ESelectInfo::Type)
{
	SelectedItem = Item;
}

TSharedRef<ITableRow> UModioModInfoUIFactory::OnGenerateRowForList(TSharedPtr<FModioModInfo> Item,
																   const TSharedRef<class STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FModioModInfo>>, OwnerTable)
		.Content()[SNew(STextBlock).Text(Item ? FText::FromString(Item->ProfileName) : FText())];
}

UClass* FModioModInfoUIAssetActions::GetSupportedClass() const
{
	return UModioModInfoUI::StaticClass();
}

uint32 FModioModInfoUIAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

#endif