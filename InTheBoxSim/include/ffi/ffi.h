#ifdef __arm__
  #include <arm/arch.h>
  #ifdef _ARM_ARCH_6
    #include "include/armv6/ffi.h"
  #elif _ARM_ARCH_7
    #include "include/armv7/ffi.h"
  #endif
#elif defined(__i386__)
  #include "include/i386/ffi.h"
#endif
