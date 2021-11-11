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

#include "ModioModCreationHandle.generated.h"

namespace Modio
{
	struct ModCreationHandle;
}

struct FModioModCreationHandle;

USTRUCT(BlueprintType)
struct MODIO_API FModioModCreationHandle
{
	GENERATED_BODY()

	private:
	int64 Underlying;
	friend FModioModCreationHandle ToUnreal(const Modio::ModCreationHandle& In);
	friend Modio::ModCreationHandle ToModio(const FModioModCreationHandle& In);
};
