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

#include "modio/cache/ModioCacheService.h"
#include "modio/detail/ModioSDKSessionData.h"
#include "modio/detail/ops/ServiceInitializationOp.h"
#include "modio/detail/ops/ReportContentOp.h"
#include "modio/detail/ops/Shutdown.h"
#include "modio/file/ModioFileService.h"
#include "modio/http/ModioHttpService.h"
#include "modio/impl/SDKPostAsync.h"
#include "modio/impl/SDKPreconditionChecks.h"
#include "modio/userdata/ModioUserDataService.h"
// Implementation header - do not include directly

#include <functional>

namespace Modio
{
	void InitializeAsync(Modio::InitializeOptions InitOptions, std::function<void(Modio::ErrorCode)> OnInitComplete)
	{
		if (Modio::Detail::RequireValidInitParams(InitOptions, OnInitComplete))
		{
			auto WrappedCallback = Modio::Detail::ApplyPostAsyncChecks(OnInitComplete);
			return asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				ServiceInitializationOp(InitOptions),
				WrappedCallback, Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void RunPendingHandlers()
	{
		// Run any pending handlers on the global io_context
		if (Modio::Detail::Services::GetGlobalContext().stopped())
		{
			Modio::Detail::Services::GetGlobalContext().restart();
		}
		Modio::Detail::Services::GetGlobalContext().poll();

		// invoke the mod management log callback if the user has set it
		Modio::Detail::SDKSessionData::FlushModManagementLog();
		// invoke log callback if the user has set it
		Modio::Detail::Services::GetGlobalService<Modio::Detail::LogService>().FlushLogBuffer();
	}

#ifndef MODIO_SEPARATE_COMPILATION
	// Forward declaration
	void DisableModManagement();
#endif

	void Shutdown()
	{
		// We need to be initialized to shutdown
		if (!Modio::Detail::SDKSessionData::IsInitialized())
		{
			return;
		}

		Modio::DisableModManagement();
		Modio::Detail::Services::GetGlobalService<Modio::Detail::CacheService>().Shutdown();
		Modio::Detail::Services::GetGlobalService<Modio::Detail::UserDataService>().Shutdown();
		Modio::Detail::Services::GetGlobalService<Modio::Detail::HttpService>().Shutdown();
		Modio::Detail::Services::GetGlobalService<Modio::Detail::FileService>().Shutdown();
		Modio::Detail::Services::GetGlobalService<Modio::Detail::LogService>().Shutdown();
		Modio::Detail::SDKSessionData::Deinitialize();
		auto OldContext = Modio::Detail::Services::ResetGlobalContext();
		auto ExhaustOldContext = [OldContext]() mutable {
			OldContext->restart();
			OldContext->run();
			OldContext.reset();
		};
		asio::post(Modio::Detail::Services::GetGlobalContext().get_executor(), std::move(ExhaustOldContext));

		// Ensure that we run the exhaust function and empties out the work queue
		Modio::Detail::Services::GetGlobalContext().restart();
		Modio::Detail::Services::GetGlobalContext().run();
	}

	// This might need a timeout parameter
	void ShutdownAsync(std::function<void(Modio::ErrorCode)> OnShutdownComplete)
	{
		if (Modio::Detail::RequireSDKIsInitialized(OnShutdownComplete))
		{
			// Halt the mod management loop
			Modio::DisableModManagement();
			// Signal the internal services their operations should shut down ASAP
			Modio::Detail::Services::GetGlobalService<Modio::Detail::CacheService>().Shutdown();
			Modio::Detail::Services::GetGlobalService<Modio::Detail::UserDataService>().Shutdown();
			Modio::Detail::Services::GetGlobalService<Modio::Detail::HttpService>().Shutdown();
			Modio::Detail::Services::GetGlobalService<Modio::Detail::FileService>().Shutdown();
			Modio::Detail::Services::GetGlobalService<Modio::Detail::LogService>().Shutdown();
			Modio::Detail::SDKSessionData::Deinitialize();
			// Steal the old io context (and associated services)
			auto OldContext = Modio::Detail::Services::ResetGlobalContext();
			// Post to the new io_context an operation that will exhaust the old context and call the user callback when
			// it's completed
			asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::ShutdownOp(std::move(OldContext)), OnShutdownComplete,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}

	void SetLogLevel(Modio::LogLevel Level)
	{
		Modio::Detail::LogService::SetGlobalLogLevel(Level);
	}

	void SetLogCallback(std::function<void(Modio::LogLevel Level, const std::string& Message)> LogCallback)
	{
		Modio::Detail::LogService::SetLogCallback(LogCallback);
	}

	std::vector<Modio::FieldError> GetLastValidationError()
	{
		return Modio::Detail::SDKSessionData::GetLastValidationError();
	}

	void ReportContentAsync(Modio::ReportParams Report, std::function<void(Modio::ErrorCode)> Callback)
	{
		if (Modio::Detail::RequireSDKIsInitialized(Callback))
		{
			
			asio::async_compose<std::function<void(Modio::ErrorCode)>, void(Modio::ErrorCode)>(
				Modio::Detail::ReportContentOp(Report), Callback,
				Modio::Detail::Services::GetGlobalContext().get_executor());
		}
	}
} // namespace Modio
