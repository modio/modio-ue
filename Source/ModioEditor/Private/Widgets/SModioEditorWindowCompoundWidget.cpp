/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Widgets/SModioEditorWindowCompoundWidget.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "Misc/EngineVersionComparison.h"
#include "SlateOptMacros.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "DesktopPlatform/Public/DesktopPlatformModule.h"
	#include "DesktopPlatform/Public/IDesktopPlatform.h"
#else
	#include "DesktopPlatformModule.h"
	#include "IDesktopPlatform.h"
#endif
#include "Engine/Engine.h"
#include "HAL/FileManagerGeneric.h"
#include "HttpModule.h"
#include "IStructureDetailsView.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IPluginManager.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Objects/ModioBrowseModFileCollectionObject.h"
#include "Objects/ModioBrowseModFileObject.h"
#include "Objects/ModioBrowseModsObject.h"
#include "Objects/ModioCreateModParamsObject.h"
#include "Objects/ModioCreateNewModFileParamsObject.h"
#include "Objects/ModioEditModParamsObject.h"
#include "SHyperlinkLaunchURL.h"
#include "Styling/SlateBrush.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"
#include "Widgets/Input/SFilePathPicker.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SWindow.h"
#include "WindowManager.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "Launch/Resources/Version.h"
#endif
#include "Kismet/GameplayStatics.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "ModioEditor.h"
#include "ModioEditorSettings.h"
#include "Types/ModioToolWindowEntry.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/SModioEditorUserAuthWidget.h"

#define LOCTEXT_NAMESPACE "LocalizedText"

const FVector2D DefaultImageSize = FVector2D(48.f, 48.f);
const FVector2D GameLogoSize = FVector2D(320.f, 180.f);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorWindowCompoundWidget::Construct(const FArguments& InArgs)
{
	LoadResources();

	// clang-format off
	ChildSlot
	[	
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage).Image(BackgroundBrush)
		]

		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			//.FillHeight(0.4f)
			.AutoHeight()
			[
				SAssignNew(GameInfoVerticalBoxList, SVerticalBox)
			]

			+SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.Padding(32.f)
			.AutoHeight()
			[
				SAssignNew(VerticalBoxList, SVerticalBox)
			]
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		[
			SAssignNew(ProgressBarBox, SBox)
			.Visibility(EVisibility::Collapsed)
			.WidthOverride(300)
			.HeightOverride(300)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Top)
				[
					SAssignNew(ProgressTitle, STextBlock)
					.Font(HeaderLargeTextStyle)
					.Visibility(EVisibility::Collapsed)
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					.MinDesiredWidth(300)
					.MinDesiredHeight(50)
					.MaxDesiredWidth(300)
					.MaxDesiredHeight(50)
					[
						SAssignNew(ProgressBar, SProgressBar)
						.Percent(Percentage)
						.Visibility(EVisibility::Collapsed)
					]
				]
			]
		]
	];
	// clang-format on

	DrawThrobberWidget();
	LoadModioSubsystem();
}

void SModioEditorWindowCompoundWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
											const float InDeltaTime)
{
	if (ModioSubsystem && !ModioSubsystem->IsUsingBackgroundThread())
	{
		ModioSubsystem->RunPendingHandlers();
	}

	if (ProgressBar.IsValid() && (ProgressBar->GetVisibility() == EVisibility::Visible))
	{
		UpdateProgressBar();
	}
}

void SModioEditorWindowCompoundWidget::LoadResources()
{
	Percentage = 0.f;

	ResourcesPath = IPluginManager::Get().FindPlugin("Modio")->GetBaseDir() / TEXT("Source/ModioEditor/Resources/");
	if (FPaths::DirectoryExists(ResourcesPath))
	{
		TArray<FString> Files;
		FFileManagerGeneric::Get().FindFilesRecursive(Files, *ResourcesPath, TEXT("*.png"), true, false, false);
		for (FString File : Files)
		{
			FString Key = FPaths::GetBaseFilename(File);
			FSlateDynamicImageBrush* DynamicImageBrush =
				new FSlateDynamicImageBrush(FName(*File), Key.Contains("Logo_") ? GameLogoSize : DefaultImageSize);
			TexturePool.Add(Key, DynamicImageBrush);
		}
	}

	LoginButtonBrush = new FSlateImageBrush(
		TSoftObjectPtr<UObject>(FSoftObjectPath("/ModioUGC/UI/Materials/M_UI_Cog_ModioUGC.M_UI_Cog_ModioUGC"))
			.LoadSynchronous(),
		FVector2D(32, 32));

	HeaderBackgroundBrush = new FSlateBrush();
	HeaderBackgroundBrush->TintColor = FLinearColor(0.025f, 0.025f, 0.025f, 1.f);
	HeaderBackgroundBrush->SetImageSize(FVector2D(64.f, 64.f));

	PanelBackgroundBrush = new FSlateBrush();
	PanelBackgroundBrush->TintColor = FLinearColor(0.015f, 0.015f, 0.015f, 1.f);

	BoldSeperatorBrush = new FSlateBrush();
	BoldSeperatorBrush->TintColor = FLinearColor(0.05, 0.05, 0.05, 1.0f);

	BackgroundBrush = new FSlateBrush();
#if ENGINE_MAJOR_VERSION >= 5
	BackgroundBrush->TintColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.f);
