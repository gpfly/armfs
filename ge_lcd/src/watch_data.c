/*************************************************************************
	> File Name: watch_data.c
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年10月18日 星期二 21时36分52秒
 ************************************************************************/

#include <stdio.h>
#include <ge_lcd.h>
#include <wangfan.h>
#include <wangled.h>
#include <ge_bjdj.h>
#include <wangred.h>
#include <wangirr.h>
#include <humidity.h>
#include <rain.h>
#include <smog.h>
#include <zhaoapp.h>


/*数字集合*/
extern unsigned char number_32x56[11][224];
extern unsigned char number_8x16[11][16];
extern unsigned char  number_16x33[11][66];
/*0123456789:*/
extern unsigned char  jiance_32x33[34][132];
/* 当前空气温度         0  6
 * 室内湿度             6  4
 * 土壤湿度             10 4
 * 降水量               14 3
 * co2含量              17 5
 * 通风                 22 2
 * 灯光                 24 2
 * 灌溉                 26 2
 * 卷帘                 28 2
 * 人体感应             30 4
 * */
extern unsigned char  openclose_32x33[5][132];
/*开关 */
extern unsigned char  rain_32x33[4][132];
/*有雨没雨*/

#define  numsize_16x33  16<<16|33     
#define  jiance_size    32<<16|33     
#define  col_y  0xffff00  
#define  col_r  0xff0000

int display_stat_ (char * pwin, struct s3c_fb_user_window * pos)
{
        int i;
        unsigned int ret;
        /*shineiwendu*/
        ret = zhaotemp();
        display_number (pwin, (const char *)number_16x33[(ret >> 24)%10] , 
                                numsize_16x33 , col_y, 300, 30, pos);
        display_number (pwin, (const char *)number_16x33[((ret >> 16) & 0xff)%10] , 
                                numsize_16x33 , col_y, 320, 30, pos);
        display_number (pwin, (const char *)openclose_32x33[3] , 
                                jiance_size , col_y, 340, 30, pos);


        /*shineishidu*/
        ret = zhaotemp();
        display_number (pwin, (const char *)number_16x33[((ret>>8) & 0xff)%10] , 
                                numsize_16x33 , col_y, 300, 70, pos);
        display_number (pwin, (const char *)number_16x33[(ret & 0xff)%10] , 
                                numsize_16x33 , col_y, 320, 70, pos);
        display_number (pwin, (const char *)openclose_32x33[4] , 
                                jiance_size , col_y, 340, 70, pos);

        /*turang*/
        ret = humidity();
        display_number (pwin, (const char *)number_16x33[ret >> 16] , 
                                numsize_16x33 , col_y, 300, 110, pos);
        display_number (pwin, (const char *)number_16x33[ret & 0xffff] , 
                                numsize_16x33 , col_y, 320, 110, pos);
        display_number (pwin, (const char *)openclose_32x33[4] , 
                                jiance_size , col_y, 340, 110, pos);

        /*rain*/
        if(rain() > 0) {
                for ( i = 0; i < 2; i++ ) {
                display_number (pwin, (const char *)rain_32x33[i] , 
                                jiance_size , col_y, 300+i*33, 150, pos);
                }
        } else if(rain() == 0) {
                for ( i = 2; i < 4; i++ ) {
                display_number (pwin, (const char *)rain_32x33[i] , 
                                jiance_size , col_y, 300+(i-2)*33, 150, pos);
                }
        }

        /*co2*/
        ret = smog();
        display_number (pwin, (const char *)number_16x33[(ret >> 16)%10] , 
                                numsize_16x33 , col_y, 300, 190, pos);
        display_number (pwin, (const char *)number_16x33[(ret & 0xffff)%10] , 
                                numsize_16x33 , col_y, 320, 190, pos);
        display_number (pwin, (const char *)openclose_32x33[4] , 
                                jiance_size , col_y, 340, 190, pos);

        /*tongfeng*/
        display_number (pwin, (const char *)openclose_32x33[1-fan_stat()] , 
                                jiance_size , col_y, 300, 230, pos);

        /*led*/
        display_number (pwin, (const char *)number_16x33[get_led_value()] , 
                                numsize_16x33 , col_y, 310, 270, pos);

        /*guangai*/
        display_number (pwin, (const char *)openclose_32x33[1-irr_stat()] , 
                                jiance_size , col_y, 300, 310, pos);

        /*juanlian*/
        display_number (pwin, (const char *)openclose_32x33[1-bjdj_stat()] , 
                                jiance_size , col_y, 300, 350, pos);

        /*hongwai*/
        display_number (pwin, (const char *)openclose_32x33[1-red_stat()] , 
                                jiance_size , col_y, 300, 390, pos);
                
        return 0;
}

int display_watch (char * pwin, struct s3c_fb_user_window * pos )
{
        int i,j;
        for (i = 0; i< 6; i++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (i+1)*33, 30, pos);
        }
        for (i = 6,j=0; i< 10; i++ ,j++) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 70, pos);
        }
        for (i = 10,j=0; i< 14; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 110, pos);
        }
        for (i = 14,j =0; i< 17; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 150, pos);
        }
        for (i = 17,j = 0; i< 22; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 190, pos);
        }
        for (i = 22,j = 0; i< 24; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 230, pos);
        }
        for (i = 24,j = 0; i< 26; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 270, pos);
        }
        for (i = 26,j = 0; i< 28; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 310, pos);
        }
        for (i = 28,j = 0; i< 30; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 350, pos);
        }
        for (i = 30,j = 0; i< 34; i++,j++ ) {
                display_number (pwin, (const char *)jiance_32x33[i] , 
                                jiance_size , col_y, (j+1)*33, 390, pos);
        }

        return 0;
}
