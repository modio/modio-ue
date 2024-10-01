/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Widgets/SModioEditorWindowCompoundWidget.h"
#include "SlateOptMacros.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "DesktopPlatform/Public/IDesktopPlatform.h"
	#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#else
	#include "DesktopPlatformModule.h"
	#include "IDesktopPlatform.h"
#endif
#include "HttpModule.h"
#include "HAL/FileManagerGeneric.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IPluginManager.h"
#include "IStructureDetailsView.h"
#include "Libraries/ModioSDKLibrary.h"
#include "Misc/FileHelper.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "Objects/ModioBrowseModFileCollectionObject.h"
#include "Objects/ModioBrowseModFileObject.h"
#include "Objects/ModioBrowseModsObject.h"
#include "Objects/ModioCreateModParamsObject.h"
#include "Objects/ModioCreateNewModFileParamsObject.h"
#include "Objects/ModioEditModParamsObject.h"
#include "Styling/SlateBrush.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModInfo.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SHeader.h"
#include "WindowManager.h"
#include "Widgets/SOverlay.h"
#include "Engine/Engine.h"
#include "Misc/MessageDialog.h"
#include "Widgets/SWindow.h"
#if UE_VERSION_OLDER_THAN(5, 3, 0)
	#include "Launch/Resources/Version.h"
#endif
#include "ModioEditor.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/ModioErrorConditionLibrary.h"


#define LOCTEXT_NAMESPACE "LocalizedText"

const FVector2D DefaultImageSize = FVector2D(48.f, 48.f);
const FVector2D GameLogoSize = FVector2D(320.f, 180.f);

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SModioEditorWindowCompoundWidget::Construct(const FArguments& InArgs)
{
	LoadResources();
	
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
			SAssignNew(GameInfoVerticalBoxList, SVerticalBox)
		]

		+SOverlay::Slot()
		.Padding(FMargin(15.f, 215.f, 15.f, 15.f))
		[
			SAssignNew(VerticalBoxList, SVerticalBox)
		]
	];
	
	DrawThrobberWidget();
	LoadModioSubsystem();
}

void SModioEditorWindowCompoundWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
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
			FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(FName(*File), Key.Contains("Logo_") ? GameLogoSize : DefaultImageSize);
			TexturePool.Add(Key, DynamicImageBrush);
		}
	}

	HeaderBackgroundBrush = new FSlateBrush();
	HeaderBackgroundBrush->TintColor = FLinearColor(0.025f, 0.025f, 0.025f, 1.f);
	HeaderBackgroundBrush->SetImageSize(FVector2D(64.f, 64.f));

	BackgroundBrush = new FSlateBrush();
#if ENGINE_MAJOR_VERSION >= 5
	BackgroundBrush->TintColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.f);
#else
	BackgroundBrush->TintColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.f);
#endif

	HeaderLargeTextStyle = GetTextStyle("EmbossedText", "Normal", 14);
	HeaderSmallTextStyle = GetTextStyle("EmbossedText", "Normal", 11);
	ButtonTextStyle = GetTextStyle("EmbossedText", "Normal", 10);
}

void SModioEditorWindowCompoundWidget::LoadModioSubsystem()
{
	FModioInitializeOptions InitializeOptions = UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(FString("ModioUnrealEditor"));
	
	if (GEngine)
	{
		ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (ModioSubsystem)
		{
			ModioSubsystem->InitializeAsync(InitializeOptions, FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnInitCallback));
		}
	}
}

void SModioEditorWindowCompoundWidget::EnableModManagement()
{
	ModioSubsystem->EnableModManagement(FOnModManagementDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnModManagementCallback));
}

