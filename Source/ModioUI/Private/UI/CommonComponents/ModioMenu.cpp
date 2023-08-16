/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */
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
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Interfaces/IModioUIDialogButtonWidget.h"
#include "UI/Interfaces/IModioMenuBackgroundProvider.h"
#include "UI/Interfaces/IModioUIDownloadQueueWidget.h"
#include "UI/Interfaces/IModioUIRefineSearchWidget.h"
#include "UI/Notifications/ModioNotificationWidgetBase.h"
#include "UI/Views/ModDetails/ModioModDetailsView.h"
#include "UI/Views/SearchResults/ModioSearchResultsView.h"
#include "UObject/WeakInterfacePtr.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Components/OverlaySlot.h"

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
	GEngine->GetEngineSubsystem<UModioUISubsystem>()->SetActiveTabIndex(!ViewController->ActiveWidgetIndex);
}

void UModioMenu::ChangePageNext()
{
	// Takes advantage of the two pages we want to swap between being index 0 and 1. If we add additional pages this
	// will have to change
	GEngine->GetEngineSubsystem<UModioUISubsystem>()->SetActiveTabIndex(!ViewController->ActiveWidgetIndex);
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
	if (DialogController && DialogController->TrySetFocusToActiveDialog())
	{
		return;
	}

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
		else 
		{
			SetFocus();
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
				else
				{
					OnDownloadQueueClosed.Broadcast();
				}
			}
		}
		else
		{
			if (DialogController)
			{
				DialogController->ShowAuthenticationChoiceDialog();
			}

			if (DialogAnim)
			{
				PlayAnimation(DialogAnim);
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
			SetKeyboardFocus();
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

	if (IsValid(DialogController) && DialogController->GetActualDialog())
	{
		DialogController->FinalizeDialog();
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
				UModioCollectionView* collectionView = Cast<UModioCollectionView>(CollectionViewInstance);
				if (collectionView)
				{
					collectionView->OnProfileOpened.AddUObject(this, &UModioMenu::ToggleDownloadQueueDrawer);
				}
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

	CachedBacgroundCursorType = Background->Cursor;

	//if (DrawerController)
	//{
	//	if (WidgetTree)
	//	{
	//		/*
	//					if (DownloadProgressDrawer)
	//					{
	//						UModioDrawer* DownloadProgress = WidgetTree->ConstructWidget<UModioDrawer>(this,
	//		   DownloadProgressDrawer); UModioDrawerControllerSlot* DLSlot =
	//		   Cast<UModioDrawerControllerSlot>(DrawerController->AddChildToOverlay(DownloadProgress)); if (DLSlot)
	//						{
	//							DLSlot->SetSlotRightEdge(VAlign_Fill);
	//						}
	//					}
	//					if (RefineSearchDrawer)
	//					{
	//						UModioDrawer* RefineSearch =
	//							WidgetTree->ConstructWidget<UModioDrawer>(this, RefineSearchDrawer);
	//						UModioDrawerControllerSlot* RefineSlot =
	//							Cast<UModioDrawerControllerSlot>(DrawerController->AddChildToOverlay(RefineSearch));
	//						if (RefineSlot)
	//						{
	//							RefineSlot->SetSlotRightEdge(VAlign_Fill);
	//						}
	//					}
	//		*/
	//	}
	//}
}

void UModioMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Listen for UserChanged events to reset view on log in/out
	IModioUIUserChangedReceiver::Register<UModioMenu>();
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->EnableModManagement();
		// Cache tags for this game
		Subsystem->GetTagOptionsListAsync();
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

	if (Subsystem && Subsystem->GetDefaultStyleSet())
	{
		Background->SetBrushFromMaterial(Subsystem->GetDefaultStyleSet()->NativeGetBackgroundMaterial());

		if (IsValid(Subsystem->GetDefaultStyleSet()->GetHideCursorWidgetClass()))
		{
			CreateHideMouseCursorWidget(Subsystem->GetDefaultStyleSet()->GetHideCursorWidgetClass());
		}
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
				for (UWidget* DrawerControllerChild : DrawerController->GetAllChildren())
				{
					if (DrawerControllerChild->Implements<UModioUIRefineSearchWidget>())
					{
						FString searchString =
							IModioUIRefineSearchWidget::Execute_GetSearchString(DrawerControllerChild);
						View->SetSearchInputString(searchString);
					}
				}

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
	if (UModioModDetailsView* View = Cast<UModioModDetailsView>(ViewController->GetActiveWidget()))
	{
		View->ShowDetailsForMod(ID);
	}
}

bool UModioMenu::RequestShowSearchResults(FModioFilterParams Params)
{
	UpdateRefineSearchDrawerSettings(Params);
	OnSearchSettingsChanged(Params);
	return true;
}

UModioDialogController* UModioMenu::GetDialogController()
{
	return DialogController;
}

UModioDrawerController* UModioMenu::GetDrawerController()
{
	return DrawerController;
}

void UModioMenu::HandleViewChanged(int32 ViewIndex)
{
	bRoutedOnViewChanged = false;
	if (ViewChangedAnim)
	{
		PlayAnimation(ViewChangedAnim);
	}
	NativeOnViewChanged(ViewIndex);
	checkf(bRoutedOnViewChanged, TEXT("Overridden implementations of NativeOnViewChanged should call "
									  "Super::NativeOnViewChanged to ensure events are routed to blueprint"));
}

void UModioMenu::NativeOnViewChanged(int64 ViewIndex)
{
	bRoutedOnViewChanged = true;
	OnViewChanged(ViewIndex);
	if (ViewController)
	{
		UModioUISubsystem* subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (subsystem)
		{
			subsystem->SetCurrentFocusTarget(nullptr);
		}

		UModioScrollBox* OuterScrollBox = Cast<UModioScrollBox>(ViewController->GetActiveWidget());
		if (OuterScrollBox)
		{
			OuterScrollBox->SetAlwaysShowScrollbar(false);
			OuterScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
			if (UModioMenuView* MenuView = Cast<UModioMenuView>(OuterScrollBox->GetChildAt(0)))
			{
				ActiveViewWidget = MenuView;
				if (MenuBar)
				{
					MenuBar->SetSearchButtonVisibility(MenuView->ShouldShowSearchButtonForMenu());
					MenuBar->SetBackButtonVisibility(MenuView->ShouldShowBackButtonForMenu());
				}
				ModioMenuBarWidget->SetVisibility(MenuView->ShouldShowTopNavBar() ? ESlateVisibility::Visible
																				  : ESlateVisibility::Collapsed);
				MenuView->SetKeyboardFocus();
			}
		}
		UWidget* widget = ViewController->GetActiveWidget();
		if (widget)
		{
			if (UModioMenuView* MenuView = Cast<UModioMenuView>(widget))
			{
				ActiveViewWidget = MenuView;
				if (MenuBar)
				{
					MenuBar->SetSearchButtonVisibility(MenuView->ShouldShowSearchButtonForMenu());
					MenuBar->SetBackButtonVisibility(MenuView->ShouldShowBackButtonForMenu());
				}

				ModioMenuBarWidget->SetVisibility(MenuView->ShouldShowTopNavBar() ? ESlateVisibility::Visible
																				  : ESlateVisibility::Collapsed);

				MenuView->SetKeyboardFocus();
			}
		}
	}
	bMenuFocused = false;
}

void UModioMenu::NativeTick(const FGeometry& InGeometry, float DeltaTime) 
{
	if (DialogController->TrySetFocusToActiveDialog())
	{
		ViewController->SetVisibility(ESlateVisibility::HitTestInvisible);
		bMenuFocused = false;
		return;
	}

	if (DrawerController->SetFocusToActiveDrawer())
	{
		bMenuFocused = false;
		return;
	}

	ViewController->SetVisibility(ESlateVisibility::Visible);
	if (!bMenuFocused) 
	{	
		bMenuFocused = true;
		if (ActiveViewWidget)
		{
			ActiveViewWidget->SetKeyboardFocus();
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
	// If we ever receive full focus on the Menu, forward it to the correct menu
	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	}

	if (ActiveViewWidget)
	{
		ActiveViewWidget->SetKeyboardFocus();
	}
	DialogController->TrySetFocusToActiveDialog();

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UModioMenu::ShowAuthenticationChoiceDialog()
{
	if (DialogController)
	{
		DialogController->ShowAuthenticationChoiceDialog();
	}

	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
	}
}

void UModioMenu::ShowLogoutDialog()
{
	if (DialogController)
	{
		DialogController->ShowLogoutDialog();
	}

	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
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
	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
	}
}

void UModioMenu::ShowReportEmailDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowReportEmailDialog(DialogDataSource);
	}
	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
	}
}

