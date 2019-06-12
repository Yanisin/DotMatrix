#pragma once

typedef void (*initializer_cb)(void);

#define init_add(name) \
	const initializer_cb reg_ ## name \
	__attribute__((section (".dminit."#name))) = &name

void run_all_initializers(void);
