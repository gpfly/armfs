#ifndef MILLET_BEEP_H_
#define MILLET_BEEP_H_

#include <linux/ioctl.h>

#define BEEPTYPE	'b'

#define BEEP_SET_FREQ  	_IOW(BEEPTYPE, 0, int)
#define BEEP_STOP		_IO(BEEPTYPE, 1)

#endif
