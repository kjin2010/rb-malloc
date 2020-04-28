#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "debug.h"

#include <unistd.h>
static char outbuf[2048];
void panic(const char* fmt, ...) {
   va_list args;
   va_start(args,fmt);
   int a = vsnprintf(outbuf, 2048,fmt,args);
   int mina = 2048 < a ? 2048 : a;
   int ret = write(1,outbuf,mina);
   exit(1);
}