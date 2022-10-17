/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "uring.h"

DEFINE_LOG_CATEGORY(uring);

#define LOCTEXT_NAMESPACE "Furing"

void Furing::StartupModule()
{
	UE_LOG(uring, Warning, TEXT("uring module has been loaded"));
}

void Furing::ShutdownModule()
{
	UE_LOG(uring, Warning, TEXT("uring module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(Furing, uring)