#else
	BackgroundBrush->TintColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.f);
#endif

	HeaderLargeTextStyle = GetTextStyle("EmbossedText", "Normal", 14);
	HeaderSmallTextStyle = GetTextStyle("EmbossedText", "Normal", 11);

	ButtonTextStyle = GetTextStyle("EmbossedText", "Normal", 10);

	FButtonStyle NoBackButtonStyle;
}

void SModioEditorWindowCompoundWidget::LoadModioSubsystem()
{
	FModioInitializeOptions InitializeOptions =
		UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(FString("ModioUnrealEditor"));

	if (GEngine)
	{
		ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (ModioSubsystem)
		{
			ModioSubsystem->InitializeAsync(
				InitializeOptions,
				FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnInitCallback));
		}
	}
}

void SModioEditorWindowCompoundWidget::EnableModManagement()
{
	ModioSubsystem->EnableModManagement(
		FOnModManagementDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnModManagementCallback));
}

void SModioEditorWindowCompoundWidget::OnModManagementCallback(FModioModManagementEvent Event)
{
	UE_LOG(ModioEditor, Warning, TEXT("ModEvent: %d, ID: %s, Msg: %s"), (int) Event.Event, *Event.ID.ToString(),
		   *Event.Status.GetErrorMessage());

	if (Event.ID == UploadModID)
	{
		if (Event.Status.GetValue() != 0)
		{
			HideProgressBar();

			FText Message =
				Localize("ModFileNotUploaded",
						 FString::Printf(TEXT("Your mod file could not be uploaded, see error message "
											  "below:\n\n%s\n\nErrorCode: %d, EventType: %d."),
										 *Event.Status.GetErrorMessage(), Event.Status.GetValue(), (int) Event.Event));
			FMessageDialog::Open(EAppMsgType::Ok, Message);
			WindowManager::Get().GetWindow()->BringToFront();

			ModioSubsystem->DisableModManagement();
			EnableModManagement();
			return;
		}

		if (Event.Event == EModioModManagementEventType::Uploaded)
		{
			HideProgressBar();

			FText Message = Localize("ModFileUploaded", "Your mod file has uploaded successfully.");
			FMessageDialog::Open(EAppMsgType::Ok, Message);
			WindowManager::Get().GetWindow()->BringToFront();
		}
	}
}

