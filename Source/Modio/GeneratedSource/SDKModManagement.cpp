/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io SDK.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *
 */

#pragma once

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/ModioSDK.h"
#endif

#include "modio/core/ModioCoreTypes.h"
#include "modio/core/ModioModCollectionEntry.h"
#include "modio/core/ModioServices.h"
#include "modio/detail/AsioWrapper.h"
#include "modio/detail/ModioSDKSessionData.h"
#include "modio/detail/ops/ModManagementLoop.h"
#include "modio/detail/ops/SubscribeToModOp.h"
#include "modio/detail/ops/UnsubscribeFromMod.h"
#include "modio/detail/ops/mod/SubmitNewModFileOp.h"
#include "modio/detail/ops/mod/SubmitNewModOp.h"
#include "modio/detail/ops/modmanagement/ForceUninstallModOp.h"
#include "modio/impl/SDKPreconditionChecks.h"
#include "modio/userdata/ModioUserDataService.h"

namespace Modio
{
	Modio::ErrorCode EnableModManagement(std::function<void(Modio::ModManagementEvent)> ModManagementHandler)
	{
		if (!Modio::Detail::SDKSessionData::IsInitialized())
		{
			return Modio::make_error_code(Modio::GenericError::SDKNotInitialized);
		}
		if (Modio::Detail::SDKSessionData::IsModManagementEnabled())
		{
			return Modio::make_error_code(Modio::ModManagementError::ModManagementAlreadyEnabled);
		}
		Modio::Detail::SDKSessionData::SetUserModManagementCallback(ModManagementHandler);
		Modio::Detail::SDKSessionData::AllowModManagement();
		Modio::Detail::BeginModManagementLoopAsync([](Modio::ErrorCode ec) mutable {
			if (ec)
			{
				Modio::Detail::Logger().Log(LogLevel::Info, Modio::LogCategory::Core,
											"Mod Management Loop halted: status message {}", ec.message());
			}
		});
		return {};
	}
	void DisableModManagement()
	{
		Modio::Detail::SDKSessionData::SetUserModManagementCallback(std::function<void(Modio::ModManagementEvent)> {});
		Modio::Detail::SDKSessionData::DisableModManagement();
	}

	void FetchExternalUpdatesAsync(std::function<void(Modio::ErrorCode)> OnFetchDone)
	{
		if (Modio::Detail::RequireSDKIsInitialized(OnFetchDone) && Modio::Detail::RequireNotRateLimited(OnFetchDone) &&
			Modio::Detail::RequireUserIsAuthenticated(OnFetchDone) &&
			Modio::Detail::RequireModManagementEnabled(OnFetchDone))
		{
			Modio::Detail::FetchExternalUpdatesAsync(OnFetchDone);
		}
	}

