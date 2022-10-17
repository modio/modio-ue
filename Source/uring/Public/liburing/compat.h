/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_COMPAT_H
#define LIBURING_COMPAT_H

#include <endian.h>
#include <stdint.h>

struct __kernel_timespec
{
	int64_t tv_sec;
	long long tv_nsec;
};

#include <inttypes.h>

struct open_how
{
	uint64_t flags;
	uint64_t mode;
	uint64_t resolve;
};

#ifndef __kernel_rwf_t
typedef int __kernel_rwf_t;
#endif

#endif