void SModioEditorWindowCompoundWidget::OnInitCallback(FModioErrorCode ErrorCode)
{
	if (!ErrorCode ||
		UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::SDKAlreadyInitialized))
	{
		EnableModManagement();

		ModioSubsystem->GetGameInfoAsync(
			UModioSDKLibrary::GetProjectGameId(),
			FOnGetGameInfoDelegateFast::CreateLambda(
				[this](FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo) {
					if (ErrorCode)
					{
						FText Message =
							Localize("ModioGameInfoError",
									 FString::Printf(TEXT("Could not retrieve game information from mod.io\n%s"),
													 *ErrorCode.GetErrorMessage()));
						FMessageDialog::Open(EAppMsgType::Ok, Message);
						WindowManager::Get().GetWindow()->BringToFront();
						WindowManager::Get().CloseWindow();
						return;
					}
					if (GameInfo.IsSet())
					{
						ModioGameInfo = GameInfo;

						FString HeaderPath =
							ResourcesPath + "Downloaded/" + "Header_" + GameInfo->GameID.ToString() + ".png";

						auto OnHeaderObtained = FSimpleDelegate::CreateSPLambda(this, [this, HeaderPath]() {
							DrawLogoWidget();
							FString Key = FPaths::GetBaseFilename(HeaderPath);
							FSlateDynamicImageBrush* LogoBrush = TexturePool[Key];
							LogoBrush->DrawAs = ESlateBrushDrawType::Box;
							LogoBrush->Margin = FMargin(0.f);
							ModioGameLogo->SetImage(LogoBrush);
							ModioSubsystem->VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast::CreateRaw(
								this, &SModioEditorWindowCompoundWidget::OnVerifyCurrentUserAuthenticationCompleted));
						});

						if (!FPaths::FileExists(HeaderPath))
						{
							DownloadGameImage(GameInfo->HeaderImage.Original, HeaderPath, OnHeaderObtained);
						}
						else
						{
							OnHeaderObtained.ExecuteIfBound();
						}
					}
				}));
	}
	else
	{
		UE_LOG(ModioEditor, Error,
			   TEXT("ModioSubsystem - OnInitCallback - Could not initialize ModioSubsystem - ErrorCode: %d | "
					"ErrorMessage: %s"),
			   ErrorCode.GetValue(), *ErrorCode.GetErrorMessage());
		WindowManager::Get().CloseWindow();
	}
}

void SModioEditorWindowCompoundWidget::OnVerifyCurrentUserAuthenticationCompleted(FModioErrorCode ErrorCode)
{
	DrawToolLanding();
}

void SModioEditorWindowCompoundWidget::DrawToolLanding()
{
	TOptional<FModioUser> CurrentUser = ModioSubsystem->QueryUserProfile();

	ClearAllWidgets();

	TSharedPtr<SVerticalBox> VBoxContent;

	// clang-format off

	VerticalBoxList->AddSlot()
	[
		SNew(SBox)
		.MinDesiredWidth(900)
		.MinDesiredHeight(500)
		[
			SAssignNew(VBoxContent, SVerticalBox)
		]
	];
	
	VBoxContent->AddSlot()
		.VAlign(VAlign_Top)
		.FillHeight(0.15)
		.Padding(FMargin(0, 16))
		[
			// landing header
			SNew(STextBlock)
				.Text(LOCTEXT(
					"ModioLandingHeaderFmt",
					"Please choose where you would like to begin:"))
				.AutoWrapText(true)
				.Justification(ETextJustify::Center)
		];

	VBoxContent->AddSlot()
		.Padding(0, 24, 0, 0)
		[
			SAssignNew(LandingCategoryList, SVerticalBox)
		];

	if(!CurrentUser)
	{
		VBoxContent->AddSlot()
		[
			SNew(SBox)
				.WidthOverride(350)
				.HeightOverride(60)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.ContentPadding(FMargin(100, 12))
					.OnClicked(this, &SModioEditorWindowCompoundWidget::OnLoginLandingButtonClicked)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(8, 0)
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SNew(SBox)
							.WidthOverride(32)
							.HeightOverride(32)
							[
								SNew(SImage)
								.Image(LoginButtonBrush)
								.DesiredSizeOverride(FVector2D(32, 32))
							]
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.Padding(8, 0)
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ModioLandingLogin", "Login to mod.io"))
							.AutoWrapText(true)
							.Justification(ETextJustify::Left)
							.Font(ButtonTextStyle)
						]
					]
				]
		];

		VBoxContent->AddSlot()
			.VAlign(VAlign_Center)
			.FillHeight(0.15)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Text(Localize("ModioLandingAccount",
								FString::Printf(TEXT("A mod.io account is required to upload and manage mods to %s."), *ModioGameInfo->Name)))
					.AutoWrapText(true)
					.Justification(ETextJustify::Center)
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				[
					SNew(SHyperlink)
					.Text(LOCTEXT("ModioTermsAndConditions", "Click here to create a new account"))
					.OnNavigate_Lambda([this]() {
						FPlatformProcess::LaunchURL(TEXT("http://mod.io"), nullptr, nullptr);
					})
				]
			];
	}
	else
	{
		VBoxContent->AddSlot()
			.VAlign(VAlign_Bottom)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(4, 0)
				[
					SNew(STextBlock)
					.Text(FText::Format(LOCTEXT("ModioLandingLoggedInAs",
								"Logged in as {0}."), FText::FromString(CurrentUser->Username)))
					.AutoWrapText(true)
					.Justification(ETextJustify::Center)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(4, 0)
				[
					SNew(SButton)
					.ContentPadding(6)
					.OnClicked_Lambda([this]()
					{
						Logout(false);
						DrawToolLanding();
						return FReply::Handled();
					})
					.HAlign(HAlign_Center)
					.Text(LOCTEXT("LogoutButton", "Log Out"))
				]
			];
	}

	// clang-format on

	// Populate LandingCategoryList with entries
	UModioEditorSettings* Settings = GetMutableDefault<UModioEditorSettings>();
	for (auto Entry : Settings->ToolLandingEntries)
	{
		if (Entry.bRequiresAuth && (!ModioSubsystem || !ModioSubsystem->QueryUserProfile()))
			continue;

		TSharedPtr<SWidget> EntryWidget = CreateToolEntryWidget(Entry);
		if (EntryWidget.IsValid())
		{
			LandingCategoryList->AddSlot().AttachWidget(EntryWidget.ToSharedRef());
		}
	}

	for (auto Entry : Settings->SubmoduleToolLandingEntries)
	{
		if (Entry.bRequiresAuth && (!ModioSubsystem || !ModioSubsystem->QueryUserProfile()))
			continue;

		TSharedPtr<SWidget> EntryWidget = CreateToolEntryWidget(Entry);
		if (EntryWidget.IsValid())
		{
			LandingCategoryList->AddSlot().AttachWidget(EntryWidget.ToSharedRef());
		}
	}
}

