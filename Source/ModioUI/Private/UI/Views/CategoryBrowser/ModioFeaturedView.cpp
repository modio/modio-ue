// Fill out your copyright notice in the Description page of Project Settings.

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
	RealCategory->OnCategorySelectionChanged().AddDynamic(this, &UModioFeaturedView::CategorySelectionChanged);
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
		if (UModioUISettings* Settings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
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
	return FReply::Handled().SetUserFocus(PrimaryFeaturedCategory->TakeWidget());
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
	if (UModioUISettings* Settings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
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