	void SubscribeToModAsync(Modio::ModID ModToSubscribeTo, std::function<void(Modio::ErrorCode)> OnSubscribeComplete)
	{
		if (Modio::Detail::RequireSDKIsInitialized(OnSubscribeComplete) &&
			Modio::Detail::RequireNotRateLimited(OnSubscribeComplete) &&
			Modio::Detail::RequireUserIsAuthenticated(OnSubscribeComplete) &&
			Modio::Detail::RequireModManagementEnabled(OnSubscribeComplete))

		{
			return asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::SubscribeToModOp(Modio::Detail::SDKSessionData::CurrentGameID(),
												Modio::Detail::SDKSessionData::CurrentAPIKey(), ModToSubscribeTo),
				OnSubscribeComplete, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void UnsubscribeFromModAsync(Modio::ModID ModToUnsubscribeFrom,
								 std::function<void(Modio::ErrorCode)> OnUnsubscribeComplete)
	{
		if (Modio::Detail::RequireSDKIsInitialized(OnUnsubscribeComplete) &&
			Modio::Detail::RequireNotRateLimited(OnUnsubscribeComplete) &&
			Modio::Detail::RequireUserIsAuthenticated(OnUnsubscribeComplete) &&
			Modio::Detail::RequireModManagementEnabled(OnUnsubscribeComplete))
		{
			return Modio::Detail::UnsubscribeFromModAsync(ModToUnsubscribeFrom, OnUnsubscribeComplete);
		}
	}

	bool IsModManagementBusy()
	{
		if (!Modio::Detail::SDKSessionData::IsModManagementEnabled())
		{
			return false;
		}

		Modio::ModCollection UserModCollection =
			Modio::Detail::SDKSessionData::FilterSystemModCollectionByUserSubscriptions();

		for (auto ModEntry : UserModCollection.Entries())
		{
			Modio::ModState CurrentState = ModEntry.second->GetModState();
			if (CurrentState != Modio::ModState::Installed)
			{
				return true;
			}
		}
		return false;
	}

	Modio::Optional<Modio::ModProgressInfo> QueryCurrentModUpdate()
	{
		return Modio::Detail::SDKSessionData::GetModProgress();
	}

	std::map<Modio::ModID, Modio::ModCollectionEntry> QueryUserSubscriptions()
	{
		Modio::Detail::UserDataService& UserService =
			Modio::Detail::Services::GetGlobalService<Modio::Detail::UserDataService>();
		Modio::ModCollection UserModCollection =
			Modio::Detail::SDKSessionData::FilterSystemModCollectionByUserSubscriptions();
		std::map<Modio::ModID, ModCollectionEntry> UserSubscriptions;

		for (auto ModEntry : UserModCollection.Entries())
		{
			UserSubscriptions.emplace(std::make_pair(ModEntry.first, (*ModEntry.second)));
		}
		return UserSubscriptions;
	}

	std::map<Modio::ModID, Modio::ModCollectionEntry> QueryUserInstallations(bool bIncludeOutdatedMods)
	{
		Modio::Detail::UserDataService& UserService =
			Modio::Detail::Services::GetGlobalService<Modio::Detail::UserDataService>();
		Modio::ModCollection UserModCollection =
			Modio::Detail::SDKSessionData::FilterSystemModCollectionByUserSubscriptions();
		std::map<Modio::ModID, ModCollectionEntry> UserInstallations;

		// Only return mods that are either installed, and if bIncludeOutdatedMods mods that are installed but have
		// an update available that isn't currently being processed
		for (auto ModEntry : UserModCollection.Entries())
		{
			if (ModEntry.second->GetModState() == ModState::Installed)
			{
				UserInstallations.emplace(std::make_pair(ModEntry.first, (*ModEntry.second)));
			}
			if (bIncludeOutdatedMods && ModEntry.second->GetModState() == ModState::UpdatePending)
			{
				UserInstallations.emplace(std::make_pair(ModEntry.first, (*ModEntry.second)));
			}
		}
		return UserInstallations;
	}

	std::map<Modio::ModID, Modio::ModCollectionEntry> QuerySystemInstallations()
	{
		std::map<Modio::ModID, ModCollectionEntry> InstalledMods;
		Modio::ModCollection& AllInstalledMods = Modio::Detail::SDKSessionData::GetSystemModCollection();
		for (auto& ModEntry : AllInstalledMods.Entries())
		{
			InstalledMods.emplace(std::make_pair(ModEntry.first, (*ModEntry.second)));
		}
		return InstalledMods;
	}

	void ForceUninstallModAsync(Modio::ModID ModToRemove, std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireUserIsAuthenticated(Callback) &&
			Modio::Detail::RequireModManagementEnabled(Callback) &&
			Modio::Detail::RequireUserNotSubscribed(ModToRemove, Callback))
		{
			asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::ForceUninstallModOp(ModToRemove), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void SubmitNewModAsync(Modio::ModCreationHandle Handle, Modio::CreateModParams Params,
						   std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModID> CreatedModID)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireUserIsAuthenticated(Callback))
		{
			if (Modio::Optional<Modio::ModID> ResolvedID =
					Modio::Detail::SDKSessionData::ResolveModCreationHandle(Handle))
			{
				Modio::Detail::Logger().Log(
					Modio::LogLevel::Warning, Modio::LogCategory::ModManagement,
					"Attempted to call SubmitNewModAsync with an already-used handle. Returning existing Mod ID");
				asio::post(Modio::Detail::Services::GetGlobalContext().get_executor(),
						   [ID = ResolvedID.value(), Callback]() { Callback({}, ID); });
				return;
			}
			return Modio::Detail::SubmitNewModAsync(Handle, Params, Callback);
		}
	}
	Modio::ModCreationHandle GetModCreationHandle()
	{
		return Modio::Detail::SDKSessionData::GetNextModCreationHandle();
	}

	MODIOSDK_API void SubmitNewModFileForMod(Modio::ModID Mod, Modio::CreateModFileParams Params)
	{
		//TODO: @modio-core we should return the error code from this function so we can do our precondition checks
		Modio::Detail::SDKSessionData::AddPendingModfileUpload(Mod, Params);

		//(Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireUserIsAuthenticated(Callback)) && RequireModManagementEnabled
		
	}

} // namespace Modio