void SModioEditorWindowCompoundWidget::DrawSubwindowWidget(TSharedPtr<SWidget> Widget)
{
	if (!Widget.IsValid())
	{
		return;
	}

	ClearAllWidgets();

	VerticalBoxList->AddSlot().AutoHeight().AttachWidget(Widget.ToSharedRef());
}

FReply SModioEditorWindowCompoundWidget::OnLoginLandingButtonClicked()
{
	DrawLoginWidget();
	return FReply::Handled();
}

void SModioEditorWindowCompoundWidget::DrawLoginWidget()
{
	ClearAllWidgets();

	// clang-format offld

	VerticalBoxList->AddSlot().AutoHeight()[SNew(SModioEditorUserAuthWidget)
												.ParentWindow(this)
												.BackHandler(this, &SModioEditorWindowCompoundWidget::DrawToolLanding)];
	// clang-format on
}

void SModioEditorWindowCompoundWidget::ClearAllWidgets()
{
	if (VerticalBoxList.IsValid())
	{
		if (VerticalBoxList->GetAllChildren()->Num() > 0)
		{
			VerticalBoxList->ClearChildren();
		}
	}
}

void SModioEditorWindowCompoundWidget::DrawLogoWidget()
{
	// clang-format off
	GameInfoVerticalBoxList->AddSlot()
	.Padding(FMargin(0.f, 0.f, 0.f, 0.f))
	.AutoHeight()
	[
		SNew(SOverlay)

		// Background Image
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(HeaderBackgroundBrush)
		]
		
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ModioGameLogoContainer, SScaleBox) 
			.Stretch(EStretch::ScaleToFit)
			.HAlign(HAlign_Left)
			[
				SAssignNew(ModioGameLogo, SImage)
			]
		]

		+SOverlay::Slot()
		.Padding(FMargin(15.f + GameLogoSize.X + 15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		[
			SNew(SHorizontalBox)
			
			// Instruction TextBlock
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			//.AutoWidth()
			[
				SNew(SVerticalBox)

				+SVerticalBox::Slot()
				.Padding(FMargin(0.f, 0.f, 0.f, 15.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SAssignNew(GameName, STextBlock)
						.MinDesiredWidth(512.f)
						.AutoWrapText(true)
						.Font(HeaderLargeTextStyle)
						.Text(FText::GetEmpty())
						.Justification(ETextJustify::Left)
					]
				]

				+SVerticalBox::Slot()
				.Padding(FMargin(0.f, 0.f, 0.f, 15.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					[
						SAssignNew(UGCName, STextBlock)
						.MinDesiredWidth(512.f)
						.AutoWrapText(true)
						.Font(HeaderSmallTextStyle)
						.Text(FText::GetEmpty())
						.Justification(ETextJustify::Left)
					]
				]

				+SVerticalBox::Slot()
				.Padding(FMargin(0.f, 0.f, 0.f, 15.f))
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Bottom)
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					[
						SAssignNew(Description, STextBlock)
						.MinDesiredWidth(512.f)
						.AutoWrapText(true)
						.Font(HeaderSmallTextStyle)
						.Text(FText::GetEmpty())
						.Justification(ETextJustify::Left)
					]
				]
			]
		]
	];

	// clang-format on

	if (ModioGameInfo.IsSet())
	{
		GameName->SetText(Localize("GameName", ModioGameInfo->Name));
		UGCName->SetText(Localize("UGCName", ModioGameInfo->UgcName + " Management Tool"));
		Description->SetText(Localize("ModioEditorDesc", ModioGameInfo->Summary));
	}
}