void SModioEditorWindowCompoundWidget::OnModManagementCallback(FModioModManagementEvent Event) 
{
	UE_LOG(ModioEditor, Warning, TEXT("ModEvent: %d, ID: %s, Msg: %s"), (int)Event.Event, *Event.ID.ToString(), *Event.Status.GetErrorMessage());

	if (Event.ID == UploadModID)
	{
		if (Event.Status.GetValue() != 0)
		{
			if (Event.Event == EModioModManagementEventType::BeginUpload || Event.Event == EModioModManagementEventType::Uploaded)
			{
				HideProgressBar();

				FText Message = Localize("ModFileNotUploaded", FString::Printf(TEXT("Your mod file could not be uploaded, see error message below:\n\n%s\n\nErrorCode: %d, EventType: %d."), *Event.Status.GetErrorMessage(), Event.Status.GetValue(), (int)Event.Event));
				FMessageDialog::Open(EAppMsgType::Ok, Message);
				WindowManager::Get().GetWindow()->BringToFront();
				
				ModioSubsystem->DisableModManagement();
				EnableModManagement();
			}
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
	if (!ErrorCode || UModioErrorConditionLibrary::ErrorCodeMatches(ErrorCode, EModioErrorCondition::SDKAlreadyInitialized))
	{
		EnableModManagement();

		ModioSubsystem->GetGameInfoAsync(UModioSDKLibrary::GetProjectGameId(), FOnGetGameInfoDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo) 
		{
			if (ErrorCode)
			{
				FText Message = Localize("ModioGameInfoError", FString::Printf(TEXT("Could not retrieve game information from mod.io\n%s"), *ErrorCode.GetErrorMessage()));
				FMessageDialog::Open(EAppMsgType::Ok, Message);
				WindowManager::Get().GetWindow()->BringToFront();
				WindowManager::Get().CloseWindow();
				return;
			}
			if (GameInfo.IsSet())
			{
				ModioGameInfo = GameInfo;
				
				FString LogoPath = ResourcesPath + "Downloaded/" + "Logo_" + GameInfo->GameID.ToString() + ".png";
				if (!FPaths::FileExists(LogoPath))
				{
					DownloadGameLogo(GameInfo->Logo.Thumb320x180);
				}
				else
				{
					DrawLogoWidget();

					FString Key = FPaths::GetBaseFilename(LogoPath);
					ModioGameLogo->SetImage(TexturePool[Key]);

					ModioSubsystem->VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnVerifyCurrentUserAuthenticationCompleted));
				}
			}
		}));
	}
	else
	{
		UE_LOG(ModioEditor, Error, TEXT("ModioSubsystem - OnInitCallback - Could not initialize ModioSubsystem - ErrorCode: %d | ErrorMessage: %s"), ErrorCode.GetValue(), *ErrorCode.GetErrorMessage());
		WindowManager::Get().CloseWindow();
	}
}

void SModioEditorWindowCompoundWidget::OnVerifyCurrentUserAuthenticationCompleted(FModioErrorCode ErrorCode)
{
	if (!ErrorCode)
	{
		TOptional<FModioUser> CurrentUser = ModioSubsystem->QueryUserProfile();
		if (CurrentUser.IsSet())
		{
			FString FileName = FPaths::ProjectSavedDir() + "ModioEditorExtension.txt";
			if (FPaths::FileExists(FileName))
			{
				DrawModCreationToolWidget();
				return;
			}

			const FText Title = Localize("UserInfo", "User Information");
			const FText Message = Localize("UserInfoMessage", FString::Printf(TEXT("Modio user '%s' already logged in on this platform"), *CurrentUser->Username));
#if UE_VERSION_OLDER_THAN(5, 3, 0)
			EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, &Title);
			#else
			EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message, Title);
			#endif
			if (UserSelection == EAppReturnType::Ok)
			{
				DrawModCreationToolWidget();
				WindowManager::Get().GetWindow()->BringToFront();
				FFileHelper::SaveStringToFile(CurrentUser->Username, *FileName);
			}
		}
	}
	else
	{
		DrawLoginWidget();
	}
}

void SModioEditorWindowCompoundWidget::DrawLoginWidget()
{
	ClearAllWidgets();

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SNew(SHorizontalBox)

			// Modio Email Label
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Localize("ModioEmail", "Log in with Email:"))
			]

			// Modio Email EditableTextBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(ModioEmailEditableTextBox, SEditableTextBox)
				.MinDesiredWidth(256.f)
			]

			// Login Button
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(1.f, 1.f))
						
				.OnClicked(this, &SModioEditorWindowCompoundWidget::OnLoginButtonClicked)
				[
					// Login Button Text
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(Localize("Login", "Login"))
					.Justification(ETextJustify::Center)
				]
			]	
		]
	];
}

FReply SModioEditorWindowCompoundWidget::OnLoginButtonClicked()
{
	if (ModioEmailEditableTextBox->GetText().ToString().IsEmpty())
	{
		FText Message = Localize("ModioLoginInvalid", "Unable to login, please enter a valid email address for mod.io!"); 
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		WindowManager::Get().GetWindow()->BringToFront();
		return FReply::Handled();
	}

	DrawThrobberWidget();
	ModioSubsystem->RequestEmailAuthCodeAsync(FModioEmailAddress(ModioEmailEditableTextBox->GetText().ToString()), FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnRequestEmailAuthCodeCompleted));
	return FReply::Handled();
}

void SModioEditorWindowCompoundWidget::OnRequestEmailAuthCodeCompleted(FModioErrorCode ErrorCode)
{
	if (ErrorCode.GetValue() == 0)
	{
		DrawAuthenticateWidget();
	}
	else if (ErrorCode.GetValue() == 20993)
	{
		DrawModCreationToolWidget();

		FText Message = Localize("ModioEmailAlreadyAuthenticated", FString::Printf(TEXT("%s!"), *ErrorCode.GetErrorMessage()));
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		WindowManager::Get().GetWindow()->BringToFront();
	}
	else
	{
		DrawLoginWidget();

		const FString LastValidationError = [this]() -> FString
		{
			if (ModioSubsystem && ModioSubsystem->GetLastValidationError().Num() > 0)
			{
				return FString::Printf(TEXT("\n\"%s\": \"%s\""), *ModioSubsystem->GetLastValidationError()[0].FieldName, *ModioSubsystem->GetLastValidationError()[0].ValidationFailureDescription);
			}
			return FString();
		}();
		if (ModioSubsystem)
		{
			FText Message = Localize("ModioAuthFailed", FString::Printf(TEXT("Authentication failed, see message for detail\n%s%s"), *ErrorCode.GetErrorMessage(), *LastValidationError));
			FMessageDialog::Open(EAppMsgType::Ok, Message);
			WindowManager::Get().GetWindow()->BringToFront();
		}
	}
}

