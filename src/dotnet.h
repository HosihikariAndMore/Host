#ifndef PRELOADER_NETHOST_H
#define PRELOADER_NETHOST_H

#include "main.h"

extern load_assembly_fn load_assembly_fptr;
extern get_function_pointer_fn get_function_pointer_fptr;

extern hostfxr_initialize_for_runtime_config_fn init_fptr;
extern hostfxr_get_runtime_delegate_fn get_delegate_fptr;
extern hostfxr_close_fn close_fptr;

int load_hostfxr();

int init_delegate_fptrs(const char_t *);

#endif  // PRELOADER_NETHOST_H
