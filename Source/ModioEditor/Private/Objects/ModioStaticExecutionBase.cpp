/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Objects/ModioStaticExecutionBase.h"

UModioStaticExecutionBase::UModioStaticExecutionBase()
	: Super()
{}

bool UModioStaticExecutionBase::ExecuteAction(FString Args) const
{
	UE_LOG(LogTemp, Warning, TEXT("You executed a static executor, but the base class was called. "
		"Double check that your overriding class exists and has overriden ExecuteAction()"));
	return false;
}