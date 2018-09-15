#pragma once
#include <inttypes.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



// #define EXTERN_VREAL(func) extern void *Real_##func;

#if defined(CPU64)
#define PRINTF_UPTR(prefix, value) printf(prefix ": %#018" PRIx64 "\n", value);
#define EXTERN_REAL(func) extern typeof(func) *Real_##func;
#elif defined(CPU32)
#define PRINTF_UPTR(prefix, value) printf(prefix ": %#010" PRIx32 "\n", value);
#define EXTERN_REAL(func) extern typeof(func) *Real_##func asm("Real_"#func);
#else
#error Unknown CPU
#endif

