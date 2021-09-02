/**
 * hash.h - Hash table implementation for the routing algorithm.
 * Author: FallAngel
 * Date: 2021-08-16
 * 
 * Copyright (c) 2021
 */

#ifndef _RTABLE_H
#error "Do not use hash.h directly; include the rtable.h file instead."
#endif

#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdint.h>
#include <math.h>
#include <sys/ioctl.h>

#define A 0.1415F

static inline uint64_t mulhashing(uint64_t _k, uint64_t _m)
{
    return (uint64_t)(floor(fmod((_k*A), 1)*_m));
}

#endif /* _HASH_TABLE_H */
