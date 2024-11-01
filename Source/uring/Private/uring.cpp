/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "uring.h"

DEFINE_LOG_CATEGORY(uring);

#define LOCTEXT_NAMESPACE "Furing"

void Furing::StartupModule()
{
	UE_LOG(uring, Display, TEXT("uring module has been loaded"));
}

void Furing::ShutdownModule()
{
	UE_LOG(uring, Display, TEXT("uring module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(Furing, uring)