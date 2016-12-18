#ifndef MILLET_LED_H_
#define MILLET_LED_H_

#include <linux/ioctl.h>

#define LEDTYPE	'L'

#define LED_ON  	_IOW(LEDTYPE, 0, int)
#define LED_OFF 	_IOW(LEDTYPE, 1, int)
#define LED_ON_ALL	_IO(LEDTYPE, 2)
#define LED_OFF_ALL	_IO(LEDTYPE, 3)
#define LED_GET_STAT	_IOR(LEDTYPE, 4, int)

#endif
