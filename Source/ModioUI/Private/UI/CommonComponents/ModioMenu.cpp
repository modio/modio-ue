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
#include "Components/SizeBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Border.h"
#include "UI/BaseWidgets/ModioScrollBox.h"
#include "UI/BaseWidgets/Slots/ModioDrawerControllerSlot.h"
#include "UI/Commands/ModioCommonUICommands.h"
#include "UI/Commands/ModioUIMenuCommandList.h"
#include "UI/Dialogs/ModioDialogBaseInternal.h"
#include "UI/Drawers/RefineSearch/ModioRefineSearchDrawer.h"
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

void UModioMenu::HandleDialogClosed()
{
	TrySetFocus();
}

bool UModioMenu::CanExecutePageChange()
{
	if (IsValid(DialogController) && DialogController->DialogStack.Num() > 0)
	{
		return false;
	}

	if (DrawerController->IsAnyDrawerExpanded())
	{
		return false;
	}

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
	if (IsValid(DialogController) && DialogController->DialogStack.Num() > 0)
	{
		return false;
	}
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
	if (IsValid(DialogController) && DialogController->DialogStack.Num() > 0)
	{
		return false;
	}

	return true;
}

void UModioMenu::ChangePagePrevious()
{
	
	// Takes advantage of the two pages we want to swap between being index 0 and 1. If we add additional pages this
	// will have to change
	GEngine->GetEngineSubsystem<UModioUI4Subsystem>()->SetActiveTabIndex(!ViewController->ActiveWidgetIndex);
}

void UModioMenu::ChangePageNext()
{
	// Takes advantage of the two pages we want to swap between being index 0 and 1. If we add additional pages this
	// will have to change
	GEngine->GetEngineSubsystem<UModioUI4Subsystem>()->SetActiveTabIndex(!ViewController->ActiveWidgetIndex);
}

void UModioMenu::SetPageIndex(int Index)
{
	if (ViewController)
	{
		if (ViewController->GetActiveWidgetIndex() != Index)
		{
			if (Index == (int32) EModioMenuScreen::EMMS_ModDetails)
			{
				ShowDetailsForMod_Implementation(CachedModID);
			}
			else
			{
				ViewController->SetActiveWidgetIndex(Index);
			}
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

void UModioMenu::CacheCurrentPage()
{
	CachedPageIndex = GetPageIndex();
}

void UModioMenu::RestoreCachedPage()
{
	SetPageIndex(CachedPageIndex);
}

bool UModioMenu::GetIsCollectionModDisableUIEnabled_Implementation()
{
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		return Settings->bEnableCollectionModDisableUI;
	}
	return false;
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
				}
			}
		}
	}
}


bool UModioMenu::IsAnyDrawerExpanded()
{
	if (DrawerController)
	{
		return DrawerController->IsAnyDrawerExpanded();
	}

	return false;
}

void UModioMenu::RefreshDownloadQueue()
{
	if(DrawerController)
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

	if (const UModioUISettings* CurrentUISettings =
			UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		UHorizontalBoxSlot* slot = Cast<UHorizontalBoxSlot>(MenuSizeBox->Slot);
		if (slot)
		{
			slot->SetHorizontalAlignment(CurrentUISettings->bEnableCenteredUI ? EHorizontalAlignment::HAlign_Center
																			  : EHorizontalAlignment::HAlign_Fill);
			slot->SetSize(CurrentUISettings->bEnableCenteredUI ? ESlateSizeRule::Automatic : ESlateSizeRule::Fill);
			ESlateVisibility blurBorderVisibility =
				CurrentUISettings->bEnableCenteredUI ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
			LeftBlurBorder->SetVisibility(blurBorderVisibility);
			RightBlurBorder->SetVisibility(blurBorderVisibility);
			MenuSizeBox->SetWidthOverride(CurrentUISettings->MaxMenuWidth);
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

	if (DialogController)
	{
		DialogController->OnDialogClosed.AddDynamic(this, &UModioMenu::HandleDialogClosed);
	}

	// Listen for UserChanged events to reset view on log in/out
	IModioUIUserChangedReceiver::Register<UModioMenu>();
	IModioUIActionHandler::Register<UModioMenu>();
	UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	if (Subsystem)
	{
		Subsystem->EnableModManagement();
		// Cache tags for this game
		Subsystem->GetTagOptionsListAsync();

		Subsystem->OnUserChanged.AddWeakLambda(this, [this](TOptional<FModioUser> User) {
			ForceCloseDownloadDrawer();
			RestoreCachedPage();
		});

		Subsystem->OnAuthenticationChangeStarted.AddWeakLambda(this, [this]()
		{
			CacheCurrentPage();
		});

		Subsystem->OnModManagementEvent.AddWeakLambda(
			this, [this](FModioModManagementEvent Event)
			{
				RefreshDownloadQueue();
			});
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

	UModioUI4Subsystem* Subsystem4 = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (Subsystem4 && Subsystem4->GetDefaultStyleSet())
	{
		Background->SetBrushFromMaterial(Subsystem4->GetDefaultStyleSet()->NativeGetBackgroundMaterial());

		Subsystem4->OnMenuTabIndexChanged.AddUniqueDynamic(this, &UModioMenu::SetPageIndex);
		Subsystem4->OnGetCurrentTabIndex.BindUObject(ViewController, &UModioWidgetSwitcher::GetActiveWidgetIndex);
		Subsystem4->OnGetIsDialogStackOpen.BindUObject(this, &UModioMenu::GetIsAnyDialogOpen);
		
		if (IsValid(Subsystem4->GetDefaultStyleSet()->GetHideCursorWidgetClass()))
		{
			CreateHideMouseCursorWidget(Subsystem4->GetDefaultStyleSet()->GetHideCursorWidgetClass());
		}
	}
}

bool UModioMenu::GetIsAnyDialogOpen()
{
	return DialogController->DialogStack.Num() > 0;
}

void UModioMenu::NativeOnCursorVisibilityChanged(bool bNewVisibility)
{
	SetShowCursor(bNewVisibility);
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

void UModioMenu::ShowDetailsForMod_Implementation(FModioModID ID)
{
	int32 UnderlyingWidgetIndex = (int32) EModioMenuScreen::EMMS_ModDetails;

	ViewController->PushActiveWidgetIndex(UnderlyingWidgetIndex);
	if (UModioModDetailsView* View = Cast<UModioModDetailsView>(ViewController->GetActiveWidget()))
	{
		CachedModID = ID;
		View->ShowDetailsForMod(ID);
	}
}

void UModioMenu::RequestExternalAuthentication_Implementation(EModioAuthenticationProvider Provider)
{
	IModioModBrowserInterface::RequestExternalAuthentication_Implementation(Provider);
	// Query the settings, get the authentication data provider, invoke the provider to fill in the rest of the params
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (UClass* AuthProviderClass = Settings->AuthenticationDataProvider.Get())
		{
			UObject* TmpProvider = NewObject<UObject>(this, AuthProviderClass);
			FModioAuthenticationParams NewParams =
				IModioUIAuthenticationDataProvider::Execute_GetAuthenticationParams(TmpProvider, Provider);
			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
				{
					Subsystem->AuthenticateUserExternalAsync(
						NewParams, Provider,
						FOnErrorOnlyDelegateFast::CreateUObject(UISubsystem, &UModioUISubsystem::OnAuthenticationComplete));
				}
			}
		}
	}
}

void UModioMenu::RequestExternalAuthenticationNative(EModioAuthenticationProvider Provider, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	IModioModBrowserInterface::RequestExternalAuthenticationNative(Provider, DedicatedCallback);
	// Query the settings, get the authentication data provider, invoke the provider to fill in the rest of the params
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (UClass* AuthProviderClass = Settings->AuthenticationDataProvider.Get())
		{
			UObject* TmpProvider = NewObject<UObject>(this, AuthProviderClass);
			FModioAuthenticationParams NewParams =
				IModioUIAuthenticationDataProvider::Execute_GetAuthenticationParams(TmpProvider, Provider);
			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
				{
					Subsystem->AuthenticateUserExternalAsync(
						NewParams, Provider,
						FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
																	UISubsystem, &UModioUISubsystem::OnAuthenticationComplete),
																DedicatedCallback](FModioErrorCode ec) {
							DedicatedCallback.ExecuteIfBound(ec);
							HookedHandler.ExecuteIfBound(ec);
						}));
				}
			}
		}
	}
}

