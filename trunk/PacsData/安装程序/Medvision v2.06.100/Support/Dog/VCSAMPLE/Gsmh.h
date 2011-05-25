#include "resource.h"

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef HUINT
#define HUINT unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#define MHSTATUS ULONG

int (WINAPI *InstDriver)( int ntd, int verold);