void SModioEditorWindowCompoundWidget::DrawAuthenticateWidget()
{
	ClearAllWidgets();

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.AutoHeight()
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SNew(SHorizontalBox)

			// Modio Authentication  Label
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Localize("AuthCode", "Authentication Code:"))
			]

			// Modio Authentication EditableTextBox
			+ SHorizontalBox::Slot()
			.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(ModioAuthenticationCodeEditableTextBox, SEditableTextBox)
				.MinDesiredWidth(256.f)
			]

			// Authenticate Button
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.DesiredSizeScale(FVector2D(1.f, 1.f))
				.OnClicked(this, &SModioEditorWindowCompoundWidget::OnAuthenticateButtonClicked)
				[
					// Authenticate Button Text
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(Localize("ModioAuth", "Authenticate"))
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
}

FReply SModioEditorWindowCompoundWidget::OnAuthenticateButtonClicked()
{
	DrawThrobberWidget();

	ModioSubsystem->AuthenticateUserEmailAsync(FModioEmailAuthCode(ModioAuthenticationCodeEditableTextBox->GetText().ToString()), FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnAuthCodeCompleted));
	return FReply::Handled();
}

void SModioEditorWindowCompoundWidget::OnAuthCodeCompleted(FModioErrorCode ErrorCode)
{
	if (ErrorCode == 0)
	{
		DrawModCreationToolWidget();
	}
	else
	{
		DrawLoginWidget();
	}
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
	GameInfoVerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 60.f))
	.MaxHeight(175.f)
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
		.Padding(FMargin(15.f))
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)

			// Modio Icon
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.AutoWidth()
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

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	.AutoHeight()
	[
		SNew(SCircularThrobber)
		.Radius(50.f)	
	];
}

