/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/CategoryBrowser/ModioFeaturedView.h"
#include "Algo/Transform.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/BaseWidgets/Slate/SModioTileView.h"
#include "UI/CommonComponents/ModioTabBar.h"
#include "UI/Views/ModDetails/ModioAsyncOpWrapperWidget.h"

void UModioFeaturedView::NativeBeginLoadExternalData()
{
	Super::NativeBeginLoadExternalData();
	for (const auto& Category : AdditionalCategories->GetDisplayedEntryWidgets())
	{
		UModioFeaturedCategory* RealCategory = Cast<UModioFeaturedCategory>(Category);
		if (RealCategory)
		{
			RealCategory->LoadExternalData();
		}
	}
}

void UModioFeaturedView::OnAdditionalCategoryCreated(UUserWidget& CategoryWidget)
{
	UModioFeaturedCategory* RealCategory = Cast<UModioFeaturedCategory>(&CategoryWidget);
	RealCategory->OnCategorySelectionChanged().AddUniqueDynamic(this, &UModioFeaturedView::CategorySelectionChanged);
	FOnGetSelectionIndex SelectionIndexDelegate;
	SelectionIndexDelegate.BindDynamic(this, &UModioFeaturedView::GetSelectionIndex);
	RealCategory->SetSelectionIndexDelegate(SelectionIndexDelegate);
}

void UModioFeaturedView::NativeConstruct()
{
	Super::NativeConstruct();
	AdditionalCategories->OnEntryWidgetGenerated().AddUObject(this, &UModioFeaturedView::OnAdditionalCategoryCreated);
}

void UModioFeaturedView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	IModioUIModInfoReceiver::Register<UModioFeaturedView>(EModioUIModInfoEventType::ListAllMods);
	IModioUIUserChangedReceiver::Register<UModioFeaturedView>();

	if (PrimaryFeaturedCategoryLoader)
	{
		TScriptInterface<IModioUIAsyncOperationWidget> InterfaceWrapper =
			TScriptInterface<IModioUIAsyncOperationWidget>(this);
		IModioUIAsyncHandlerWidget::Execute_LinkAsyncOperationWidget(PrimaryFeaturedCategoryLoader, InterfaceWrapper);
	}

	if (FeaturedViewContent)
	{
		FeaturedViewContent->SetActualAsyncOperationWidget(TScriptInterface<IModioUIAsyncOperationWidget>(this));
	}

	FetchPrimaryCategoryMods();
}

void UModioFeaturedView::NativeRequestOperationRetry()
{
	FetchPrimaryCategoryMods();
}

void UModioFeaturedView::FetchPrimaryCategoryMods()
{
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		if (const UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
		{
			if (!Settings->BrowserCategoryConfiguration.IsNull())
			{
				if (UModioModBrowserParams* ModBrowserParams = Settings->BrowserCategoryConfiguration.LoadSynchronous())
				{
					FModioFilterParams Filter = FModioFilterParams()
					                            .WithTags(ModBrowserParams->PrimaryCategoryParams->Tags)
					                            .WithoutTags(ModBrowserParams->PrimaryCategoryParams->ExcludedTags)
					                            .SortBy(ModBrowserParams->PrimaryCategoryParams->SortField,
					                                    ModBrowserParams->PrimaryCategoryParams->Direction)
					                            .IndexedResults(0, ModBrowserParams->PrimaryCategoryParams->Count);
					Subsystem->RequestListAllMods(Filter, GetName().ToString());
				}
			}
		}
		else
		{
			Subsystem->RequestListAllMods(FModioFilterParams().IndexedResults(0, 5), GetName().ToString());
		}
		IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::InProgress);
	}
}

FNavigationReply UModioFeaturedView::NativeOnNavigation(const FGeometry& InGeometry,
                                                        const FNavigationEvent& InNavigationEvent)
{
	return Super::NativeOnNavigation(InGeometry, InNavigationEvent);
}

FReply UModioFeaturedView::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled()
	       .SetNavigation(PrimaryFeaturedCategory->TakeWidget(), ENavigationGenesis::User)
	       .SetUserFocus(PrimaryFeaturedCategory->TakeWidget(), EFocusCause::SetDirectly);
}

int32 UModioFeaturedView::GetSelectionIndex()
{
	UE_LOG(LogTemp, Display, TEXT("Delegate called, returning %d"), CurrentlySelectedCategoryColumn);
	return CurrentlySelectedCategoryColumn;
}

void UModioFeaturedView::CategorySelectionChanged(int32 Index, UModioFeaturedCategory* RealCategory)
{
	UE_LOG(LogTemp, Display, TEXT("Changed Index to %d"), Index);
	CurrentlySelectedCategoryColumn = Index;
}

void UModioFeaturedView::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ec,
                                                             TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ec, List);

	if (RequestIdentifier == GetName().ToString())
	{
		if (!ec)
		{
			CachedFeaturedItems.Empty();
			Algo::Transform(List.GetValue().GetRawList(), CachedFeaturedItems, [](const FModioModInfo& In) {
				UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
				WrappedMod->Underlying = In;
				return WrappedMod;
			});
			PrimaryFeaturedCategory->SetItems(CachedFeaturedItems);

			FSlateApplication::Get().SetUserFocus(0, PrimaryFeaturedCategory->TakeWidget(), EFocusCause::SetDirectly);

			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this,
			                                                           EModioUIAsyncOperationWidgetState::Success);
		}
		else
		{
			IModioUIAsyncOperationWidget::Execute_NotifyOperationState(this, EModioUIAsyncOperationWidgetState::Error);
		}
	}
}

bool UModioFeaturedView::ShouldShowSearchButtonForMenu()
{
	return true;
}

void UModioFeaturedView::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (const UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (!Settings->BrowserCategoryConfiguration.IsNull())
		{
			if (UModioModBrowserParams* ModBrowserParams = Settings->BrowserCategoryConfiguration.LoadSynchronous())
			{
				AdditionalCategories->SetListItems(ModBrowserParams->AdditionalCategoryParams);
				AdditionalCategories->SetSelectedIndex(0);
			}
		}
	}
}

// Regenerates mod tiles on log in / out to update subscribe button and subscription indicators
void UModioFeaturedView::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	FetchPrimaryCategoryMods();
	AdditionalCategories->RegenerateAllEntries();
	SynchronizeProperties();
}