void SModioEditorWindowCompoundWidget::DrawThrobberWidget()
{
	ClearAllWidgets();

	// clang-format on
	VerticalBoxList->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()[SNew(SCircularThrobber).Radius(50.f)];
	// clang-format off
}

void SModioEditorWindowCompoundWidget::ShowProgressBar()
{
	WindowManager::Get().GetWindow()->EnableWindow(false);

	VerticalBoxList->SetVisibility(EVisibility::Collapsed);
	ProgressBarBox->SetVisibility(EVisibility::Visible);

	ProgressBar->SetVisibility(EVisibility::Visible);
	ProgressTitle->SetVisibility(EVisibility::Visible);
}

void SModioEditorWindowCompoundWidget::UpdateProgressBar()
{
	TOptional<FModioModProgressInfo> ProgressInfo = ModioSubsystem->QueryCurrentModUpdate();
	if (ProgressInfo.IsSet())
	{
		if (ProgressInfo->GetCurrentState() == EModioModProgressState::Compressing)
		{
			FModioUnsigned64 CurrentProgress = ProgressInfo->GetCurrentProgress(EModioModProgressState::Compressing);
			FModioUnsigned64 TotalProgress = ProgressInfo->GetTotalProgress(EModioModProgressState::Compressing);
			Percentage = (float) CurrentProgress / (float) TotalProgress;
			ProgressTitle->SetText(Localize("CompressingModfile", "Compressing"));
			ProgressBar->SetPercent(Percentage);
		}
		else if (ProgressInfo->GetCurrentState() == EModioModProgressState::Uploading)
		{
			FModioUnsigned64 CurrentProgress = ProgressInfo->GetCurrentProgress(EModioModProgressState::Uploading);
			FModioUnsigned64 TotalProgress = ProgressInfo->GetTotalProgress(EModioModProgressState::Uploading);
			Percentage = (float) CurrentProgress / (float) TotalProgress;
			ProgressTitle->SetText(Localize("UploadingModfile", "Uploading"));
			ProgressBar->SetPercent(Percentage);
		}
		else
		{
			if(ProgressBar->GetVisibility() == EVisibility::Visible)
			{
				HideProgressBar();
			}
		}
	}
}

void SModioEditorWindowCompoundWidget::HideProgressBar()
{
	WindowManager::Get().GetWindow()->EnableWindow(true);
	
	VerticalBoxList->SetVisibility(EVisibility::Visible);
	ProgressBarBox->SetVisibility(EVisibility::Collapsed);


	ProgressBar->SetVisibility(EVisibility::Hidden);
	ProgressTitle->SetVisibility(EVisibility::Hidden);
}

void SModioEditorWindowCompoundWidget::Logout(bool Exit)
{
	if (ModioSubsystem)
	{
		ModioSubsystem->ClearUserDataAsync(
			FOnErrorOnlyDelegateFast::CreateLambda([this, Exit](FModioErrorCode ErrorCode) {
				if (ErrorCode == 0)
				{
					WindowManager::Get().GetWindow()->BringToFront();
				}
			}));
	}
}

void SModioEditorWindowCompoundWidget::UnloadResources()
{
	HeaderBackgroundBrush = nullptr;
	BackgroundBrush = nullptr;
	SubmitThrobber = nullptr;
	ProgressBar = nullptr;
	PercentageText = nullptr;
	GameInfoVerticalBoxList = nullptr;
	VerticalBoxList = nullptr;
	TexturePool.Empty();
}

FText SModioEditorWindowCompoundWidget::Localize(FString Key, FString Text)
{
	return FText::Format(LOCTEXT("{0}", "{1}"), FText::FromString(Key), FText::FromString(Text));
}

FSlateFontInfo SModioEditorWindowCompoundWidget::GetTextStyle(FName PropertyName, FName FaceName, int32 Size)
{
	FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle(PropertyName);
	FontInfo.Size = Size;
	FontInfo.TypefaceFontName = FaceName;
	return FontInfo;
}

