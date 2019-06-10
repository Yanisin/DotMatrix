#pragma once

#include <stdarg.h>
#include <stddef.h>

int small_snprintf(char* buffer, size_t count, const char* format, ...);
int small_vsnprintf(char* buffer, size_t count, const char* format, va_list va);
int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);
int vfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va);
