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
#include "common/HttpSharedState.h"
#endif


 HttpSharedStateBase::HttpSharedStateBase(HINTERNET SessionHandle) : CurrentSession(SessionHandle) {}

void HttpSharedStateBase::InitializeRequest(std::shared_ptr<HttpRequestImplementation> Request, Modio::ErrorCode& ec)
{
	Modio::Detail::Logger().Log(Modio::LogLevel::Info, Modio::LogCategory::Http, "Initializing {0} request for {1} {2}", Request->GetParameters().GetVerb(),
			   Request->GetParameters().GetServerAddress(), Request->GetParameters().GetFormattedResourcePath());
	//"https://api.test.mod.io"
	// WinHttpConnect always blocks so no need to spin this off as an async operation
	auto ConnectionHandle =
		WinHttpConnect(CurrentSession, UTF8ToWideChar(Request->GetParameters().GetServerAddress()).c_str(),
					   INTERNET_DEFAULT_HTTPS_PORT, 0);
	if (ConnectionHandle == nullptr)
	{
		auto ConnectionStatus = GetLastError();
		if (ConnectionStatus == ERROR_WINHTTP_OPERATION_CANCELLED || ConnectionStatus == ERROR_WINHTTP_SHUTDOWN)
		{
			ec = Modio::make_error_code(Modio::GenericError::OperationCanceled);
			return;
		}
		else
		{
			ec = Modio::make_error_code(Modio::HttpError::CannotOpenConnection);
			return;
		}
	}

	// TODO: @ModioGDK specify accept types properly
	// constexpr auto AcceptJsonType = L"application/json";
	auto RequestHandle =
		WinHttpOpenRequest(ConnectionHandle, UTF8ToWideChar(Request->GetParameters().GetVerb()).c_str(),
						   UTF8ToWideChar(Request->GetParameters().GetFormattedResourcePath()).c_str(), nullptr,
						   WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

	bool Success = WinHttpSetOption(RequestHandle, WINHTTP_OPTION_CONTEXT_VALUE, this, sizeof(uintptr_t));
	if (RequestHandle == nullptr)
	{
		ec = Modio::make_error_code(Modio::HttpError::CannotOpenConnection);
		return;
	}
	else
	{
		ConnectionHandles[RequestHandle] = ConnectionHandle;
		CallbackStatus[RequestHandle] = WinHTTPCallbackStatus::Waiting;
		Request->ConnectionHandle = ConnectionHandle;
		Request->RequestHandle = RequestHandle;
		ec = {};
		return;
	}
}

void HttpSharedStateBase::SetHandleStatus(HINTERNET Handle, WinHTTPCallbackStatus Status, void* ExtendedStatusData,
										  unsigned long ExtendedStatusLength)
{
	ExtendedStatus[Handle] = std::make_pair((std::uintptr_t) ExtendedStatusData, ExtendedStatusLength);
	CallbackStatus[Handle].exchange(Status);
}

WinHTTPCallbackStatus HttpSharedStateBase::PeekHandleStatus(HINTERNET Handle)
{
	return CallbackStatus[Handle].load();
}

WinHTTPCallbackStatus HttpSharedStateBase::FetchAndClearHandleStatus(HINTERNET Handle)
{
	return CallbackStatus[Handle].exchange(WinHTTPCallbackStatus::Waiting);
}

std::pair<std::uintptr_t, std::uintmax_t> HttpSharedStateBase::GetExtendedStatus(HINTERNET Handle)
{
	auto StatusValues = ExtendedStatus[Handle];
	ExtendedStatus[Handle] = std::make_pair(0, 0);
	return StatusValues;
}

bool HttpSharedStateBase::IsClosing()
{
	return CurrentServiceState == HttpServiceState::Closing;
}

void HttpSharedStateBase::Close()
 {
	 CurrentServiceState = HttpServiceState::Closing;
 }
