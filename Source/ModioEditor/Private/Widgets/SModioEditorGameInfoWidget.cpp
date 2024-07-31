/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "Widgets/SModioEditorGameInfoWidget.h"

#include "EngineMinimal.h"

#include "Libraries/ModioSDKLibrary.h"
#include "WindowManager.h"
#include "ModioSettings.h"

#include "Widgets/Text/STextBlock.h"

#include "Widgets/Images/SThrobber.h"
#include "Interfaces/IPluginManager.h"
#include "HAL/FileManagerGeneric.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "SHyperlinkLaunchURL.h"

#define LOCTEXT_NAMESPACE "ModioEditorGameInfoWidget"

void SModioEditorGameInfoWidget::LoadModioSubsystem()
{
	FModioInitializeOptions InitializeOptions =
		UModioSDKLibrary::GetProjectInitializeOptionsForSessionId(FString("ModioUnrealEditor"));

	if (GEngine)
	{
		ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (ModioSubsystem)
		{
			ModioSubsystem->InitializeAsync(InitializeOptions, FOnErrorOnlyDelegateFast::CreateRaw(
																   this, &SModioEditorGameInfoWidget::OnInitCallback));
		}
	}
}

void SModioEditorGameInfoWidget::OnInitCallback(FModioErrorCode ErrorCode)
{
	if (ErrorCode.GetValue() == 0 || ErrorCode.GetValue() == 21769)
	{
		UE_LOG(LogTemp, Display,
			   TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - ModioSubsystem initialized."));
		ModioSubsystem->VerifyUserAuthenticationAsync(
			FOnErrorOnlyDelegateFast::CreateRaw(this, &SModioEditorGameInfoWidget::OnUserAuthCheckResponse));
	}
	else
	{
		UE_LOG(LogTemp, Error,
			   TEXT("ModioSubsystem - UserAuthWidget - OnInitCallback - Could not initialize ModioSubsystem - "
					"ErrorCode: %d | "
					"ErrorMessage: %s"),
			   ErrorCode.GetValue(), *ErrorCode.GetErrorMessage());
		DrawGameInfoInvalid(ErrorCode);
	}
}

void SModioEditorGameInfoWidget::Construct(const SModioEditorGameInfoWidget::FArguments& InArgs)
{
	LoadResources();
	
	if(!InArgs._GameId)
	{
		GameId = TOptional<FModioGameID>();
	}
	else
	{
		GameId = InArgs._GameId;
	}

	ChildSlot
	[	
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		[
			SAssignNew(RootWidget, SVerticalBox)
		]
	];
	
	DrawThrobberWidget();
	LoadModioSubsystem();
}

void SModioEditorGameInfoWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (ModioSubsystem && !ModioSubsystem->IsUsingBackgroundThread())
	{
		ModioSubsystem->RunPendingHandlers();
	}
}

void SModioEditorGameInfoWidget::ClearAllWidgets()
{
	if (RootWidget.IsValid())
	{
		if (RootWidget->GetAllChildren()->Num() > 0)
		{
			RootWidget->ClearChildren();
		}
	}
}

void SModioEditorGameInfoWidget::DrawThrobberWidget()
{
	ClearAllWidgets();

	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.f)
		[
			SNew(SCircularThrobber).Radius(50.f)
		];
}

void SModioEditorGameInfoWidget::LoadResources()
{
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
}

void SModioEditorGameInfoWidget::UnloadResources()
{
	LoadedGameLogo = nullptr;
	LoadedGameInfo = nullptr;
	RootWidget = nullptr;
	TexturePool.Empty();
}

void SModioEditorGameInfoWidget::OnUserAuthCheckResponse(FModioErrorCode ErrorCode)
{
	if (ErrorCode.GetValue() == 0)
	{
		LoadGameInfo();
	}
	else
	{
		DrawGameInfoInvalid(ErrorCode);
	}
}

