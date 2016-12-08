/*************************************************************************
	> File Name: ts.h
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年10月18日 星期二 15时53分08秒
 ************************************************************************/

#ifndef _TS_H
#define _TS_H
extern int init_ts(void);
extern int read_ts(void);
extern int check_area (struct win_icon * icon , struct s3c_fb_user_window * pos);
#endif
