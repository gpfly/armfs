#ifndef _IRQ_H_
#define _IRQ_H_


#define ICCBASE 0x10480000
#define ICDBASE 0x10490000

#define ICCICR_CPU0 	*((volatile unsigned int *)(ICCBASE + 0x0000)) 
#define ICCPMR_CPU0 	*((volatile unsigned int *)(ICCBASE + 0x0004)) 
#define ICCIAR_CPU0 	*((volatile unsigned int *)(ICCBASE + 0x000C)) 
#define ICCEOIR_CPU0 	*((volatile unsigned int *)(ICCBASE + 0x0010)) 

#define ICDISER0_CPU0 	((volatile unsigned int *)(ICDBASE + 0x0100)) 
#define ICDDCR 			*((volatile unsigned int *)(ICDBASE + 0x0000)) 
#define ICDIPR0_CPU0 	((volatile unsigned int *)(ICDBASE + 0x0400)) 
#define ICDIPTR0_CPU0 	((volatile unsigned int *)(ICDBASE + 0x0800)) 
#define ICDSGIR 		*((volatile unsigned int *)(ICDBASE + 0x0F00)) 

#define WDTBASE 0x10060000

#define WTCON 		*((volatile unsigned int *)(WDTBASE + 0x0000)) 
#define WTDAT 		*((volatile unsigned int *)(WDTBASE + 0x0004)) 
#define WTCNT 		*((volatile unsigned int *)(WDTBASE + 0x0008)) 
#define WTCLRINT	*((volatile unsigned int *)(WDTBASE + 0x000C)) 

#endif
