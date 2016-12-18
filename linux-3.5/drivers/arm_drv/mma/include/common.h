#ifndef _COMMON_H_
#define _COMMON_H_

typedef volatile unsigned long u32;
typedef volatile unsigned short u16;
typedef volatile unsigned char u8; 

#define printf(...) (((int (*)(const char *, ...))0x43e11a2c)(__VA_ARGS__))

#define NULL (void *)0

#endif
