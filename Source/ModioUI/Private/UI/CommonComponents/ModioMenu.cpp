// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CommonComponents/ModioMenu.h"
#include "Algo/Transform.h"
#include "Core/ModioFilterParamsUI.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Input/Reply.h"
#include "Settings/ModioUISettings.h"
#include "UI/BaseWidgets/ModioScrollBox.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UI/Interfaces/IModioMenuBackgroundProvider.h"
#include "UI/Interfaces/IModioUIDownloadQueueWidget.h"
#include "UI/Interfaces/IModioUIRefineSearchWidget.h"
#include "UI/Notifications/ModioNotificationWidgetBase.h"
#include "UI/Views/ModDetails/ModioModDetailsView.h"
#include "UI/Views/SearchResults/ModioSearchResultsView.h"
#include "UObject/WeakInterfacePtr.h"
#include "Widgets/Notifications/SNotificationList.h"

bool UModioMenu::CanExecutePageChange()
{
	// Only allow the user to change the page if we're currently displaying Collection or Browse
	if (ViewController)
	{
		int32 CurrentPage = ViewController->GetActiveWidgetIndex();
		return CurrentPage == (int32) EModioMenuScreen::EMMS_Collection ||
			   CurrentPage == (int32) EModioMenuScreen::EMMS_Featured;
	}
	return false;
}

bool UModioMenu::CanShowRefineSearch()
{
	// Only show the refine search results if we're currently displaying Browse or SearchResults
	if (ViewController)
	{
		int32 CurrentPage = ViewController->GetActiveWidgetIndex();
		return CurrentPage == (int32) EModioMenuScreen::EMMS_SearchResults ||
			   CurrentPage == (int32) EModioMenuScreen::EMMS_Featured;
	}
	return false;
}

bool UModioMenu::CanShowDownloadQueue()
{
	return true;
}

void UModioMenu::ChangePagePrevious()
{
	// Takes advantage of the two pages we want to swap between being index 0 and 1. If we add additional pages this
	// will have to change
	if (ViewController)
	{
		ViewController->SetActiveWidgetIndex(!ViewController->ActiveWidgetIndex);
	}
}

void UModioMenu::ChangePageNext()
{
	// Takes advantage of the two pages we want to swap between being index 0 and 1. If we add additional pages this
	// will have to change
	if (ViewController)
	{
		ViewController->SetActiveWidgetIndex(!ViewController->ActiveWidgetIndex);
	}
}

void UModioMenu::SetPageIndex(int Index)
{
	if (ViewController)
	{
		if (ViewController->GetActiveWidgetIndex() != Index)
		{
			ViewController->SetActiveWidgetIndex(Index);
		}
	}
}

int32 UModioMenu::GetPageIndex()
{
	if (ViewController)
	{
		return ViewController->GetActiveWidgetIndex();
	}

	return INDEX_NONE;
}

void UModioMenu::ToggleRefineSearchDrawer()
{
	if (DrawerController)
	{
		if (DrawerController->ToggleDrawerExpanded((int32) EModioMenuDrawer::EMMD_RefineSearch))
		{
			// Only bother to update the drawer if we're going to display it, not hide it
			for (UWidget* Child : DrawerController->GetAllChildren())
			{
				if (Child->Implements<UModioUIRefineSearchWidget>())
				{
					IModioUIRefineSearchWidget::Execute_RefreshTags(Child);
					FSlateApplication::Get().SetUserFocus(0, Child->TakeWidget(), EFocusCause::Navigation);
				}
			}
		}
	}
}

void UModioMenu::RefreshDownloadQueue()
{
	if (DrawerController)
	{
		if (DrawerController)
		{
			for (UWidget* Child : DrawerController->GetAllChildren())
			{
				if (Child->Implements<UModioUIDownloadQueueWidget>())
				{
					IModioUIDownloadQueueWidget::Execute_RefreshDownloadQueue(Child);
				}
			}
		}
	}
}