void SModioEditorGameInfoWidget::LoadGameInfo()
{
	ModioSubsystem->GetGameInfoAsync(
		(GameId.IsSet() ? GameId.GetValue() : FModioGameID(GetDefault<UModioSettings>()->GameId)),
		FOnGetGameInfoDelegateFast::CreateRaw(this, &SModioEditorGameInfoWidget::OnLoadGameInfoResponse));
}

void SModioEditorGameInfoWidget::DownloadGameLogo(FString URL)
{
	FHttpModule* httpModule = &FHttpModule::Get();

#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 26)
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
#else
	TSharedRef<IHttpRequest> Request = httpModule->CreateRequest();
#endif

	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
		const TArray<uint8> bytes = Response->GetContent();
		const FString content = Response->GetContentAsString();

		UE_LOG(LogTemp, Warning, TEXT("Response Content: %s"), *content);

		if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			UE_LOG(LogTemp, Error, TEXT("DownloadGameLogo - Request Failed -_url: %s | content: %s"), *URL, *content);
			return;
		}

		FString LogoPath = ResourcesPath + "Downloaded/" + "Logo_" + GameId.GetValue().ToString() + ".png";
		FFileHelper::SaveArrayToFile(bytes, *LogoPath);

		FString Key = FPaths::GetBaseFilename(LogoPath);
		FSlateDynamicImageBrush* DynamicImageBrush = new FSlateDynamicImageBrush(FName(*LogoPath), GameLogoSize);
		TexturePool.Add(Key, DynamicImageBrush);

		DrawGameInfo();
		if (LoadedGameLogo.IsValid())
			LoadedGameLogo->SetImage(TexturePool[Key]);
	});

	Request->ProcessRequest();
}

void SModioEditorGameInfoWidget::OnLoadGameInfoResponse(FModioErrorCode ErrorCode, TOptional<FModioGameInfo> GameInfo)
{
	if(ErrorCode == 0 && GameInfo.IsSet())
	{
		LoadedGameInfo = TSharedPtr<FModioGameInfo>(&GameInfo.GetValue());
		FString LogoPath = ResourcesPath + "Downloaded/" + "Logo_" + LoadedGameInfo->GameID.ToString() + ".png";
		if (!FPaths::FileExists(LogoPath))
		{
			DownloadGameLogo(GameInfo.GetValue().Logo.Thumb320x180);
		}
		else
		{
			DrawGameInfo();
			FString Key = FPaths::GetBaseFilename(LogoPath);
			if (LoadedGameLogo.IsValid())
				LoadedGameLogo->SetImage(TexturePool[Key]);
		}
		
	}
	else
	{
		DrawGameInfoInvalid(ErrorCode);
	}
}

void SModioEditorGameInfoWidget::DrawGameInfo()
{
	ClearAllWidgets();

	RootWidget->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 20.f))
	.MaxHeight(175.f)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Top)
	.AutoHeight()
	[
		SAssignNew(LoadedGameLogo, SImage)
	];
	RootWidget->AddSlot()
	.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
	.MaxHeight(175.f)
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Fill)
	[
		SNew(STextBlock)
			.Text(FText::FromString(LoadedGameInfo->Summary))
	];
	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 0.f, 15.f, 0.f))
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			SNew(SHyperlinkLaunchURL, LoadedGameInfo->ProfileUrl)
						  .Text(LOCTEXT("GameIntructionsURL", "Game Page"))
						  .ToolTipText(LOCTEXT("GameInstructionURLTooltip",
											   "Opens the given game's modding instructions URL."))
		];
}

void SModioEditorGameInfoWidget::DrawGameInfoInvalid(FModioErrorCode ErrorCode)
{
	ClearAllWidgets();
	RootWidget->AddSlot()
		.Padding(FMargin(15.f, 15.f, 15.f, 15.f))
		.MaxHeight(175.f)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SNew(STextBlock)
				.Text(FText::FromString("Bad Response: " + ErrorCode.GetErrorMessage()))
		];
}

#undef LOCTEXT_NAMESPACE
