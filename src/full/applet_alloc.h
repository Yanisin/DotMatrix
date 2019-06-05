#ifndef APPLET_ALLOC_H
#define APPLET_ALLOC_H

#include <stdint.h>
#include <stddef.h>

void *malloc_applet(size_t size);
void free_applet(void);

#endif // APPLET_ALLOC_H
