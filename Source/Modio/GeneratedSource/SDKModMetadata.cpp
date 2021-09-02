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

#include "modio/core/ModioStdTypes.h"
#include "modio/core/entities/ModioModDetails.h"
#include "modio/core/entities/ModioModInfo.h"
#include "modio/core/entities/ModioModInfoList.h"
#include "modio/core/entities/ModioModTagOptions.h"
#include "modio/detail/ops/mod/GetModDependenciesOp.h"
#include "modio/detail/ops/mod/GetModDetailsOp.h"
#include "modio/detail/ops/mod/GetModInfoOp.h"
#include "modio/detail/ops/mod/GetModMediaAvatarOp.h"
#include "modio/detail/ops/mod/GetModMediaGalleryOp.h"
#include "modio/detail/ops/mod/GetModMediaLogoOp.h"
#include "modio/detail/ops/mod/GetModTagsOp.h"
#include "modio/detail/ops/mod/ListAllModsOp.h"
#include "modio/detail/ops/mod/SubmitModRatingOp.h"
#include "modio/impl/SDKPreconditionChecks.h"

// Implementation header - do not include directly

namespace Modio
{
	void ListAllModsAsync(FilterParams Filter,
						  std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModInfoList>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModInfoList>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::ModInfoList>)>(
				Modio::Detail::ListAllModsOp(Modio::Detail::SDKSessionData::CurrentGameID(), std::move(Filter)),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void GetModInfoAsync(Modio::ModID ModId,
						 std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModInfo>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModInfo>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::ModInfo>)>(
				Modio::Detail::GetModInfoOp(Modio::Detail::SDKSessionData::CurrentGameID(),
											Modio::Detail::SDKSessionData::CurrentAPIKey(), ModId),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}
// Disabled
#if (0)
	void GetModFileDetailsAsync(Modio::ModID ModId,
								std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModDetails>)> Callback)
	{
		return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModDetails>)>,
								   void(Modio::ErrorCode, Modio::Optional<Modio::ModDetails>)>(
			Modio::Detail::GetModDetailsOp(Modio::Detail::SDKSessionData::CurrentGameID(),
										   Modio::Detail::SDKSessionData::CurrentAPIKey(), ModId),
			Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
	}
#endif
	void GetModMediaAsync(Modio::ModID ModId, Modio::LogoSize LogoSize,
						  std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>(
				Modio::Detail::GetModMediaLogoOp(Modio::Detail::SDKSessionData::CurrentGameID(),
												 Modio::Detail::SDKSessionData::CurrentAPIKey(), ModId, LogoSize),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void GetModMediaAsync(Modio::ModID ModId, Modio::AvatarSize AvatarSize,
						  std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>(
				Modio::Detail::GetModMediaAvatarOp(Modio::Detail::SDKSessionData::CurrentGameID(),
												   Modio::Detail::SDKSessionData::CurrentAPIKey(), ModId, AvatarSize),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void GetModMediaAsync(Modio::ModID ModId, Modio::GallerySize GallerySize, Modio::GalleryIndex Index,
						  std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::filesystem::path>)>(
				Modio::Detail::GetModMediaGalleryOp(Modio::Detail::SDKSessionData::CurrentGameID(),
													Modio::Detail::SDKSessionData::CurrentAPIKey(), ModId, GallerySize,
													Index),
				Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void GetModTagOptionsAsync(std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModTagOptions>)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModTagOptions>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::ModTagOptions>)>(
				Modio::Detail::GetModTagsOp(), Callback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void SubmitModRatingAsync(Modio::ModID ModID, Modio::Rating Rating, std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback) &&
			Modio::Detail::RequireUserIsAuthenticated(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::SubmitModRatingOp(ModID, Rating), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void GetModDependenciesAsync(
		Modio::ModID ModID,
		std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModDependencyList> Dependencies)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback) && Modio::Detail::RequireNotRateLimited(Callback))
		{
			return asio::async_compose<std::function<void(Modio::ErrorCode, Modio::Optional<Modio::ModDependencyList>)>,
									   void(Modio::ErrorCode, Modio::Optional<Modio::ModDependencyList>)>(
				Modio::Detail::GetModDependenciesOp(ModID, Modio::Detail::SDKSessionData::CurrentGameID()), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}
} // namespace Modio
