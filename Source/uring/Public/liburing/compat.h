/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_COMPAT_H
#define LIBURING_COMPAT_H

#include <stdint.h>
#include <endian.h>

struct __kernel_timespec
    {
        int64_t tv_sec;
        long long tv_nsec;
    };

# include <inttypes.h>

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