void UModioMenu::ShowUninstallConfirmationDialog(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowUninstallConfirmationDialog(DialogDataSource);
	}
	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
	}
}

void UModioMenu::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	UModioUISubsystem* subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (subsystem)
	{
		subsystem->SetCurrentFocusTarget(nullptr);
	}
	// Reset view to Featured when a user logs out
	if (GetPageIndex() > 0)
	{
		if (DrawerController)
		{
			if (DrawerController->IsAnyDrawerExpanded())
			{
				DrawerController->CollapseAllDrawers();
			}
		}
		if (ViewController)
		{
			ViewController->SetActiveWidgetIndex(0);
		}
	}
	return;
}

void UModioMenu::SetShowCursor(bool bShow)
{
	if (HideCursorWidget)
	{
		HideCursorWidget->SetVisibility(bShow ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	

	if (!bShow)
	{
		FVector2D mousePos;
		GetOwningPlayer()->GetMousePosition(mousePos.X, mousePos.Y);

		mousePos.Y == 0 ? GetOwningPlayer()->SetMouseLocation(mousePos.X, mousePos.Y + 1)
						: GetOwningPlayer()->SetMouseLocation(mousePos.X, mousePos.Y - 1);
	}
}

void UModioMenu::CreateHideMouseCursorWidget(TSubclassOf<UUserWidget> widgetClass)
{
	HideCursorWidget = CreateWidget<UUserWidget>(this->GetRootWidget(), widgetClass);
	HideCursorWidget->AddToViewport(1000);
	HideCursorWidget->SetVisibility(ESlateVisibility::Collapsed);
}
