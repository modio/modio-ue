// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Core/Input/ModioInputKeys.h"
#include "Core/Input/ModioUIInputProcessor.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Math/IntPoint.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Settings/ModioUISettings.h"
#include "UI/CommonComponents/ModioMenu.h"
#include "UI/Interfaces/IModioUIAuthenticationDataProvider.h"
#include "UI/Interfaces/IModioUIInputHintGlyphProvider.h"
#include "UI/Interfaces/IModioUINotification.h"
#include "UI/Styles/ModioUIStyleSet.h"

#include "Loc/BeginModioLocNamespace.h"

void UModioUISubsystem::GetPreloadDependencies(TArray<UObject*>& OutDeps)
{
	Super::GetPreloadDependencies(OutDeps);

	if (LoadedDefaultStyleSet != nullptr)
	{
		OutDeps.Add(LoadedDefaultStyleSet);
	}
}

bool UModioUISubsystem::QueryIsModEnabled(FModioModID ID)
{
	if (GetModEnabledDelegate.IsBound())
	{
		return GetModEnabledDelegate.Execute(ID);
	}
	else
	{
		return false;
	}
}

void UModioUISubsystem::RequestModEnabledState(FModioModID ID, bool bNewEnabledState)
{
	if (OnModEnabledChanged.IsBound())
	{
		OnModEnabledChanged.Execute(ID, bNewEnabledState);
	}
}

void UModioUISubsystem::ShowUserAuthenticationDialog()
{
	ModBrowserInstance->ShowAuthenticationChoiceDialog();
}

void UModioUISubsystem::ShowModUnsubscribeDialog(UObject* DialogDataSource)
{
	ModBrowserInstance->ShowModUnsubscribeDialog(DialogDataSource);
}

void UModioUISubsystem::ShowLogoutDialog()
{
	ModBrowserInstance->ShowLogoutDialog();
}

void UModioUISubsystem::SubscriptionHandler(FModioErrorCode ec, FModioModID ID)
{
	FString Message = ec.GetErrorMessage();
	int32 value = ec.GetValue();
	OnSubscriptionRequestCompleted.Broadcast(ec, ID);
	if (!ec)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, true);
	}
	DisplayErrorNotification(UModioNotificationParamsLibrary::CreateSubscriptionNotification(ec));
}

void UModioUISubsystem::UnsubscribeHandler(FModioErrorCode ec, FModioModID ID)
{
	if (!ec)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, false);
	}
}

void UModioUISubsystem::UninstallHandler(FModioErrorCode ec, FModioModID ID)
{
	// Need to create a synthetic FModioModManagementEvent to let the UI know an uninstallation has occurred.
	// UninstallHandler is used by ForceUninstallAsync, which does not emit a ModManagementEvent as it's an async
	// function with a callback, and is not used by the mod management loop.
	FModioModManagementEvent Event;
	Event.ID = ID;
	Event.Event = EModioModManagementEventType::Uninstalled;
	Event.Status = ec;
	OnModManagementEvent.Broadcast(Event);
}

void UModioUISubsystem::ModManagementEventHandler(FModioModManagementEvent Event)
{
	OnModManagementEvent.Broadcast(Event);

	// If we've installed a mod, add it to the session downloads for the download queue panel
	if (Event.Event == EModioModManagementEventType::Installed)
	{
		ModsDownloadedThisSession.Add(Event.ID);
	}

	// If we're uninstalled a mod, remove it from the session details
	if (Event.Event == EModioModManagementEventType::Uninstalled)
	{
		if (ModsDownloadedThisSession.Contains(Event.ID))
		{
			ModsDownloadedThisSession.Remove(Event.ID);	
		}
	}
	
	ModBrowserInstance->RefreshDownloadQueue();
}

void UModioUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UModioSubsystem::StaticClass());
	// TODO: @modio-ue4 allow the input processor class to be loaded from project settings
	Processor = MakeShared<FModioUIInputProcessor>();
	// Bind to event on the processor
	Processor->OnInputDeviceChanged().AddUObject(this, &UModioUISubsystem::HandleInputModeChanged);
	if (FSlateApplication::IsInitialized())
	{
		// Register the input processor so we can detect input device changes
		FSlateApplication::Get().RegisterInputPreProcessor(Processor.ToSharedRef());
		// Retrieve the navigation config factory and set it on the application so we can map our virtual inputs to
		// navigation directions and actions
		if (UModioUISettings* CurrentUISettings = GetMutableDefault<UModioUISettings>(UModioUISettings::StaticClass()))
		{
			if (!CurrentUISettings->NavigationConfigOverride.IsNull())
			{
				if (UModioNavigationConfigFactoryBase* FactoryInstance =
						CurrentUISettings->NavigationConfigOverride.LoadSynchronous())
				{
					FSlateApplication::Get().SetNavigationConfig(FactoryInstance->CreateNavigationConfig());
				}
			}
		}
	}

	GetDefaultStyleSet();
}

