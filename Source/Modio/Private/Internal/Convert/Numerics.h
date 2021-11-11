/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once
#include <cstdint>

FORCEINLINE std::uint8_t ToModio(const uint8& In)
{
	return static_cast<std::uint8_t>(In);
}

FORCEINLINE uint8 ToUnreal(const std::uint8_t& In)
{
    return static_cast<uint8>(In); 
}