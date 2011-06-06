/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_CUTILS_ATOMIC_X86_H
#define ANDROID_CUTILS_ATOMIC_X86_H

#include <stdint.h>

#ifdef __GNUC_GNU_INLINE__
#define FC_INLINE extern inline
#else
#define FC_INLINE inline
#endif

FC_INLINE void android_compiler_barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

#if ANDROID_SMP == 0
FC_INLINE void android_memory_barrier(void)
{
    android_compiler_barrier();
}
#else
FC_INLINE void android_memory_barrier(void)
{
    __asm__ __volatile__ ("mfence" : : : "memory");
}
#endif

FC_INLINE int32_t android_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    android_compiler_barrier();
    return value;
}

FC_INLINE int32_t android_atomic_release_load(volatile const int32_t *ptr)
{
    android_memory_barrier();
    return *ptr;
}

FC_INLINE void android_atomic_acquire_store(int32_t value,
                                                volatile int32_t *ptr)
{
    *ptr = value;
    android_memory_barrier();
}

FC_INLINE void android_atomic_release_store(int32_t value,
                                                volatile int32_t *ptr)
{
    android_compiler_barrier();
    *ptr = value;
}

FC_INLINE int android_atomic_cas(int32_t old_value, int32_t new_value,
                                     volatile int32_t *ptr)
{
    int32_t prev;
    __asm__ __volatile__ ("lock; cmpxchgl %1, %2"
                          : "=a" (prev)
                          : "q" (new_value), "m" (*ptr), "0" (old_value)
                          : "memory");
    return prev != old_value;
}

FC_INLINE int android_atomic_acquire_cas(int32_t old_value,
                                             int32_t new_value,
                                             volatile int32_t *ptr)
{
    /* Loads are not reordered with other loads. */
    return android_atomic_cas(old_value, new_value, ptr);
}

FC_INLINE int android_atomic_release_cas(int32_t old_value,
                                             int32_t new_value,
                                             volatile int32_t *ptr)
{
    /* Stores are not reordered with other stores. */
    return android_atomic_cas(old_value, new_value, ptr);
}

FC_INLINE int32_t android_atomic_swap(int32_t new_value,
                                          volatile int32_t *ptr)
{
    __asm__ __volatile__ ("xchgl %1, %0"
                          : "=r" (new_value)
                          : "m" (*ptr), "0" (new_value)
                          : "memory");
    /* new_value now holds the old value of *ptr */
    return new_value;
}

FC_INLINE int32_t android_atomic_add(int32_t increment,
                                         volatile int32_t *ptr)
{
    __asm__ __volatile__ ("lock; xaddl %0, %1"
                          : "+r" (increment), "+m" (*ptr)
                          : : "memory");
    /* increment now holds the old value of *ptr */
    return increment;
}

FC_INLINE int32_t android_atomic_inc(volatile int32_t *addr)
{
    return android_atomic_add(1, addr);
}

FC_INLINE int32_t android_atomic_dec(volatile int32_t *addr)
{
    return android_atomic_add(-1, addr);
}

FC_INLINE int32_t android_atomic_and(int32_t value,
                                         volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev & value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

FC_INLINE int32_t android_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev | value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}

#endif /* ANDROID_CUTILS_ATOMIC_X86_H */
