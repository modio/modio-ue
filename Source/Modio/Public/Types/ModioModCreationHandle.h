/* 
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io UE Plugin.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *   
 */

#pragma once

#include "ModioModCreationHandle.generated.h"

namespace Modio
{
	struct ModCreationHandle;
}

struct FModioModCreationHandle;

/**
 * @docpublic
 * @brief Struct type to store the handle when a new mod is created
 */
USTRUCT(BlueprintType)
struct MODIO_API FModioModCreationHandle
{
	GENERATED_BODY()

private:
	int64 Underlying;
	friend FModioModCreationHandle ToUnreal(const Modio::ModCreationHandle& In);
	friend Modio::ModCreationHandle ToModio(const FModioModCreationHandle& In);
};
