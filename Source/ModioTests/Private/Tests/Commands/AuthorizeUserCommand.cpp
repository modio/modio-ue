/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Tests/Commands/AuthorizeUserCommand.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/CommandLine.h"
#include "Engine/Engine.h"

void FModioAuthorizeUserCommand::Start()
{
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return;
	}

	FString AuthToken;
	if (!FParse::Value(FCommandLine::Get(), TEXT("AutomationAuthToken="), AuthToken))
	{
		AddError(TEXT("No AutomationAuthToken provided for automation tests"));
		return;
	}

	FString AuthEmail;
	if (!FParse::Value(FCommandLine::Get(), TEXT("AutomationAuthEmail="), AuthEmail))
	{
		AddError(TEXT("No AutomationAuthEmail provided for automation tests"));
		return;
	}

	FModioAuthenticationParams Params = {AuthToken, AuthEmail, true, {}};

	ModioSubsystem->AuthenticateUserExternalAsync(
		Params, EModioAuthenticationProvider::OpenID,
		FOnErrorOnlyDelegateFast::CreateLambda([this, ModioSubsystem, WeakThis = AsWeak()](FModioErrorCode ec) {
			if (!WeakThis.IsValid())
			{
				// The command has been destroyed by the time this callback is called
				return;
			}
			if (ec)
			{
				AddError(
					FString::Printf(TEXT("Failed to authenticate for automation tests: %s"), *ec.GetErrorMessage()));
			}
			else
			{
				AddInfo(TEXT("Successfully authenticated for automation tests"));
				ModioSubsystem->EnableModManagement(FOnModManagementDelegateFast());
			}
		}));
}

bool FModioAuthorizeUserCommand::ModioUpdate(float DeltaTime)
{
	return CheckAuthCompletion();
}

bool FModioAuthorizeUserCommand::CheckAuthCompletion()
{
	UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!ModioSubsystem)
	{
		AddError(TEXT("ModioSubsystem not found"));
		return true;
	}

	if (bAuthCompleted)
	{
		AddInfo(TEXT("User authenticated successfully"));
		return true;
	}

	if (!bAuthCheckInFlight && RepeatedLogTimer <= 0.0f)
	{
		RepeatedLogTimer = 1.0f;
		bAuthCheckInFlight = true;
		AddInfo(TEXT("Checking for user authentication"));
		ModioSubsystem->VerifyUserAuthenticationAsync(
			FOnErrorOnlyDelegateFast::CreateLambda([this, WeakThis = AsWeak()](FModioErrorCode ErrorCode) {
				if (!WeakThis.IsValid())
				{
					// The command has been destroyed by the time this callback is called
					return;
				}
				if (ErrorCode)
				{
					bAuthCheckInFlight = false;
					AddInfo(TEXT("Not authorized, waiting..."));
				}
				else
				{
					bAuthCompleted = true;
					AddInfo(TEXT("User authentication verified successfully"));
				}
			}));
	}

	return false;
}

#endif