void SModioEditorWindowCompoundWidget::DrawModCreationToolWidget()
{
	ClearAllWidgets();

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f))
	.AutoHeight()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[	
					SNew(SImage)
					.Image(TexturePool["icon_CreateMod"])
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(15.f, 0.f, 100.f, 0.f)
				.MaxWidth(100.f)
				[
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.Font(ButtonTextStyle)
					.Text(Localize("CreateMod", "Create " + ToNonPlural(ModioGameInfo->UgcName)))
					.Justification(ETextJustify::Center)
				]

				+ SHorizontalBox::Slot()
				.Padding(0.f, 0.f, 0.f, 0.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.MaxWidth(700.f)
				[
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.MinDesiredWidth(700.f)
					.AutoWrapText(true)
					.Font(ButtonTextStyle)
					.Text(Localize("CreateModDesc", "Create a new " + ToNonPlural(ModioGameInfo->UgcName) + " for " + ModioGameInfo->Name))
					.Justification(ETextJustify::Left)
				]
			]
			.OnClicked_Lambda([this]() 
			{
				 ClearAllWidgets();

				 UModioCreateModParamsObject* ModProperties = NewObject<UModioCreateModParamsObject>();
				 FDetailsViewArgs DetailsViewArgs;
				 DetailsViewArgs.bHideSelectionTip = true;
				 DetailsViewArgs.bAllowSearch = false;
				 DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
				 TSharedPtr<IDetailsView> CreateModDetailsView = WindowManager::Get().GetPropertyModule().CreateDetailView(DetailsViewArgs);

				 VerticalBoxList->AddSlot()
				 [
					CreateModDetailsView.ToSharedRef()
				 ];

				 VerticalBoxList->AddSlot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				 [
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					 .Padding(5.0f, 0.f, 5.0f, 25.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.MaxWidth(128.f)
					[
						SNew(SButton)
						.DesiredSizeScale(FVector2D(2.f, 1.f))
						.OnClicked_Lambda([this, ModProperties]()
						{				
							if (ModioSubsystem)
							{
								ModioSubsystem->GetGameInfoAsync(UModioSDKLibrary::GetProjectGameId(), FOnGetGameInfoDelegateFast::CreateLambda([this, ModProperties](FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo) 
								{
									if (GameInfo.IsSet())
									{
										FModioModCreationHandle Handle = ModioSubsystem->GetModCreationHandle();

										FModioCreateModParams Params;
										Params.Name = ModProperties->Name;
										Params.Description = ModProperties->Summary;
										Params.PathToLogoFile = ModProperties->PathToLogoFile;
										Params.Summary = ModProperties->Summary;

										SubmitThrobber->SetVisibility(EVisibility::Visible);
								
										ModioSubsystem->SubmitNewModAsync(Handle, Params, FOnSubmitNewModDelegateFast::CreateLambda( [this, Params](FModioErrorCode ErrorCode, TOptional<FModioModID> ModId) 
										{
											if (!ErrorCode)
											{
												UE_LOG(ModioEditor, Warning, TEXT("Mod (%s) Submitted Successfully"), *ModId->ToString());

												const FText Title = Localize("ModCreated", "Mod Created");
												const FText Message = Localize("ModCreatedMessage", "Would you like to add a mod file?");
#if UE_VERSION_OLDER_THAN(5, 3, 0)
												EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::YesNo, Message, &Title);
#else
												EAppReturnType::Type UserSelection =
													FMessageDialog::Open(EAppMsgType::YesNo, Message, Title);
#endif
												if (UserSelection == EAppReturnType::Yes) 
												{
													DrawCreateModFileToolWidget(ModId.GetValue());
												}
												else if (UserSelection == EAppReturnType::No)
												{
													DrawModCreationToolWidget();
												}
											}
											else
											{
												const FString LastValidationError = [this]() -> FString
												{
													if (ModioSubsystem && ModioSubsystem->GetLastValidationError().Num() > 0)
													{
														return FString::Printf(TEXT("\n\"%s\": \"%s\""), *ModioSubsystem->GetLastValidationError()[0].FieldName, *ModioSubsystem->GetLastValidationError()[0].ValidationFailureDescription);
													}
													return FString();
												}();
												FText Message = Localize("ModCreationFailed", FString::Printf(TEXT("Mod creation failed, please see the message below and try again.\nError Message: %s%s"), *ErrorCode.GetErrorMessage(), *LastValidationError));
												EAppReturnType::Type UserSelection = FMessageDialog::Open(EAppMsgType::Ok, Message);
											}
											WindowManager::Get().GetWindow()->BringToFront(true);
											SubmitThrobber->SetVisibility(EVisibility::Hidden);
										}));
									}
								}));
								
							}

							return FReply::Handled();
						})
						[
							SNew(STextBlock)
							.Text(Localize("ModSubmit", "Submit"))
							.Justification(ETextJustify::Center)
						]
					]

					+ SHorizontalBox::Slot()
					.Padding(5.0f, 0.f, 25.0f, 25.f)
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SAssignNew(SubmitThrobber, SCircularThrobber)
						.Radius(15.f)
						.Visibility(EVisibility::Hidden)
					]

					+ SHorizontalBox::Slot()
					.Padding(5.0f, 0.f, 25.0f, 25.f)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.MaxWidth(128.f)
					[
						SNew(SButton)
						.DesiredSizeScale(FVector2D(2.f, 1.f))
						.OnClicked_Lambda([this, ModProperties]()
						{
							DrawModCreationToolWidget();
							return FReply::Handled();
						})
						[
							SNew(STextBlock)
							.Text(Localize("ModCreationToolBack", "Back"))
							.Justification(ETextJustify::Center)
						]
					]
				];
				CreateModDetailsView->SetObject(ModProperties);
				return FReply::Handled();
			})
		]
	];

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f))
	.AutoHeight()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f))
			.OnClicked_Lambda([this]() 
			{
				DrawThrobberWidget();
				
				FModioFilterParams FilterParams;
				ModioSubsystem->ListUserCreatedModsAsync(FilterParams, FOnListAllModsDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfoList> OptionalModList)
				{
					if (ErrorCode == false)
					{
						TArray<FModioModInfo> ModInfo = OptionalModList.GetValue().GetRawList();

						DrawBrowseModsWidget(ModInfo);
						
					}
				}));
				return FReply::Handled();
			})
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[	
					SNew(SImage)
					.Image(TexturePool["icon_EditMod"])
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(15.f, 0.f, 100.f, 0.5f)
				.MaxWidth(100.f)
				[
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.Font(ButtonTextStyle)
					.Text(Localize("EditMods", "Edit " + ModioGameInfo->UgcName))
					.Justification(ETextJustify::Center)
				]

				+ SHorizontalBox::Slot()
				.Padding(0.f, 0.f, 0.f, 0.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.MaxWidth(700.f)
				[
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.MinDesiredWidth(700.f)
					.AutoWrapText(true)
					.Font(ButtonTextStyle)
					.Text(Localize("EditModsDesc", "Edit your existing " + ModioGameInfo->UgcName + " for " + ModioGameInfo->Name))
					.Justification(ETextJustify::Left)
				]
			]
		]
	];

	VerticalBoxList->AddSlot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this]()
			{
				Logout(true);
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("ModioLogout", "Logout"))
				.Justification(ETextJustify::Center)
			]
		]
	];
}

