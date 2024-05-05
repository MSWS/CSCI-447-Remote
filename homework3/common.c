#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#define DEBUG
void debug(const char *msg, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
#endif
}
