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
#include "modio/compression/ModioCompressionService.h"
#endif


void Modio::Detail::CompressionService::construct(implementation_type& Implementation)
{
	PlatformImplementation->InitializeIOObjectImplementation(Implementation);
}

void Modio::Detail::CompressionService::move_construct(implementation_type& Implementation, implementation_type& Other)
{
	PlatformImplementation->MoveIOObjectImplementation(Implementation, Other);
}

void Modio::Detail::CompressionService::move_assign(implementation_type& Implementation,
													Modio::Detail::CompressionService& OtherService,
													implementation_type& Other)
{
	// No difference between this and move_construct for us because our application will only have a single
	// io_context and a single HttpService instance
	PlatformImplementation->MoveIOObjectImplementation(Implementation, Other);
}

void Modio::Detail::CompressionService::converting_move_construct(implementation_type& impl,
																  Modio::Detail::CompressionService&,
																  implementation_type& other_impl)
{
	move_construct(impl, other_impl);
}

void Modio::Detail::CompressionService::converting_move_assign(implementation_type& impl,
															   Modio::Detail::CompressionService& other_service,
															   implementation_type& other_impl)
{
	move_assign(impl, other_service, other_impl);
}
