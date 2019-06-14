#pragma once

#include <stdarg.h>
#include <stddef.h>

/**
 * @file
 *
 * Small printf is a simpler printf implementation of the usual printf variants.
 * You can use the usual modifiers, (%d, %x), alignments etc. Floating point and long long
 * is not supported.
 */

int small_snprintf(char* buffer, size_t count, const char* format, ...);
int small_vsnprintf(char* buffer, size_t count, const char* format, va_list va);
/**
 * printf with a callback function called for each byte writen.
 */
int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);
int vfctprintf(void (*out)(char character, void* arg), void* arg, const char* format, va_list va);