void SModioEditorWindowCompoundWidget::DrawBrowseModsWidget(TArray<FModioModInfo> ModInfoList)
{
	ClearAllWidgets();

	UModioBrowseModsObject* BrowseModsProperties = NewObject<UModioBrowseModsObject>();
	BrowseModsProperties->Items = ModInfoList;
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedPtr<IDetailsView> EditModDetailsView = WindowManager::Get().GetPropertyModule().CreateDetailView(DetailsViewArgs);

	VerticalBoxList->AddSlot()
	.MaxHeight(256.f)
	.AutoHeight()
	[
		EditModDetailsView.ToSharedRef()
	];

	VerticalBoxList->AddSlot()
	.Padding(FMargin(0.f, 30.f, 0.f, 0.f))
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	.AutoHeight()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, BrowseModsProperties]()
			{
				if (!BrowseModsProperties->SelectedItem.IsValid())
				{
					FText Message = Localize("SelectMod", "Please select a mod to edit.");
					FMessageDialog::Open(EAppMsgType::Ok, Message);
					WindowManager::Get().GetWindow()->BringToFront();
					return FReply::Handled();;
				}

				DrawEditModWidget(BrowseModsProperties);
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("BrowseModsEditMod", "Edit Mod"))
				.Justification(ETextJustify::Center)
			]
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this]()
			{
				DrawModCreationToolWidget();
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("BrowseModsBack", "Back"))
				.Justification(ETextJustify::Center)
			]
		]
	];

	EditModDetailsView->SetObject(BrowseModsProperties);
}

void SModioEditorWindowCompoundWidget::DrawEditModWidget(UModioBrowseModsObject* BrowseModsProperties)
{
	ClearAllWidgets();

	UModioEditModParamsObject* EditModParams = NewObject<UModioEditModParamsObject>();
	EditModParams->Name = BrowseModsProperties->SelectedItem->ProfileName;
	EditModParams->Summary = BrowseModsProperties->SelectedItem->ProfileSummary;
	EditModParams->HomepageURL = BrowseModsProperties->SelectedItem->ProfileURL;

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedPtr<IDetailsView> EditModDetailsView = WindowManager::Get().GetPropertyModule().CreateDetailView(DetailsViewArgs);

	VerticalBoxList->AddSlot()
	[
		EditModDetailsView.ToSharedRef()
	];

	VerticalBoxList->AddSlot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 5.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, BrowseModsProperties, EditModParams]()
			{
				SubmitThrobber->SetVisibility(EVisibility::Visible);

				FModioEditModParams Params;
				Params.Name = EditModParams->Name;
				Params.Summary = EditModParams->Summary;
				Params.HomepageURL = EditModParams->HomepageURL;

				ModioSubsystem->SubmitModChangesAsync(BrowseModsProperties->SelectedItem->ModId, Params, FOnGetModInfoDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo)
				{
					if (ErrorCode == 0)
					{
						FText Message = Localize("ModChangedSuccessfully", FString::Printf(TEXT("Your mod changes submitted successfully.\n%s!"), *ErrorCode.GetErrorMessage()));
						FMessageDialog::Open(EAppMsgType::Ok, Message);

						DrawThrobberWidget();

						FModioFilterParams FilterParams;
						ModioSubsystem->ListUserCreatedModsAsync(FilterParams, FOnListAllModsDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfoList> OptionalModList)
						{
							if (ErrorCode == false)
							{
								TArray<FModioModInfo> ModInfo = OptionalModList.GetValue().GetRawList();
								DrawBrowseModsWidget(ModInfo);

							}
						}));
					}
					else
					{
						FText Message = Localize("ModChangedFailed", FString::Printf(TEXT("%s!"), *ErrorCode.GetErrorMessage()));
						FMessageDialog::Open(EAppMsgType::Ok, Message);
					}
					SubmitThrobber->SetVisibility(EVisibility::Hidden);
					WindowManager::Get().GetWindow()->BringToFront();
				}));
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("EditModsSubmit", "Submit"))
				.Justification(ETextJustify::Center)
			]
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SAssignNew(SubmitThrobber, SCircularThrobber)
			.Radius(15.f)
			.Visibility(EVisibility::Hidden)
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, BrowseModsProperties]()
			{
				DrawThrobberWidget();

				ModioSubsystem->GetModInfoAsync(BrowseModsProperties->SelectedItem->ModId, FOnGetModInfoDelegateFast::CreateLambda([this, BrowseModsProperties](FModioErrorCode ErrorCode, TOptional<FModioModInfo> ModInfo)
				{
					if (ErrorCode == 0)
					{
						DrawBrowseModFileWidget(ModInfo.GetValue());
					}
					else
					{
						DrawEditModWidget(BrowseModsProperties);
					}
				}));

				
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("EditModsEditFiles", "Edit Files"))
				.Justification(ETextJustify::Center)
			]
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this]()
			{
				DrawThrobberWidget();

				FModioFilterParams FilterParams;
				ModioSubsystem->ListUserCreatedModsAsync(FilterParams, FOnListAllModsDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfoList> OptionalModList)
				{
					if (ErrorCode == false)
					{
						TArray<FModioModInfo> ModInfo = OptionalModList.GetValue().GetRawList();
						DrawBrowseModsWidget(ModInfo);

					}
				}));
				return FReply::Handled();
				})
				[
					SNew(STextBlock)
					.Text(Localize("Back", "Back"))
					.Justification(ETextJustify::Center)
				]
		]
	];

	EditModDetailsView->SetObject(EditModParams);
}

