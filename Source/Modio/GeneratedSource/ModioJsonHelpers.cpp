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
#include "modio/detail/ModioJsonHelpers.h"
#endif

#include "modio/core/ModioBuffer.h"

bool Modio::Detail::GetSubobjectSafe(const nlohmann::json& Json, const std::string& SubobjectKey,
									 nlohmann::json& OutSubobjectJson)
{
	if (Json.contains(SubobjectKey))
	{
		const nlohmann::json& Subobject = Json.at(SubobjectKey);
		if (!Subobject.is_null())
		{
			OutSubobjectJson = Subobject;
			return true;
		}
		else
		{
			Modio::Detail::Logger().Log(Modio::LogLevel::Warning, Modio::LogCategory::Core,
										"Subobject is null for SubobjectKey: {}", SubobjectKey);
		}
	}
	else
	{
		Modio::Detail::Logger().Log(Modio::LogLevel::Warning, Modio::LogCategory::Core,
									"Json does not contain SubobjectKey: {}", SubobjectKey);
	}
	return false;
}

bool Modio::Detail::ParseArraySizeSafe(const nlohmann::json& Json, std::size_t& OutVar, const std::string& Key)
{
	if (Json.contains(Key) && !Json.at(Key).is_null())
	{
		OutVar = Json.at(Key).size();
		return true;
	}
	else
	{
		Modio::Detail::Logger().Log(Modio::LogLevel::Warning, Modio::LogCategory::Core,
									"Json ParseArraySizeSafe failed for Key: {}", Key);
		return false;
	}
}

nlohmann::json Modio::Detail::ToJson(const Modio::Detail::DynamicBuffer& ResponseBuffer)
{
	Modio::Detail::Buffer LinearBuffer(ResponseBuffer.size());
	Modio::Detail::BufferCopy(LinearBuffer, ResponseBuffer);

	return nlohmann::json::parse(LinearBuffer.Data(), nullptr, false);
}

nlohmann::json Modio::Detail::ToJson(const Modio::filesystem::path& Path)
{
	return nlohmann::json::parse(Path.string(), nullptr, false);
}

nlohmann::json Modio::Detail::ToJson(const Modio::Detail::Buffer& InBuffer)
{
	return nlohmann::json::parse(InBuffer.Data(), nullptr, false);
}
