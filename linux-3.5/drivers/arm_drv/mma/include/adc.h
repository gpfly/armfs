#ifndef _ADC_H_
#define _ADC_H_

#define ADCBASE 0x126C0000

#define ADCCON 		*((volatile unsigned int *)(ADCBASE+0x0000)) 
#define ADCDLY 		*((volatile unsigned int *)(ADCBASE+0x0008))
#define ADCDAT 		*((volatile unsigned int *)(ADCBASE+0x000C)) 
#define CLRINTADC 	*((volatile unsigned int *)(ADCBASE+0x0018)) 
#define ADCMUX 		*((volatile unsigned int *)(ADCBASE+0x001C)) 

#define ADC_CFG		*((volatile unsigned int *)0x10010118)

#define WAIT_CVT_END 		while(!(ADCCON & (1 << 15)))
#define START_READ_CVT		(ADCCON |= 1 << 1)
#define START_ENABLE_CVT 	(ADCCON |= 1)

#define IESR2 		*((volatile unsigned int *)0x10440020) 
#define ISTR2 		*((volatile unsigned int *)0x10440028) 

extern unsigned short read_adc(void);
extern void adc_init(void);
extern void do_adc(void);

#endif