void SModioEditorWindowCompoundWidget::DrawCreateModFileToolWidget(FModioModID ModID)
{
	ClearAllWidgets();

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f))
	.AutoHeight()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f))
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[	
					SNew(SImage)
					.Image(TexturePool["icon_CreateModPC"])
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.Padding(15.f, 0.f, 75.f, 0.f)
				.MaxWidth(200.f)
				[
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.Font(ButtonTextStyle)
					.Text(Localize("CreatePCMod", "Create a mod"))
					.Justification(ETextJustify::Center)
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.MaxWidth(700.f)
				[	
					SNew(STextBlock)
					.ColorAndOpacity(FLinearColor::White)
					.MinDesiredWidth(700.f)
					.AutoWrapText(true)
					.Font(ButtonTextStyle)
					.Text(Localize("CreatePCModDesc", "Lets you create a mod."))
					.Justification(ETextJustify::Left)
				]
			]
			.OnClicked_Lambda([this, ModID]() 
			{
				DrawCreateOrEditModFileWidget(ModID, {});
				return FReply::Handled();
			})
		]
	];

	VerticalBoxList->AddSlot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
		[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this]()
			{
				DrawModCreationToolWidget();
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("Back", "Back"))
				.Justification(ETextJustify::Center)
			]
		]
	];
}

void SModioEditorWindowCompoundWidget::DrawCreateOrEditModFileWidget(FModioModID ModID, FModioBrowseModFileStruct BrowseModFileObject)
{
	ClearAllWidgets();

	UModioCreateNewModFileParamsObject* ModFileParams = NewObject<UModioCreateNewModFileParamsObject>();

	if (!BrowseModFileObject.Name.IsEmpty())
	{
		ModFileParams->PathToModRootDirectory = BrowseModFileObject.ModInfo.FileInfo.Filename;
		ModFileParams->Changelog = BrowseModFileObject.ModInfo.FileInfo.Changelog;
		ModFileParams->VersionString = BrowseModFileObject.Version;
	}

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedPtr<IDetailsView> ModFileDetailsView = WindowManager::Get().GetPropertyModule().CreateDetailView(DetailsViewArgs);
	
	VerticalBoxList->AddSlot()
	.MaxHeight(200.f)
	.AutoHeight()
	[
		ModFileDetailsView.ToSharedRef()
	];
	ModFileDetailsView->SetObject(ModFileParams);

	VerticalBoxList->AddSlot()
	.Padding(FMargin(0.f, 15.f, 0.f, 0.f))
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SAssignNew(ModfileSubmitButton, SButton)
			.Visibility(EVisibility::Visible)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, ModFileParams, ModID, BrowseModFileObject]()
			{
				if(ModFileParams->PathToModRootDirectory.IsEmpty() || !FPaths::DirectoryExists(ModFileParams->PathToModRootDirectory))
				{
					FText Message = Localize("ChooseValidDir", "Uploading new mod file requires a valid workspace directory for the mod.");
					FMessageDialog::Open(EAppMsgType::Ok, Message);
					WindowManager::Get().GetWindow()->BringToFront();
					return FReply::Handled();
				}

				UploadModID = ModID;
				ShowProgressBar();

				FModioCreateModFileParams Params;
				Params.PathToModRootDirectory = ModFileParams->PathToModRootDirectory;
				Params.VersionString = ModFileParams->VersionString;
				Params.Changelog = ModFileParams->Changelog;
				Params.bSetAsActiveRelease = ModFileParams->bSetAsActiveRelease;
				Params.ModfilePlatforms = ModFileParams->ModfilePlatforms;
				Params.MetadataBlob = ModFileParams->MetadataBlob;
				ModioSubsystem->SubmitNewModFileForMod(ModID, Params);
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("Submit", "Submit"))
				.Justification(ETextJustify::Center)
			]
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SAssignNew(ModfileBackButton, SButton)
			.Visibility(EVisibility::Visible)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this]()
			{
				DrawThrobberWidget();

				FModioFilterParams FilterParams;
				ModioSubsystem->ListUserCreatedModsAsync(FilterParams, FOnListAllModsDelegateFast::CreateLambda([this](FModioErrorCode ErrorCode, TOptional<FModioModInfoList> OptionalModList)
				{
					if (ErrorCode == false)
					{
						TArray<FModioModInfo> ModInfo = OptionalModList.GetValue().GetRawList();
						DrawBrowseModsWidget(ModInfo);

					}
				}));
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("Back", "Back"))
				.Justification(ETextJustify::Center)
			]
		]
	];

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 15.f, 5.f, 5.f))
	.HAlign(HAlign_Left)
	.VAlign(VAlign_Bottom)
	.MaxHeight(15.f)
	[
		SNew(SHorizontalBox)

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		[
			SAssignNew(ProgressTitle, STextBlock)
			.Visibility(EVisibility::Hidden)
		]
	];

	VerticalBoxList->AddSlot()
	.Padding(FMargin(15.f, 5.f, 15.f, 15.f))
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Bottom)
	.AutoHeight()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.MaxWidth(WindowManager::Get().GetWindow()->GetSizeInScreen().X)
		[
			SAssignNew(ProgressBar, SProgressBar)
			.Visibility(EVisibility::Hidden)
			.Percent(Percentage)
		]

		+SHorizontalBox::Slot()
		.Padding(FMargin(15.f, 0.f, 0.f, 0.f))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		[
			SAssignNew(PercentageText, STextBlock)
			.Visibility(EVisibility::Hidden)
		]
	];
}

