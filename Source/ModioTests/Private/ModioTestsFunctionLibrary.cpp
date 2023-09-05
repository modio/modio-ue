// Fill out your copyright notice in the Description page of Project Settings.

#include "ModioTestsFunctionLibrary.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "ModioTestSettings.h"
#include "Types/ModioCommonTypes.h"

#if WITH_DEV_AUTOMATION_TESTS
FModioInitializeOptions UModioTestsFunctionLibrary::GetAutomationTestOptions()
{
	#if WITH_EDITORONLY_DATA
	const UModioTestSettings* Settings = GetDefault<UModioTestSettings>();
	FModioInitializeOptions Options = Settings->AutomationTestOptions;
	if (!Settings->AutomationSessionID.IsEmpty())
	{
		Options.LocalSessionIdentifier = Settings->AutomationSessionID;
	}
	else
	{
		Options.LocalSessionIdentifier = "test";
	}

	if (IFileManager::Get().FileExists(*(FPaths::ProjectSavedDir() / "test_server")))
	{
		FString ServerURL;
		if (FFileHelper::LoadFileToString(ServerURL, *(FPaths::ProjectSavedDir() / "test_server")) &&
			ServerURL.Len() > 0)
		{
			Options.ExtendedInitializationParameters.Add("ServerURL", ServerURL);
			UE_LOG(LogTemp, Log, TEXT("Overriding server URL: %s"), (*ServerURL));
		}
	}
	if (IFileManager::Get().FileExists(*(FPaths::ProjectSavedDir() / "test_auth")))
	{
		FString AuthToken;
		if (FFileHelper::LoadFileToString(AuthToken, *(FPaths::ProjectSavedDir() / "test_auth")) && AuthToken.Len() > 0)
		{
			Options.ExtendedInitializationParameters.Add("OAuthToken", AuthToken);
			UE_LOG(LogTemp, Log, TEXT("Injecting OAuth Token"));
		}
	}
	return Options;
	#else
	UE_LOG(LogTemp, Log, TEXT("Returning empty automation test init options"));
	return FModioInitializeOptions {};
	#endif
}

FModioModID UModioTestsFunctionLibrary::GetTestModID() 
{
	return FModioModID(50);
}

#endif