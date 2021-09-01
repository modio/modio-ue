/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/core/ModioLogService.h"
#endif

namespace Modio
{
	namespace Detail
	{
		LogService::LogService(asio::io_context& IOService)
			: asio::detail::service_base<LogService>(IOService),
			  CurrentLogLevel(LogLevel::Trace),
			  LogStrand(asio::make_strand(IOService))
		{}

		void LogService::construct(implementation_type& Implementation)
		{
			Implementation = std::make_shared<LoggerImplementation>(LogStrand);
		}

		void LogService::destroy(implementation_type& Implementation)
		{
			Implementation.reset();
		}

		void LogService::Shutdown()
		{
			FlushLogBuffer();
		}

		void LogService::SetLogLevel(LogLevel Level)
		{
			CurrentLogLevel = Level;
		}

		Modio::LogLevel LogService::GetLogLevel() const
		{
			return CurrentLogLevel;
		}

		void LogService::FlushLogBuffer()
		{
			if (UserCallbackFunction)
			{
				for (const LogMessage& Log : LogBuffer)
				{
					UserCallbackFunction(Log.Level, Log.Message);
				}
			}
			LogBuffer.clear();
		}

		void LogService::SetLogCallback(std::function<void(Modio::LogLevel, const std::string&)> LogCallback)
		{
			// Flush out the current logs
			if (UserCallbackFunction)
			{
				// @todo: What will happen here if there is no service alive as the global context is dead?
				Modio::Detail::Services::GetGlobalService<Modio::Detail::LogService>().FlushLogBuffer();
			}
			UserCallbackFunction = LogCallback;
		}

		void LogService::SetGlobalLogLevel(Modio::LogLevel Level)
		{
			auto& LogService =
				asio::use_service<Modio::Detail::LogService>(Modio::Detail::Services::GetGlobalContext());
			LogService.SetLogLevel(Level);
		}
#ifdef MODIO_SEPARATE_COMPILATION
		std::function<void(Modio::LogLevel, const std::string&)> LogService::UserCallbackFunction;
#endif
	} // namespace Detail
}

