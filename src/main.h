#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include "irc_types.h"

char* read_str(int fd);
int write_str(int fd, char* str);
char* alloc_ptr(char* string);
int read_single(int fd, char *buf);

#endif