#include <stdio.h>

#include "zmij.h"

int main(void) {
  char buf[ZMIJ_BUFFER_SIZE];
  zmij_dtoa(6.62607015e-34, buf);
  puts(buf);
  return 0;
}
