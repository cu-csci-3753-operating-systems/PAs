#include "multi-lookup.h"
#include "array.h"
#include "util.h"

int main() {
  // sample hostname
  char *hostname = "google.com";

  // buffer to store result of dnslookup
  char ipv4[INET_ADDRSTRLEN];

  // resolve hostname to ip address
  if (!dnslookup(hostname, ipv4, INET_ADDRSTRLEN)) {
    printf("%s -> %s\n", hostname, ipv4);
  }
  return 0;
}
