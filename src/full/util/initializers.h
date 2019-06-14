#pragma once

typedef void (*initializer_cb)(void);

/**
 * Register a function as an initializer that will be run during
 * full demo startup. Initializers are run before topology detection.
 */
#define init_add(name) \
	const initializer_cb reg_ ## name \
	__attribute__((section (".dminit."#name))) = &name

void run_all_initializers(void);
