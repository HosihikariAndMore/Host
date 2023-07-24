#ifndef PRELOADER_MAIN_H
#define PRELOADER_MAIN_H

#include <nethost/coreclr_delegates.h>
#include <nethost/hostfxr.h>
#include <nethost/nethost.h>

typedef int (*__libc_start_main_type)(int (*)(int, char **, char **), int,
                                      char **, void (*)(), void (*)(),
                                      void (*)(), void *);

int __libc_start_main(int (*)(int, char **, char **), int, char **,
                      void (*init)(), void (*)(), void (*)(), void *);

void init_loader();

#endif  // PRELOADER_MAIN_H
