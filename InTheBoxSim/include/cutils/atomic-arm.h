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

#ifndef ANDROID_CUTILS_ATOMIC_ARM_H
#define ANDROID_CUTILS_ATOMIC_ARM_H

#include <stdint.h>
#include <machine/cpu-features.h>
// FlexyCore Edit: replaced every "extern inline" by "FC_INLINE"

// Begin FlexyCore
#ifdef __GNUC_GNU_INLINE__
#define FC_INLINE extern inline
#else
#define FC_INLINE inline
#endif
// End FlexyCore

FC_INLINE void android_compiler_barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

#if ANDROID_SMP == 0
FC_INLINE void android_memory_barrier(void)
{
  android_compiler_barrier();
}
#elif defined(__ARM_HAVE_DMB)
FC_INLINE void android_memory_barrier(void)
{
    __asm__ __volatile__ ("dmb" : : : "memory");
}
#elif defined(__ARM_HAVE_LDREX_STREX)
FC_INLINE void android_memory_barrier(void)
{
    __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5"
                          : : "r" (0) : "memory");
}
#else
FC_INLINE void android_memory_barrier(void)
{
    typedef void (kuser_memory_barrier)(void);
    (*(kuser_memory_barrier *)0xffff0fa0)();
}
#endif

FC_INLINE int32_t android_atomic_acquire_load(volatile const int32_t *ptr)
{
    int32_t value = *ptr;
    android_memory_barrier();
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
    android_memory_barrier();
    *ptr = value;
}

// Begin FlexyCore
//#if defined(__thumb__)
//extern int android_atomic_cas(int32_t old_value, int32_t new_value,
//                              volatile int32_t *ptr);
//#elif defined(__ARM_HAVE_LDREX_STREX)
#if defined(__ARM_HAVE_LDREX_STREX)
// End FlexyCore
FC_INLINE int android_atomic_cas(int32_t old_value, int32_t new_value,
                                     volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        __asm__ __volatile__ ("ldrex %0, [%3]\n"
                              "mov %1, #0\n"
                              "teq %0, %4\n"
                              "strexeq %1, %5, [%3]"
                              : "=&r" (prev), "=&r" (status), "+m"(*ptr)
                              : "r" (ptr), "Ir" (old_value), "r" (new_value)
                              : "cc");
    } while (__builtin_expect(status != 0, 0));
    return prev != old_value;
}
#else
FC_INLINE int android_atomic_cas(int32_t old_value, int32_t new_value,
                                     volatile int32_t *ptr)
{
    typedef int (kuser_cmpxchg)(int32_t, int32_t, volatile int32_t *);
    int32_t prev, status;
    prev = *ptr;
    do {
        status = (*(kuser_cmpxchg *)0xffff0fc0)(old_value, new_value, ptr);
        if (__builtin_expect(status == 0, 1))
            return 0;
        prev = *ptr;
    } while (prev == old_value);
    return 1;
}
#endif

FC_INLINE int android_atomic_acquire_cas(int32_t old_value,
                                             int32_t new_value,
                                             volatile int32_t *ptr)
{
    int status = android_atomic_cas(old_value, new_value, ptr);
    android_memory_barrier();
    return status;
}

FC_INLINE int android_atomic_release_cas(int32_t old_value,
                                             int32_t new_value,
                                             volatile int32_t *ptr)
{
    android_memory_barrier();
    return android_atomic_cas(old_value, new_value, ptr);
}


#if defined(__thumb__)
extern int32_t android_atomic_swap(int32_t new_value,
                                   volatile int32_t *ptr);
#elif defined(__ARM_HAVE_LDREX_STREX)
FC_INLINE int32_t android_atomic_swap(int32_t new_value,
                                          volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        __asm__ __volatile__ ("ldrex %0, [%3]\n"
                              "strex %1, %4, [%3]"
                              : "=&r" (prev), "=&r" (status), "+m" (*ptr)
                              : "r" (ptr), "r" (new_value)
                              : "cc");
    } while (__builtin_expect(status != 0, 0));
    android_memory_barrier();
    return prev;
}
#else
FC_INLINE int32_t android_atomic_swap(int32_t new_value,
                                          volatile int32_t *ptr)
{
    int32_t prev;
    __asm__ __volatile__ ("swp %0, %2, [%3]"
                          : "=&r" (prev), "+m" (*ptr)
                          : "r" (new_value), "r" (ptr)
                          : "cc");
    android_memory_barrier();
    return prev;
}
#endif

// Begin FlexyCore
//#if defined(__thumb__)
//extern int32_t android_atomic_add(int32_t increment,
//                                  volatile int32_t *ptr);
//#elif defined(__ARM_HAVE_LDREX_STREX)
#if defined(__ARM_HAVE_LDREX_STREX)
// End FlexyCore
FC_INLINE int32_t android_atomic_add(int32_t increment,
                                         volatile int32_t *ptr)
{
    int32_t prev, tmp, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ ("ldrex %0, [%4]\n"
                              "add %1, %0, %5\n"
                              "strex %2, %1, [%4]"
                              : "=&r" (prev), "=&r" (tmp),
                                "=&r" (status), "+m" (*ptr)
                              : "r" (ptr), "Ir" (increment)
                              : "cc");
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#else
FC_INLINE int32_t android_atomic_add(int32_t increment,
                                         volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev + increment, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#endif

FC_INLINE int32_t android_atomic_inc(volatile int32_t *addr)
{
    return android_atomic_add(1, addr);
}

FC_INLINE int32_t android_atomic_dec(volatile int32_t *addr)
{
    return android_atomic_add(-1, addr);
}

#if defined(__thumb__)
extern int32_t android_atomic_and(int32_t value, volatile int32_t *ptr);
#elif defined(__ARM_HAVE_LDREX_STREX)
FC_INLINE int32_t android_atomic_and(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, tmp, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ ("ldrex %0, [%4]\n"
                              "and %1, %0, %5\n"
                              "strex %2, %1, [%4]"
                              : "=&r" (prev), "=&r" (tmp),
                                "=&r" (status), "+m" (*ptr)
                              : "r" (ptr), "Ir" (value)
                              : "cc");
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#else
FC_INLINE int32_t android_atomic_and(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev & value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#endif

#if defined(__thumb__)
extern int32_t android_atomic_or(int32_t value, volatile int32_t *ptr);
#elif defined(__ARM_HAVE_LDREX_STREX)
FC_INLINE int32_t android_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, tmp, status;
    android_memory_barrier();
    do {
        __asm__ __volatile__ ("ldrex %0, [%4]\n"
                              "orr %1, %0, %5\n"
                              "strex %2, %1, [%4]"
                              : "=&r" (prev), "=&r" (tmp),
                                "=&r" (status), "+m" (*ptr)
                              : "r" (ptr), "Ir" (value)
                              : "cc");
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#else
FC_INLINE int32_t android_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    android_memory_barrier();
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev | value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
#endif

#endif /* ANDROID_CUTILS_ATOMIC_ARM_H */