void UModioMenu::ShowReportMod_Implementation(UObject* DialogDataSource)
{
	IModioModBrowserInterface::ShowReportMod_Implementation(DialogDataSource);
	if (DialogController)
	{
		DialogController->ShowModReportDialog(DialogDataSource);
	}
	if (DialogAnim)
	{
		PlayAnimation(DialogAnim);
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

void UModioMenu::RefreshView(int32 ViewIndex)
{
	NativeOnViewChanged(ViewIndex);
}

void UModioMenu::NativeOnViewChanged(int64 ViewIndex)
{
	bRoutedOnViewChanged = true;
	OnViewChanged(ViewIndex);
	if (ViewController)
	{
		UModioUI4Subsystem* subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
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
			}
		}
	}
	bMenuFocused = false;
}

void UModioMenu::NativeTick(const FGeometry& InGeometry, float DeltaTime)
{
	Super::NativeTick(InGeometry, DeltaTime);
	UpdateMenuFocus();
}

FReply UModioMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	if (ProcessCommandForEvent(InPointerEvent))
	{
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InPointerEvent);
}

FReply UModioMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsValid(DialogController) && DialogController->DialogStack.Num() > 0)
	{
		return FReply::Handled();
	}

	if (ProcessCommandForEvent(InKeyEvent))
	{
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UModioMenu::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	// If we ever receive full focus on the Menu, forward it to the correct menu
	UpdateMenuFocus();
	return FReply::Handled();
}

void UModioMenu::TrySetFocus()
{
	if (DialogController->TrySetFocusToActiveDialog())
	{
		ViewController->SetVisibility(ESlateVisibility::HitTestInvisible);
		DialogController->SetRenderOpacity(1.f);
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
		UModioUI4Subsystem* subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
		if (ActiveViewWidget && !ActiveViewWidget->HasFocusedDescendants() && (
									subsystem->GetLastInputDevice() == EModioUIInputMode::Keyboard ||
									subsystem->GetLastInputDevice() == EModioUIInputMode::Controller))
		{
			bMenuFocused = true;
			ActiveViewWidget->SetKeyboardFocus();
		}
	}
}

void UModioMenu::ShowUserAuth_Implementation()
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

void UModioMenu::LogOut_Implementation()
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

void UModioMenu::ShowModUnsubscribeDialog_Implementation(UObject* DialogDataSource)
{
	if (DialogController)
	{
		DialogController->ShowModUnsubscribeDialog(DialogDataSource);
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
	UModioUI4Subsystem* subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
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
		TrySetFocus();
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

void UModioMenu::UpdateMenuFocus()
{
	UModioUI4Subsystem* subsystem = GEngine->GetEngineSubsystem<UModioUI4Subsystem>();
	if (subsystem && subsystem->GetLastInputDevice() == EModioUIInputMode::Mouse)
	{
		return;
	}

	if (IsValid(DialogController))
	{
		if (DialogController->TrySetFocusToActiveDialog())
		{
			DialogController->SetVisibility(ESlateVisibility::Visible);
			DialogController->SetRenderOpacity(1.f);
			bMenuFocused = false;
			return;
		}

		if (DialogController->DialogStack.Num() > 0)
		{
			bMenuFocused = false;
			return;
		}
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
			if (subsystem->GetCurrentFocusTarget())
			{
				subsystem->GetCurrentFocusTarget()->SetKeyboardFocus();
				return;
			}
			ActiveViewWidget->SetKeyboardFocus();
		}
	}
}