void SModioEditorWindowCompoundWidget::DownloadGameImage(FString URL, FString SaveAs, FSimpleDelegate OnComplete)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda(
		[this, URL, SaveAs, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
			const TArray<uint8> bytes = Response->GetContent();
			const FString content = Response->GetContentAsString();

			if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(ModioEditor, Error, TEXT("DownloadGameImage - Request Failed -_url: %s | content: %s"), *URL,
					   *content);
				return;
			}

			FString ImagePath = SaveAs;
			FFileHelper::SaveArrayToFile(bytes, *SaveAs);

			FString Key = FPaths::GetBaseFilename(SaveAs);
			FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(FName(*SaveAs), GameLogoSize);
			TexturePool.Add(Key, DynamicImageBrush);

			OnComplete.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

FString SModioEditorWindowCompoundWidget::ToNonPlural(const FString& String)
{
	if (String.EndsWith("s", ESearchCase::IgnoreCase))
	{
		FString NonPlural = FString(String);
		NonPlural.RemoveFromEnd("s", ESearchCase::IgnoreCase);
		return NonPlural;
	}
	return String;
}

EModioModfilePlatform SModioEditorWindowCompoundWidget::ToPlatformEnum(FString PlatformString)
{
	static const UEnum* PlatformEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModfilePlatform"));
	int32 Index = PlatformEnum->GetIndexByNameString(PlatformString);
	return EModioModfilePlatform((uint8) Index);
}

FString SModioEditorWindowCompoundWidget::ToPlatformString(EModioModfilePlatform Platform)
{
	static const UEnum* PlatformEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModfilePlatform"));
	return PlatformEnum->GetNameStringByIndex(static_cast<uint32>(Platform));
}

TSharedPtr<SWidget> SModioEditorWindowCompoundWidget::CreateToolEntryWidget(FModioToolWindowEntry Entry)
{
	if (!Entry.CreateWindowAction.IsBound() || !Entry.bEnabled)
	{
		UE_LOG(ModioEditor, Error, TEXT("Failed to create entry widget for modio tools window: %s"),
			   *Entry.Name.ToString())
		return nullptr;
	}

	TextureAssetPool.Add(Entry.Icon, FSlateImageBrush(Entry.Icon.LoadSynchronous(), FVector2D(45, 45)));

	FOnClicked OnClicked = FOnClicked::CreateLambda([this, CreateWidget = Entry.CreateWindowAction]() {
		DrawSubwindowWidget(CreateWidget.Execute(this));
		return FReply::Handled();
	});

	static FButtonStyle MyHoverBorderStyle;
	static bool bStyleInitialized = false;

	if (!bStyleInitialized)
	{
		const FName BorderBrushName = "SButton.Border";
    
		FSlateBorderBrush HoveredAndPressedBrush(
			BorderBrushName,
			FMargin(1.0f),
			FLinearColor(1.0f, 1.0f, 1.0f, 0.7f)
		);

		MyHoverBorderStyle = FButtonStyle()
			.SetNormal(FSlateNoResource())
			.SetHovered(HoveredAndPressedBrush)
			.SetPressed(HoveredAndPressedBrush)
			.SetNormalPadding(FMargin(1))
			.SetPressedPadding(FMargin(1));

		bStyleInitialized = true;
	}

	// Each game has it's own name for UGC, so we replace the placeholder in the entry name and description
	FString EntryName = Entry.Name.ToString().Replace(TEXT("[UGC_NAME]"), *ModioGameInfo->UgcName);
	FString EntryDescription = Entry.Description.ToString().Replace(TEXT("[UGC_NAME]"), *ModioGameInfo->UgcName);

	// clang-format off
	return 
		SNew(SBox)
		[
			SNew(SButton)
			.OnClicked(OnClicked)
			.ContentPadding(12)
			.ButtonStyle(&MyHoverBorderStyle)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.AutoWidth()
				[
					SNew(SScaleBox)
					.Stretch(EStretch::None)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(SImage)
						.Image(&TextureAssetPool[Entry.Icon])
					]
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.2f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(12, 0)
				[
					SNew(STextBlock)
					.Text(FText::FromString(EntryName))
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.FillWidth(0.8f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(EntryDescription))
					.AutoWrapText(true)
					.MinDesiredWidth(256.f)
				]
			]
		];
	// clang-format on
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE