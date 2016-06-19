#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include "irc_types.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

int connect_irc();
char* read_str(int fd);
int write_str(int fd, char* str);
char* alloc_ptr(char* string);
int read_single(int fd, char *buf);
int get_random(int min, int max);
SSL* ssl_init();

#endif