void SModioEditorWindowCompoundWidget::DrawBrowseModFileWidget(FModioModInfo ModInfo)
{
	ClearAllWidgets();

	FModioBrowseModFileStruct BrowseModFileParams;
	BrowseModFileParams.ModInfo = ModInfo;
	FModioFileMetadata ModioFileMetaData = ModInfo.FileInfo;
	BrowseModFileParams.Name = ModioFileMetaData.Filename;
	BrowseModFileParams.Version = ModioFileMetaData.Version;
	BrowseModFileParams.Platform = EModioModfilePlatform::Windows;

	#if ENGINE_MAJOR_VERSION >= 5
	static const UEnum* ModStatusEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModServerSideStatus"));
#else
	static const UEnum* ModStatusEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EModioModServerSideStatus"));
#endif
	FString Status = ModStatusEnum->GetNameStringByIndex(static_cast<uint32>(ModInfo.ModStatus));
	BrowseModFileParams.Status = FName::NameToDisplayString(Status, true);

	UModioBrowseModFileCollectionObject* ModFiles = NewObject<UModioBrowseModFileCollectionObject>();
	ModFiles->Items.Add(BrowseModFileParams);
	
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedPtr<IDetailsView> BrowseModFileDetailsView = WindowManager::Get().GetPropertyModule().CreateDetailView(DetailsViewArgs);

	VerticalBoxList->AddSlot()
	[
		BrowseModFileDetailsView.ToSharedRef()
	];

	BrowseModFileDetailsView->SetObject(ModFiles);

	VerticalBoxList->AddSlot()
	.HAlign(HAlign_Right)
	.VAlign(VAlign_Bottom)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, ModInfo, BrowseModFileParams]()
			{
				ModioSubsystem->GetGameInfoAsync(UModioSDKLibrary::GetProjectGameId(), FOnGetGameInfoDelegateFast::CreateLambda([this, ModInfo, BrowseModFileParams](FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo) 
				{
					if (ErrorCode)
					{
						FText Message = Localize("ModioGameInfoError", FString::Printf(TEXT("Could not retrieve game information from mod.io\n%s"), *ErrorCode.GetErrorMessage()));
						FMessageDialog::Open(EAppMsgType::Ok, Message);
						WindowManager::Get().GetWindow()->BringToFront();
						WindowManager::Get().CloseWindow();
						return;
					}
					if (GameInfo.IsSet())
					{
						DrawCreateOrEditModFileWidget(ModInfo.ModId, BrowseModFileParams);
					}
				}));
				
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("NewModfile", "New Modfile"))
				.Justification(ETextJustify::Center)
			]
		]

		+ SHorizontalBox::Slot()
		.Padding(5.0f, 0.f, 25.0f, 25.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.MaxWidth(128.f)
		[
			SNew(SButton)
			.DesiredSizeScale(FVector2D(2.f, 1.f))
			.OnClicked_Lambda([this, ModFiles, BrowseModFileDetailsView]()
			{
				DrawThrobberWidget();

				FModioFilterParams FilterParams;
				ModioSubsystem->ListUserCreatedModsAsync(FilterParams, FOnListAllModsDelegateFast::CreateLambda([this, ModFiles, BrowseModFileDetailsView](FModioErrorCode ErrorCode, TOptional<FModioModInfoList> OptionalModList)
				{
					if (ErrorCode == false)
					{
						TArray<FModioModInfo> ModInfo = OptionalModList.GetValue().GetRawList();
						DrawBrowseModsWidget(ModInfo);
					}
				}));
				return FReply::Handled();
			})
			[
				SNew(STextBlock)
				.Text(Localize("Back", "Back"))
				.Justification(ETextJustify::Center)
			]
		]
	];
}

void SModioEditorWindowCompoundWidget::ShowProgressBar()
{
	WindowManager::Get().GetWindow()->EnableWindow(false);
	ProgressBar->SetVisibility(EVisibility::Visible);
	ProgressTitle->SetVisibility(EVisibility::Visible);
	PercentageText->SetVisibility(EVisibility::Visible);
	ModfileSubmitButton->SetVisibility(EVisibility::Hidden);
	ModfileBackButton->SetVisibility(EVisibility::Hidden);
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
			Percentage = (float)CurrentProgress / (float)TotalProgress;
			ProgressTitle->SetText(Localize("CompressingModfile", "Compressing Modfile ..."));
			ProgressBar->SetPercent(Percentage);
			PercentageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(Percentage * 100)) + " %"));
		}
		else if (ProgressInfo->GetCurrentState() == EModioModProgressState::Uploading)
		{
			FModioUnsigned64 CurrentProgress = ProgressInfo->GetCurrentProgress(EModioModProgressState::Uploading);
			FModioUnsigned64 TotalProgress = ProgressInfo->GetTotalProgress(EModioModProgressState::Uploading);
			Percentage = (float)CurrentProgress / (float)TotalProgress;
			ProgressTitle->SetText(Localize("UploadingModfile", "Uploading Modfile ..."));
			ProgressBar->SetPercent(Percentage);
			PercentageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(Percentage * 100)) + " %"));
		}
	}
}

void SModioEditorWindowCompoundWidget::HideProgressBar()
{
	WindowManager::Get().GetWindow()->EnableWindow(true);
	ProgressBar->SetVisibility(EVisibility::Hidden);
	ProgressTitle->SetVisibility(EVisibility::Hidden);
	PercentageText->SetVisibility(EVisibility::Hidden);
	ModfileSubmitButton->SetVisibility(EVisibility::Visible);
	ModfileBackButton->SetVisibility(EVisibility::Visible);
}

void SModioEditorWindowCompoundWidget::Logout(bool Exit)
{
	if (ModioSubsystem)
	{
		ModioSubsystem->ClearUserDataAsync(FOnErrorOnlyDelegateFast::CreateLambda([this, Exit](FModioErrorCode ErrorCode) 
			{
				if (ErrorCode == 0)
				{
					DrawLoginWidget();
					WindowManager::Get().GetWindow()->BringToFront();
				}
			
			ModioSubsystem->ShutdownAsync(FOnErrorOnlyDelegateFast::CreateLambda([this, Exit](FModioErrorCode ErrorCode)
			{
				if (ErrorCode == 0) 
				{
					if (Exit)
					{
						WindowManager::Get().CloseWindow();
					}
				}
			}));
		}));
	}
}

void SModioEditorWindowCompoundWidget::UnloadResources() 
{
	ModioEmailEditableTextBox = nullptr;
	ModioAuthenticationCodeEditableTextBox = nullptr;
	HeaderBackgroundBrush = nullptr;
	BackgroundBrush = nullptr;
	SubmitThrobber = nullptr;
	ProgressBar = nullptr;
	PercentageText = nullptr;
	ModfileSubmitButton = nullptr;
	ModfileBackButton = nullptr;
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

void SModioEditorWindowCompoundWidget::DownloadGameLogo(FString URL)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([this, URL](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
	{
		const TArray<uint8> bytes = Response->GetContent();
		const FString content = Response->GetContentAsString();

		if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			UE_LOG(ModioEditor, Error, TEXT("DownloadGameLogo - Request Failed -_url: %s | content: %s"), *URL, *content);
			return;
		}

		FString LogoPath = ResourcesPath + "Downloaded/" + "Logo_" + ModioGameInfo->GameID.ToString() + ".png";
		FFileHelper::SaveArrayToFile(bytes, *LogoPath);

		FString Key = FPaths::GetBaseFilename(LogoPath);
		FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(FName(*LogoPath), GameLogoSize);
		TexturePool.Add(Key, DynamicImageBrush);
			
		DrawLogoWidget();
		ModioGameLogo->SetImage(TexturePool[Key]);

		ModioSubsystem->VerifyUserAuthenticationAsync(FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorWindowCompoundWidget::OnVerifyCurrentUserAuthenticationCompleted));
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
#if ENGINE_MAJOR_VERSION >= 5
	static const UEnum* PlatformEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModfilePlatform"));
#else
	static const UEnum* PlatformEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EModioModfilePlatform"));
#endif
	int32 Index = PlatformEnum->GetIndexByNameString(PlatformString);
	return EModioModfilePlatform((uint8)Index);
}

FString SModioEditorWindowCompoundWidget::ToPlatformString(EModioModfilePlatform Platform) 
{
#if ENGINE_MAJOR_VERSION >= 5 
	static const UEnum* PlatformEnum = FindObject<UEnum>(nullptr, TEXT("/Script/Modio.EModioModfilePlatform"));
#else
	static const UEnum* PlatformEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EModioModfilePlatform"));
#endif
	return PlatformEnum->GetNameStringByIndex(static_cast<uint32>(Platform));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE