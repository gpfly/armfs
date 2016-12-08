#ifndef MILLET_BJDJ_H_
#define MILLET_BJDJ_H_

#include <linux/ioctl.h>

#define BJDJTYPE	'B'

#define BJDJ_ZHENG  	_IOW(BJDJTYPE, 0, int)
#define BJDJ_FAN 	_IOW(BJDJTYPE, 1, int)
#define BJDJ_RATE	_IOW(BJDJTYPE, 2, int)

extern int bjdj_stat(void);
extern int bjdj_zheng(void);
extern int bjdj_fan(void);
#endif
