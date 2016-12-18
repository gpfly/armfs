#ifndef _IRQ_H_
#define _IRQ_H_

#include <common.h>

#define IRQMAX  160
#define ICCBASE 0x10480000
#define ICDBASE 0x10490000

#define ICCICR_CPU0 	(*(u32 *)(ICCBASE + 0x0000)) 
#define ICCPMR_CPU0 	(*(u32 *)(ICCBASE + 0x0004)) 

#define ICDDCR 			(*(u32 *)(ICDBASE + 0x0000)) 

#define ICDISER0_CPU0 	((u32 *)(ICDBASE + 0x0100))
#define ICDIPTR0_CPU0 	((u32 *)(ICDBASE + 0x0800)) 
#define ICDIPR0_CPU0 	((u32 *)(ICDBASE + 0x0400))

#define ICCIAR_CPU0 	(*(u32 *)(ICCBASE + 0x000C)) 
#define ICCEOIR_CPU0 	(*(u32 *)(ICCBASE + 0x0010)) 

#define ICDSGIR 		(*(u32 *)(ICDBASE + 0x0F00)) 

typedef void (*isr_t)(void);
extern void irq_request_cpu0_(int irqnum, void (*handler)(void), int pri);

#endif
