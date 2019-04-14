#ifndef HEADERS_H
#define HEADERS_H

/* General part */
#include <errno.h>
#include <iostream>

/* Server part */
#include <ev++.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

/* Log part */
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

typedef int type_socket;

#endif // HEADERS_H
