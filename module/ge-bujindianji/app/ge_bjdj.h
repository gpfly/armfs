#ifndef MILLET_BJDJ_H_
#define MILLET_BJDJ_H_

#include <linux/ioctl.h>

#define BJDJTYPE	'B'

#define BJDJ_ZHENG  	_IOW(BJDJTYPE, 0, int)
#define BJDJ_FAN 	_IOW(BJDJTYPE, 1, int)
#define BJDJ_RATE	_IOW(BJDJTYPE, 2, int)

#endif