void UModioUISubsystem::EnableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
	{
		DependencySubsystem->EnableModManagement(
			FOnModManagementDelegateFast::CreateUObject(this, &UModioUISubsystem::ModManagementEventHandler));
	}
}

void UModioUISubsystem::DisableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
	{
		DependencySubsystem->DisableModManagement();
	}
}

void UModioUISubsystem::RequestSubscriptionForModID(FModioModID ID)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuthenticationDialog();
				return;
			}
		}
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::SubscriptionHandler, ID));
	}
}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															this, &UModioUISubsystem::UnsubscribeHandler, ID),
														DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ForceUninstallModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															this, &UModioUISubsystem::UninstallHandler, ID),
														DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestRateUpForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuthenticationDialog();
				return;
			}
		}
	}

	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Positive,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Positive),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateDownForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (!ModioSubsystem->QueryUserProfile().IsSet())
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->ShowUserAuthenticationDialog();
				return;
			}
		}
	}

	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Negative,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Negative),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::OnRatingSubmissionComplete(FModioErrorCode ModioErrorCode, EModioRating ModioRating) {}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::UnsubscribeHandler, ID));
	}
}

void UModioUISubsystem::RequestLogoDownloadForModID(FModioModID ID,
													EModioLogoSize LogoSize /*= EModioLogoSize::Thumb320*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, LogoSize, FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::LogoDownloadHandler, ID));
	}
}

float UModioUISubsystem::GetCurrentDPIScaleValue()
{
	FVector2D ViewportSize;

	GEngine->GameViewport->GetViewportSize(ViewportSize);
	return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
		->GetDPIScaleBasedOnSize(FIntPoint(FGenericPlatformMath::FloorToInt(ViewportSize.X),
										   FGenericPlatformMath::FloorToInt(ViewportSize.Y)));
}

void UModioUISubsystem::RequestModInfoForModIDs(TArray<FModioModID> IDs)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Bind the list of IDs as an additional parameter so that if we get an error we can report the error for the
		// specific IDs that were queried
		Subsystem->ListAllModsAsync(
			FModioFilterParams().MatchingIDs(IDs),
			FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ModInfoRequestCompletedHandler, IDs));
	}
}

void UModioUISubsystem::RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListAllModsAsync(
			Params, FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ListAllModsCompletedHandler,
															  RequestIdentifier));
	}
}

FText UModioUISubsystem::FormatText(FText Input)
{
	// Currently returns the text unmodified, this is a hook for replacing UI icons etc later.
	return Input;
}

UModioUIStyleSet* UModioUISubsystem::GetDefaultStyleSet()
{
	if (!LoadedDefaultStyleSet)
	{
		UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>();

		// Could we load the user's override?
		if (Settings && !Settings->DefaultStyleSet.IsNull())
		{
			LoadedDefaultStyleSet = Settings->DefaultStyleSet.LoadSynchronous();
		}
		else
		{
			FSoftObjectPath Fallback("/Modio/UI/Styles/ModioUIDefaultStyle.ModioUIDefaultStyle");
			LoadedDefaultStyleSet = Cast<UModioUIStyleSet>(Fallback.TryLoad());
		}
	}

	checkf(LoadedDefaultStyleSet, TEXT("Unable to load default style set for the Modio UI module. Please ensure this "
									   "is set in your project settings window."));

	return LoadedDefaultStyleSet;
}

UMaterialInterface* UModioUISubsystem::GetInputGlyphMaterialForInputType(FKey VirtualInput, EModioUIInputMode InputType)
{
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (UClass* GlyphProviderClass = Settings->InputHintGlyphProvider.Get())
		{
			if (UObject* TmpProvider = NewObject<UObject>(this, GlyphProviderClass))
			{
				if (UMaterialInterface* GlyphMaterial =
						IModioUIInputHintGlyphProvider::Execute_GetInputGlyphMaterialForInputType(
							TmpProvider, VirtualInput, InputType))
				{
					return GlyphMaterial;
				}
			}
		}
	}

	UModioUIStyleSet* DefaultStyleSet = GetDefaultStyleSet();
	if (DefaultStyleSet)
	{
		return DefaultStyleSet->GetInputGlyphMaterialForInputType(VirtualInput, InputType);
	}
	return nullptr;
}

TArray<FName> UModioUISubsystem::GetAllNamedStyleNames()
{
	TArray<FName> StyleNames;
	UModioUIStyleSet* DefaultStyleSet = GetDefaultStyleSet();
	if (DefaultStyleSet)
	{
		StyleNames = DefaultStyleSet->GetAllStyleNames();
	}
	return StyleNames;
}

void UModioUISubsystem::SetControllerOverrideType(EModioUIInputMode NewOverride)
{
	if (Processor)
	{
		Processor->SetControllerOverrideType(NewOverride);
	}
}

UModioMenu* UModioUISubsystem::ShowModBrowserUIForPlayer(TSubclassOf<UModioMenu> MenuClass,
														 APlayerController* Controller,
														 FOnModBrowserClosed BrowserClosedDelegate)
{
	if (UModioUISettings* Settings = UModioUISettings::StaticClass()->GetDefaultObject<UModioUISettings>())
	{
		if (MenuClass)
		{
			ModBrowserInstance = CreateWidget<UModioMenu>(Controller, MenuClass);
			ModBrowserInstance->AddToViewport();
			OnModBrowserClosed = BrowserClosedDelegate;
			OwningPlayerController = Controller;
			return ModBrowserInstance;
		}
	}
	return nullptr;
	// UE_LOG warn that the mod browser couldn't be displayed
}

void UModioUISubsystem::ExecuteOnModBrowserClosedDelegate()
{
	// Host application must bind BrowserClosedDelegate before creating ModBrowserUI
	OnModBrowserClosed.ExecuteIfBound();
}

void UModioUISubsystem::SendCommandToBrowserUI(FKey CommandKey, int32 UserIndex)
{
	if (FModioInputKeys::GetAll().Contains(CommandKey) && ModBrowserInstance)
	{
		FSlateApplication::Get().ProcessKeyDownEvent(
			FKeyEvent(CommandKey, FModifierKeysState {}, UserIndex, false, 0, 0));
	}
}

void UModioUISubsystem::ShowDetailsForMod(FModioModID ID)
{
	// OnDisplayModDetailsForID.Broadcast(ID);
	// For now we need to do this so that we get the widget switcher to show the menu page. Eventually should be
	// decoupled further I think
	if (ModBrowserInstance)
	{
		ModBrowserInstance->ShowDetailsForMod(ID);
	}
}

bool UModioUISubsystem::DisplaySearchResults(FModioFilterParams SearchParameters)
{
	if (ModBrowserInstance)
	{
		return ModBrowserInstance->RequestShowSearchResults(SearchParameters);
	}
	return false;
}

void UModioUISubsystem::DisplayNotification(TScriptInterface<IModioUINotification>& Notification)
{
	OnDisplayNotificationWidget.Broadcast(Notification);
}

void UModioUISubsystem::DisplayErrorNotification(const FModioNotificationParams& Params)
{
	OnDisplayErrorNotification.Broadcast(Params);
}

void UModioUISubsystem::LogoDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image, FModioModID ID)
{
	OnModLogoDownloadCompleted.Broadcast(ID, ec, Image);
}

void UModioUISubsystem::UserAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image)
{
	OnUserAvatarDownloadCompleted.Broadcast(ec, Image);
}

void UModioUISubsystem::RequestUserAvatar()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetUserMediaAsync(
			EModioAvatarSize::Thumb100,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::UserAvatarDownloadHandler));
	}
}

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnAuthenticationComplete));
	}
}

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															  this, &UModioUISubsystem::OnAuthenticationComplete),
														  DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestExternalAuthentication(EModioAuthenticationProvider Provider)
{
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
				Subsystem->AuthenticateUserExternalAsync(
					NewParams, Provider,
					FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnAuthenticationComplete));
			}
		}
	}
}

void UModioUISubsystem::RequestExternalAuthentication(EModioAuthenticationProvider Provider,
													  FOnErrorOnlyDelegateFast DedicatedCallback)
{
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
				Subsystem->AuthenticateUserExternalAsync(
					NewParams, Provider,
					FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
																this, &UModioUISubsystem::OnAuthenticationComplete),
															DedicatedCallback](FModioErrorCode ec) {
						DedicatedCallback.ExecuteIfBound(ec);
						HookedHandler.ExecuteIfBound(ec);
					}));
			}
		}
	}
}

void UModioUISubsystem::RequestGalleryImageDownloadForModID(
	FModioModID ID, int32 Index, EModioGallerySize ImageSize /*= EModioGallerySize::Original*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, ImageSize, Index,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::GalleryImageDownloadHandler, ID, Index));
	}
}

void UModioUISubsystem::GalleryImageDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image,
													FModioModID ID, int32 Index)
{
	OnModGalleryImageDownloadCompleted.Broadcast(ID, ec, Index, Image);
}

void UModioUISubsystem::CreatorAvatarDownloadHandler(FModioErrorCode ec, TOptional<FModioImageWrapper> Image,
													 FModioModID ID)
{
	OnModCreatorAvatarDownloadCompleted.Broadcast(ID, ec, Image);
}

void UModioUISubsystem::OnAuthenticationComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TOptional<FModioUser> NewUser = Subsystem->QueryUserProfile();
			OnUserChanged.Broadcast(NewUser);
		}
	}
}

void UModioUISubsystem::ModInfoRequestCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
													   TArray<FModioModID> IDs)
{
	if (ec)
	{
		// Got an error, notify anybody that the specified IDs got errors when requesting mod info
		for (FModioModID& ID : IDs)
		{
			OnModInfoRequestCompleted.Broadcast(ID, ec, {});
		}
	}
	else
	{
		for (FModioModInfo Info : ModInfos.GetValue().GetRawList())
		{
			OnModInfoRequestCompleted.Broadcast(Info.ModId, {}, Info);
		}
	}
}

void UModioUISubsystem::ListAllModsCompletedHandler(FModioErrorCode ec, TOptional<FModioModInfoList> ModInfos,
													FString RequestIdentifier)
{
	OnListAllModsRequestCompleted.Broadcast(RequestIdentifier, ec, ModInfos);
}

int32 UModioUISubsystem::GetActiveTabIndex()
{
	if (ModBrowserInstance)
	{
		return ModBrowserInstance->GetPageIndex();
	}

	return INDEX_NONE;
}

void UModioUISubsystem::SetActiveTabIndex(int TabIndex)
{
	if (ModBrowserInstance)
	{
		ModBrowserInstance->SetPageIndex(TabIndex);
	}
}

void UModioUISubsystem::HandleInputModeChanged(EModioUIInputMode NewDevice)
{
	OnInputDeviceChanged.Broadcast(NewDevice);
	LastInputMode = NewDevice;

	// If we're on a controller, then we want to disable Synthetic Cursor Moves in Slate.
	// This means that any element that the mouse cursor is hovering over won't re-take focus as you scroll through with
	// a keyboard or controller
	if (NewDevice == EModioUIInputMode::Controller || NewDevice == EModioUIInputMode::Keyboard)
	{
		auto* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("Slate.EnableSyntheticCursorMoves"));
		if (CVar)
		{
			CVar->Set(0);
		}
	}
	else if (NewDevice == EModioUIInputMode::Mouse)
	{
		auto* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("Slate.EnableSyntheticCursorMoves"));
		if (CVar)
		{
			CVar->Set(1);
		}
	}
}

void UModioUISubsystem::LogOut(FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ClearUserDataAsync(FOnErrorOnlyDelegateFast::CreateLambda(
			[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnLogoutComplete),
			 DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::OnLogoutComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		OnUserChanged.Broadcast({});

		// If the download drawer is open, we want to close it as part of the logout process
		ModBrowserInstance->ForceCloseDownloadDrawer();
	}
}

void UModioUISubsystem::CloseSearchDrawer()
{
	ModBrowserInstance->ForceCloseSearchDrawer();
}

void UModioUISubsystem::ShowModReportDialog(UObject* DialogDataSource)
{
	ModBrowserInstance->ShowModReportDialog(DialogDataSource);
}

void UModioUISubsystem::ShowReportEmailDialog(UObject* DialogDataSource)
{
	ModBrowserInstance->ShowReportEmailDialog(DialogDataSource);
}

void UModioUISubsystem::ShowUninstallConfirmationDialog(UObject* DialogDataSource)
{
	ModBrowserInstance->ShowUninstallConfirmationDialog(DialogDataSource);
}

TOptional<FModioModTagOptions> UModioUISubsystem::GetTagOptionsList()
{
	if (CachedModTags.IsSet())
	{
		return CachedModTags;
	}
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModTagOptionsAsync(
			FOnGetModTagOptionsDelegateFast::CreateUObject(this, &UModioUISubsystem::OnGetModTagOptionsComplete));
	}

	return TOptional<FModioModTagOptions>();
}

void UModioUISubsystem::OnGetModTagOptionsComplete(FModioErrorCode ec, TOptional<FModioModTagOptions> ModTags)
{
	if (!ec)
	{
		CachedModTags = ModTags;
	}
}

#include "Loc/EndModioLocNamespace.h"