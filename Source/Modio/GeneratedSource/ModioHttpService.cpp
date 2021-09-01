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
#include "modio/http/ModioHttpService.h"
#endif

namespace Modio
{
	namespace Detail
	{
		HttpService::HttpService(asio::io_context& IOService) : asio::detail::service_base<HttpService>(IOService)
		{
			APIQueue = std::make_shared<Modio::Detail::OperationQueue>(IOService);
			FileDownloadQueue = std::make_shared<Modio::Detail::OperationQueue>(IOService);

			auto NewImplementation = std::make_shared<HttpImplementation>(*this);

			PlatformImplementation.swap(NewImplementation);
		}

		void HttpService::construct(implementation_type& Implementation)
		{
			PlatformImplementation->InitializeIOObjectImplementation(Implementation);
		}

		void HttpService::move_construct(implementation_type& Implementation, implementation_type& Other)
		{
			PlatformImplementation->MoveIOObjectImplementation(Implementation, Other);
		}

		void HttpService::move_assign(implementation_type& Implementation, Modio::Detail::HttpService& OtherService,
									  implementation_type& Other)
		{
			// No difference between this and move_construct for us because our application will only have a single
			// io_context and a single HttpService instance
			PlatformImplementation->MoveIOObjectImplementation(Implementation, Other);
		}

		void HttpService::converting_move_construct(implementation_type& impl, Modio::Detail::HttpService&,
													implementation_type& other_impl)
		{
			move_construct(impl, other_impl);
		}

		void HttpService::converting_move_assign(implementation_type& impl, Modio::Detail::HttpService& other_service,
												 implementation_type& other_impl)
		{
			move_assign(impl, other_service, other_impl);
		}

		void HttpService::destroy(implementation_type& Implementation)
		{
		}

		Modio::Detail::OperationQueue::Ticket HttpService::GetAPIRequestTicket()
		{
			return APIQueue->GetTicket();
		}

		Modio::Detail::OperationQueue::Ticket HttpService::GetFileDownloadTicket()
		{
			return FileDownloadQueue->GetTicket();
		}

		void HttpService::Shutdown()
		{
			PlatformImplementation->Shutdown();
			APIQueue->CancelAll();
			FileDownloadQueue->CancelAll();
		}

		void HttpService::shutdown_service() {}

	} // namespace Detail
} // namespace Modio
