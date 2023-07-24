#ifndef PRELOADER_NETHOST_H
#define PRELOADER_NETHOST_H

#include <main.h>

load_assembly_and_get_function_pointer_fn
    load_assembly_and_get_function_pointer_fptr;
// get_function_pointer_fn get_function_pointer_fptr;
load_assembly_fn load_assembly_fptr;
// load_assembly_bytes_fn load_assembly_bytes_fptr;

hostfxr_initialize_for_runtime_config_fn init_fptr;
hostfxr_get_runtime_delegate_fn get_delegate_fptr;
hostfxr_close_fn close_fptr;

int load_hostfxr();

int init_delegate_fptrs(const char_t *);

#endif  // PRELOADER_NETHOST_H
