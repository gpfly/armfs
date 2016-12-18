#ifndef _REGS_WDT_H
#define _REGS_WDT_H

//offset for register
#define WTCON		0x00
#define WTDAT		0x04
#define	WTCNT		0x08
#define WTCLRINT	0x0C

//offset for bits
#define WTCON_RSTEN   (1<<0)
#define WTCON_INTEN   (1<<2)
#define WTCON_ENABLE  (1<<5)

#define WTCON_DIV16   (0<<3)
#define WTCON_DIV32   (1<<3)
#define WTCON_DIV64   (2<<3)
#define WTCON_DIV128  (3<<3)

#define WTCON_PRESCALE(x) ((x) << 8)
#define WTCON_PRESCALE_MASK (0xff00)

#endif //_REGS_WDT_H
