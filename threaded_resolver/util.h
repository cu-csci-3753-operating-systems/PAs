#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define UTIL_FAILURE -1
#define UTIL_SUCCESS 0

/**
 * @brief Resolve hostname to IPv4 address.
 * Returns -1 on failure, 0 on success.
 * Stores resulting IP address in firstIPstr.
 * maxSize is size of IP address.
 *
 * @param hostname
 * @param firstIPstr
 * @param maxSize
 * @return int
 */
int dnslookup(const char *hostname, char *firstIPstr, int maxSize);