void UModioMenu::ToggleDownloadQueueDrawer()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioUser> User = Subsystem->QueryUserProfile())
		{
			if (DrawerController)
			{
				if (DrawerController->ToggleDrawerExpanded((int32) EModioMenuDrawer::EMMD_DownloadQueue))
				{
					// Only bother to update the drawer if we're going to display it, not hide it
					for (UWidget* Child : DrawerController->GetAllChildren())
					{
						if (Child->Implements<UModioUIDownloadQueueWidget>())
						{
							IModioUIDownloadQueueWidget::Execute_RefreshDownloadQueue(Child);
							FModioOptionalUser UserWrapper {User};
							IModioUIDownloadQueueWidget::Execute_DisplayUserInfo(Child, UserWrapper);
							FSlateApplication::Get().SetUserFocus(0, Child->TakeWidget(), EFocusCause::SetDirectly);
						}
					}
				}
			}
		}
		else
		{
			if (DialogController)
			{
				DialogController->ShowAuthenticationChoiceDialog();
			}
		}
	}
}
bool UModioMenu::CanGoToPreviousSubmenu()
{
	return true;
};

void UModioMenu::GoToPreviousSubmenu()
{
	if (DrawerController)
	{
		if (DrawerController->IsAnyDrawerExpanded())
		{
			DrawerController->CollapseAllDrawers();
			return;
		}
	}

	if (ViewController)
	{
		if (ViewController->PopActiveWidgetIndex() == INDEX_NONE)
		{
			if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
			{
				Subsystem->ExecuteOnModBrowserClosedDelegate();
			}
		}
	}
}

void UModioMenu::BuildCommandList(TSharedRef<FUICommandList> CommandList)
{
	CommandList->MapAction(FModioCommonUICommands::Get().PreviousPage,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioMenu::ChangePagePrevious),
									 FCanExecuteAction::CreateUObject(this, &UModioMenu::CanExecutePageChange)));

	CommandList->MapAction(FModioCommonUICommands::Get().NextPage,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioMenu::ChangePageNext),
									 FCanExecuteAction::CreateUObject(this, &UModioMenu::CanExecutePageChange)));
	CommandList->MapAction(FModioCommonUICommands::Get().RefineSearch,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioMenu::ToggleRefineSearchDrawer),
									 FCanExecuteAction::CreateUObject(this, &UModioMenu::CanShowRefineSearch)));
	CommandList->MapAction(FModioCommonUICommands::Get().DownloadQueue,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioMenu::ToggleDownloadQueueDrawer),
									 FCanExecuteAction::CreateUObject(this, &UModioMenu::CanShowDownloadQueue)));
	CommandList->MapAction(FModioCommonUICommands::Get().Back,
						   FUIAction(FExecuteAction::CreateUObject(this, &UModioMenu::GoToPreviousSubmenu),
									 FCanExecuteAction::CreateUObject(this, &UModioMenu::CanGoToPreviousSubmenu)));
}


void UModioMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (MenuBar)
	{
		if (ViewController && WidgetTree)
		{
			UModioMenuView* FeaturedViewInstance = CreateWidget<UModioMenuView>(this, FeaturedView);
			if (FeaturedViewInstance)
			{
				UModioScrollBox* ScrollWrapper = WidgetTree->ConstructWidget<UModioScrollBox>();

				ScrollWrapper->ScrollWhenFocusChanges = EScrollWhenFocusChanges::AnimatedScroll;
				ScrollWrapper->AddChild(FeaturedViewInstance);
				ViewController->AddChild(ScrollWrapper);
			}
			UModioMenuView* CollectionViewInstance = CreateWidget<UModioMenuView>(this, CollectionView);
			if (CollectionViewInstance)
			{
				// UModioScrollBox* ScrollWrapper = WidgetTree->ConstructWidget<UModioScrollBox>();
				// ScrollWrapper->ScrollWhenFocusChanges = EScrollWhenFocusChanges::AnimatedScroll;
				// ScrollWrapper->AddChild(CollectionViewInstance);
				ViewController->AddChild(CollectionViewInstance);
			}
			UModioMenuView* SearchResultsViewInstance = CreateWidget<UModioMenuView>(this, SearchResultsView);
			if (SearchResultsViewInstance)
			{
				// UModioScrollBox* ScrollWrapper = WidgetTree->ConstructWidget<UModioScrollBox>();
				// ScrollWrapper->ScrollWhenFocusChanges = EScrollWhenFocusChanges::AnimatedScroll;
				// ScrollWrapper->AddChild(SearchResultsViewInstance);
				ViewController->AddChild(SearchResultsViewInstance);
			}
			UModioMenuView* ModDetailsViewInstance = CreateWidget<UModioMenuView>(this, ModDetailsView);
			if (ModDetailsViewInstance)
			{
				ViewController->AddChild(ModDetailsViewInstance);
			}
			ViewController->GetActiveWidgetChangedDelegate().AddUniqueDynamic(this, &UModioMenu::HandleViewChanged);
			ViewController->SetActiveWidgetIndex((int32) EModioMenuScreen::EMMS_Featured);
		}
	}
	if (DrawerController)
	{
		if (WidgetTree)
		{
			/*
						if (DownloadProgressDrawer)
						{
							UModioDrawer* DownloadProgress = WidgetTree->ConstructWidget<UModioDrawer>(this,
			   DownloadProgressDrawer); UModioDrawerControllerSlot* DLSlot =
			   Cast<UModioDrawerControllerSlot>(DrawerController->AddChildToOverlay(DownloadProgress)); if (DLSlot)
							{
								DLSlot->SetSlotRightEdge(VAlign_Fill);
							}
						}
						if (RefineSearchDrawer)
						{
							UModioDrawer* RefineSearch =
								WidgetTree->ConstructWidget<UModioDrawer>(this, RefineSearchDrawer);
							UModioDrawerControllerSlot* RefineSlot =
								Cast<UModioDrawerControllerSlot>(DrawerController->AddChildToOverlay(RefineSearch));
							if (RefineSlot)
							{
								RefineSlot->SetSlotRightEdge(VAlign_Fill);
							}
						}
			*/
		}
	}
}

void UModioMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (DrawerController)
	{
		int32 NumDrawers = DrawerController->GetChildrenCount();
		for (int32 ChildIndex = 0; ChildIndex < NumDrawers; ChildIndex++)
		{
			DrawerController->SetDrawerExpanded(ChildIndex, false);
		}
	}
}

void UModioMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->EnableModManagement();

		// Cache tags for this game
		Subsystem->GetTagOptionsList();
	}
	// Find the refine search drawer, bind to its confirmed event so we know to display the search results
	if (DrawerController)
	{
		for (UWidget* Child : DrawerController->GetAllChildren())
		{
			// Non-UInterface function requires explicit cast and interface call
			if (Child->Implements<UModioUIRefineSearchWidget>())
			{
				Cast<IModioUIRefineSearchWidget>(Child)->GetOnSettingsChangedDelegate().AddDynamic(
					this, &UModioMenu::OnSearchSettingsChanged);
			}
		}
	}
	if (ViewController)
	{
		ViewController->SetClipping(EWidgetClipping::ClipToBoundsAlways);
	}

	bool bHasSetBackground = false;
	if (Background)
	{
		if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
		{
			if (UClass* BackgroundImageProviderClass = Settings->BackgroundImageProvider.Get())
			{
				if (UObject* TmpBackgroundProvider = NewObject<UObject>(this, BackgroundImageProviderClass))
				{
					if (UMaterialInterface *BackgroundMaterial = IModioMenuBackgroundProvider::Execute_GetBackgroundMaterial(TmpBackgroundProvider))
					{
						Background->SetBrushFromMaterial(BackgroundMaterial);
						bHasSetBackground = true;
					}
				}
			}
		}
	}

	if (bHasSetBackground == false)
	{
		Background->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UModioMenu::UpdateRefineSearchDrawerSettings(FModioFilterParams Settings)
{
	for (UWidget* Child : DrawerController->GetAllChildren())
	{
		if (Child->Implements<UModioUIRefineSearchWidget>())
		{
			IModioUIRefineSearchWidget::Execute_NotifySettingsChanged(Child, Settings);
		}
	}
}

void UModioMenu::OnSearchSettingsChanged(FModioFilterParams Settings)
{
	ViewController->PushActiveWidgetIndex((int32) EModioMenuScreen::EMMS_SearchResults);
	if (UWidget* ActiveWidget = ViewController->GetActiveWidget())
	{
		TArray<UWidget*> Children;

		if (UUserWidget* AsUserWidget = Cast<UUserWidget>(ActiveWidget))
		{
			AsUserWidget->WidgetTree->GetAllWidgets(Children);
		}
		// Either we didn't have a user widget so this Active Widget is effectively a leaf, or ActiveWidget has children
		// but is itself the widget we want to scan
		Children.Add(ActiveWidget);

		for (UWidget* Child : Children)
		{
			if (UModioSearchResultsView* View = Cast<UModioSearchResultsView>(Child))
			{
				UModioFilterParamsUI* Data = NewObject<UModioFilterParamsUI>();
				Data->Underlying = Settings;

				View->SetDataSource(Data);
			}
		}
	}
}

void UModioMenu::ShowDetailsForMod(FModioModID ID)
{
	int32 UnderlyingWidgetIndex = (int32) EModioMenuScreen::EMMS_ModDetails;

	ViewController->PushActiveWidgetIndex(UnderlyingWidgetIndex);
	if (UWidget* ActiveWidget = ViewController->GetActiveWidget())
	{
		if (UModioModDetailsView* View = Cast<UModioModDetailsView>(ActiveWidget))
		{
			View->ShowDetailsForMod(ID);
		}
	}
}

bool UModioMenu::RequestShowSearchResults(FModioFilterParams Params)
{
	UpdateRefineSearchDrawerSettings(Params);
	OnSearchSettingsChanged(Params);
	return true;
}

void UModioMenu::HandleViewChanged(int32 ViewIndex)
{
	bRoutedOnViewChanged = false;
	NativeOnViewChanged(ViewIndex);
	checkf(bRoutedOnViewChanged, TEXT("Overridden implementations of NativeOnViewChanged should call "
									  "Super::NativeOnViewChanged to ensure events are routed to blueprint"));
}

void UModioMenu::NativeOnViewChanged(int64 ViewIndex)
{
	bRoutedOnViewChanged = true;
	OnViewChanged(ViewIndex);
	if (ViewController && MenuTitleContent)
	{
		UModioScrollBox* OuterScrollBox = Cast<UModioScrollBox>(ViewController->GetActiveWidget());
		if (OuterScrollBox)
		{
			if (UModioMenuView* MenuView = Cast<UModioMenuView>(OuterScrollBox->GetChildAt(0)))
			{
				MenuTitleContent->SetContent(MenuView->GetMenuTitleContent());
				if (MenuBar)
				{
					MenuBar->SetSearchButtonVisibility(MenuView->ShouldShowSearchButtonForMenu());
				}
				FSlateApplication::Get().SetKeyboardFocus(MenuView->TakeWidget(), EFocusCause::Navigation);
			}
		}
		UWidget* ActiveWidget = ViewController->GetActiveWidget();
		if (ActiveWidget)
		{
			if (UModioMenuView* MenuView = Cast<UModioMenuView>(ActiveWidget))
			{
				MenuTitleContent->SetContent(MenuView->GetMenuTitleContent());
				if (MenuBar)
				{
					MenuBar->SetSearchButtonVisibility(MenuView->ShouldShowSearchButtonForMenu());
				}

				FSlateApplication::Get().SetKeyboardFocus(MenuView->TakeWidget(), EFocusCause::Navigation);
			}
		}
	}
}

FReply UModioMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UModioMenu::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled()
		.SetNavigation(ViewController->GetActiveWidget()->TakeWidget(), ENavigationGenesis::User)
		.SetUserFocus(ViewController->GetActiveWidget()->TakeWidget(), EFocusCause::SetDirectly);
}

void UModioMenu::ShowAuthenticationChoiceDialog()
{
	if (DialogController)
	{
		DialogController->ShowAuthenticationChoiceDialog();
	}
}

void UModioMenu::ShowLogoutDialog()
{
	if (DialogController)
	{
		DialogController->ShowLogoutDialog();
	}
}

void UModioMenu::ShowModUnsubscribeDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowModUnsubscribeDialog(DialogDataSource);
	}
}

void UModioMenu::ShowModReportDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowModReportDialog(DialogDataSource);
	}
}

void UModioMenu::ShowReportEmailDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowReportEmailDialog(DialogDataSource);
	}
}

void UModioMenu::ShowUninstallConfirmationDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowUninstallConfirmationDialog(DialogDataSource);
	}
}
