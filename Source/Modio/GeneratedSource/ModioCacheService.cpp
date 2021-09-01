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
	#include "modio/cache/ModioCacheService.h"
#endif

#include "modio/core/ModioLogger.h"
#include "modio/core/ModioLogEnum.h"
#include "modio/core/ModioBuffer.h"

namespace Modio
{
	namespace Detail
	{
		CacheService::CacheService(asio::io_context& IOService)
			: asio::detail::service_base<CacheService>(IOService)
		{
			CacheInstance = std::make_shared<Cache>();
		}

		void CacheService::Shutdown()
		{
			// Cancel all timers
			for(auto& CacheEntry : CacheInstance->CacheEntries)
			{
				CacheEntry.second.Timer->cancel();	
			}
			ClearCache();
		}

		void CacheService::construct(implementation_type& Implementation) {}

		void CacheService::destroy(implementation_type& Implementation) {}

		void CacheService::SetCacheExpireTime(std::chrono::steady_clock::duration ExpireTime)
		{
			CacheExpiryTime = ExpireTime;
		}

		void CacheService::AddToCache(std::string ResourceURL, Modio::Detail::DynamicBuffer ResponseData)
		{
			auto Hasher = std::hash<std::string>();
			std::uint32_t URLHash = (std::uint32_t) Hasher(ResourceURL);

			// Don't add the instance again as if the instance is already in there, then we will destroy the old timer and it will expire causing unnecessary removals
			if(CacheInstance->CacheEntries.find(URLHash) == std::end(CacheInstance->CacheEntries))
			{
				// @todo-optimize This will fragment the heap quite much, rewrite using another container so we don't need to allocate so many objects on the heap
				std::unique_ptr<asio::steady_timer> CacheExpiryTimer = std::make_unique<asio::steady_timer>(get_io_context());
				CacheExpiryTimer->expires_after(CacheExpiryTime);

				Modio::Detail::Logger().Log(LogLevel::Info, LogCategory::Http, "Adding hash {} to cache", URLHash);

				auto DeleteCacheEntry = [WeakCacheReference = std::weak_ptr<Cache>(CacheInstance), URLHash](std::error_code) {
					std::shared_ptr<Cache> CacheReference = WeakCacheReference.lock();
					Modio::Detail::Logger().Log(LogLevel::Info, LogCategory::Http, "Removing hash {} from cache", URLHash);
					if (CacheReference)
					{
						CacheReference->CacheEntries.erase(URLHash);
					}
				};

				CacheExpiryTimer->async_wait(DeleteCacheEntry);

				CacheEntry Entry = { std::move(CacheExpiryTimer), std::move(ResponseData) };
				CacheInstance->CacheEntries.emplace(URLHash, std::move(Entry) );
			}
		}

		Modio::Optional<Modio::Detail::DynamicBuffer> CacheService::FetchFromCache(std::string ResourceURL) const
		{
			auto Hasher = std::hash<std::string>();
			std::uint32_t URLHash = (std::uint32_t) Hasher(ResourceURL);

			auto CacheEntryIterator = CacheInstance->CacheEntries.find(URLHash);
			if (CacheEntryIterator != CacheInstance->CacheEntries.end())
			{
				return (CacheEntryIterator)->second.Data;
			}
			else
			{
				return {};
			}
		}

		void CacheService::ClearCache()
		{
			CacheInstance.reset(new Cache());
		}
	} // namespace Detail
} // namespace Modio
