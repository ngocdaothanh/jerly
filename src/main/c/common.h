#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>

//#define DEBUG
#ifdef DEBUG

#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define dprintf(_fmt, args...)  {DEBUGPRINT2(WHERESTR _fmt, WHEREARG, ## args); fflush(stderr);}

#else

#define dprintf(format, args...)

#endif

#endif