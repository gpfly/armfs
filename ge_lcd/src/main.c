#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ge_lcd.h>
#include <ge_list.h>
#include <fb.h>
#include <ts.h>
#include <watch_data.h>
#include <pthread.h>
#include <ge_bjdj.h>
#include <wangfan.h>
#include <wangirr.h>
#include <wangled.h>
#include <wangred.h>
#include <beep.h>
#include <humidity.h>
#include <rain.h>
#include <smog.h>


struct s3c_fb_user_window position[] = {
        [0] = {
                .x = 0,
                .y = 0,
        },
        [1] = {
                .x = 0,
                .y = 0,
        }
};

/*关键色*/
struct s3c_fb_user_chroma color[] = {
        [1] = {
                1,
                0,
                0,
                0,
        },
        [4] = {
                1,
                0,
                0,
                0,
        },
};

int guangai_lcd(void);
int tongfeng_lcd(void);
int juanlian_lcd(void);
int camera_lcd(void);
int led_lcd(void);
int red_lcd(void);
int flag_stat;

/*图标*/
struct win_icon fb1_icon[] = {
        {
                .file_name      =       "./picture/icon/1.jpg",
                .x_offs         =       600,      /*guangai*/
                .y_offs         =       40,
                .part           =       guangai_lcd,
        }, {
                .file_name      =       "./picture/icon/2.jpg",
                .x_offs         =       400,    /*tongfeng*/
                .y_offs         =       190,
                .part           =       tongfeng_lcd,
        }, {
                .file_name      =       "./picture/icon/3.jpg",
                .x_offs         =       400,    /*juanlian*/
                .y_offs         =       40,
                .part           =       juanlian_lcd,
        }, {
                .file_name      =       "./picture/icon/4.jpg",
                .x_offs         =       400,      /*camera*/
                .y_offs         =       340,
                .part           =       camera_lcd,
        }, {
                .file_name      =       "./picture/icon/6.jpg",
                .x_offs         =       600,     /*led*/
                .y_offs         =       190,
                .part           =       led_lcd,
        }, {
                .file_name      =       "./picture/icon/14.jpg",
                .x_offs         =       600,     /*red*/
                .y_offs         =       340,
                .part           =       red_lcd,
        }, 
};

struct win_screen_image background[] = {
        [0].file_name      =       "./picture/background/0.jpg",
                                        /*主界面*/
        [1].file_name      =       "./picture/background/1.jpg",
                                        /*led*/
        [2].file_name      =       "./picture/background/2.jpg",
                                        /*灌溉*/
        [3].file_name      =       "./picture/background/3.jpg",
                                        /*卷帘*/
        [4].file_name      =       "./picture/background/4.jpg",
                                        /*通风*/
        [5].file_name      =       "./picture/background/5.jpg",
                                        /*red*/
};

struct win_screen_image kaiji[] = {
        [0].file_name      =       "./picture/kaiji/0.jpg",
        [1].file_name      =       "./picture/kaiji/1.jpg",
        [2].file_name      =       "./picture/kaiji/2.jpg",
        [3].file_name      =       "./picture/kaiji/3.jpg",
};

/*screen 1 icon list*/
LIST_HEAD(scr1);
char *pwin[5] = { NULL };
int fd[5];

struct win_icon guangai_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       100,     /*back*/
                .y_offs         =       370,
        },{
                .file_name      =       "./picture/icon/10.jpg",
                .x_offs         =       300,     /*open*/
                .y_offs         =       230,
                .part           =       irr_open,
        },{
                .file_name      =       "./picture/icon/11.jpg",
                .x_offs         =       500,     /*close*/
                .y_offs         =       230,
                .part           =       irr_close,
        },

};


int guangai_lcd (void)
{
        printf("=----guangai----\n");
        int i;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        display_fb_n (pwin[0], background[2].scr_buf, 0, 0, position+0);

        for (i = 0; i < arry_size(guangai_icon);i++) {
                display_fb_n (pwin[1], guangai_icon[i].pic_buf, 
                guangai_icon[i].x_offs, guangai_icon[i].y_offs,
                position+1);
        }

        while (1) {
                read_ts();
                for (i = 1; i < arry_size(guangai_icon);i++) {
                        if ( !check_area (guangai_icon + i, position + 1)) {
                                if ( guangai_icon[i].part )
                                        guangai_icon[i].part();
                        }
                }
                if ( !check_area (guangai_icon, position + 1)) 
                        goto end;
        }
end:
        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

struct win_icon tongfeng_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       100,     /*back*/
                .y_offs         =       370,
        },{
                .file_name      =       "./picture/icon/10.jpg",
                .x_offs         =       300,     /*open*/
                .y_offs         =       230,
                .part           =       fan_open,
        },{
                .file_name      =       "./picture/icon/11.jpg",
                .x_offs         =       500,     /*close*/
                .y_offs         =       230,
                .part           =       fan_close,
        },

};

int tongfeng_lcd (void)
{
        printf("=----tongfeng----\n");
        int i;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        display_fb_n (pwin[0], background[4].scr_buf, 0, 0, position+0);

        for (i = 0; i < arry_size(tongfeng_icon);i++) {
                display_fb_n (pwin[1], tongfeng_icon[i].pic_buf, 
                tongfeng_icon[i].x_offs, tongfeng_icon[i].y_offs,
                position+1);
        }

        while (1) {
                read_ts();
                for (i = 1; i < arry_size(tongfeng_icon);i++) {
                        if ( !check_area (tongfeng_icon + i, position + 1)) {
                                if (tongfeng_icon[i].part)
                                        tongfeng_icon[i].part();
                        }
                }
                if ( !check_area (tongfeng_icon, position + 1)) 
                        goto end;
        }
end:
        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

struct win_icon juanlian_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       100,     /*back*/
                .y_offs         =       370,
        },{
                .file_name      =       "./picture/icon/15.jpg",
                .x_offs         =       300,     /*zheng*/
                .y_offs         =       130,
                .part           =       bjdj_zheng,
        },{
                .file_name      =       "./picture/icon/16.jpg",
                .x_offs         =       500,     /*fan*/
                .y_offs         =       130,
                .part           =       bjdj_fan,
        },
        //        .file_name      =       "./picture/icon/11.jpg",
        //        .x_offs         =       400,     /*close*/
        //        .y_offs         =       330,
        //        .part           =       juanlian_off,
        

};

int juanlian_lcd (void)
{
        printf("=----juanlian----\n");
        int i;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        display_fb_n (pwin[0], background[3].scr_buf, 0, 0, position+0);

        for (i = 0; i < arry_size(juanlian_icon);i++) {
                display_fb_n (pwin[1], juanlian_icon[i].pic_buf, 
                juanlian_icon[i].x_offs, juanlian_icon[i].y_offs,
                position+1);
        }

        while (1) {
                read_ts();
                for (i = 1; i < arry_size(juanlian_icon);i++) {
                        if ( !check_area (juanlian_icon + i, position + 1)) {
                                juanlian_icon[i].part();
                        }
                }
                if ( !check_area (juanlian_icon, position + 1)) 
                        goto end;
        }
end:
        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

struct win_icon camera_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       650,     /*back*/
                .y_offs         =       200,
        },
};

int camera_lcd (void)
{
        printf("=----watch----\n");
        int i;
        pid_t pid;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        for (i = 0; i < arry_size(camera_icon);i++) {
                display_fb_n (pwin[1], camera_icon[i].pic_buf, 
                camera_icon[i].x_offs, camera_icon[i].y_offs,
                position+1);
        }
        //camera(camera_icon+0, position+1);
        
        if ((pid = fork()) == 0) {
                system("/ge_lcd/camera/vedio_test");
                printf("ford---------\n");
                exit(0);
        }
        
        while (1) {
                read_ts();
                if ( !check_area (camera_icon , position + 1)) {
                        goto end;
                }
        }
end:
        if ((pid = fork()) == 0) {
                system("killall -9 vedio_test");
                printf("fork2---------\n");
                exit(0);
        }
        wait(NULL);
        wait(NULL);
        fd[4] = init_win_n (4, position + 4, color + 4, (void **)(pwin+4));
        sleep(1);
        close_win_n(4, fd[4], position + 4, pwin[4]);

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

struct win_icon led_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       100,     /*back*/
                .y_offs         =       370,
        },{
                .file_name      =       "./picture/icon/8.jpg",
                .x_offs         =       300,     /*+*/
                .y_offs         =       230,
                .part_arg           =       led_add,
        },{
                .file_name      =       "./picture/icon/9.jpg",
                .x_offs         =       500,     /*-*/
                .y_offs         =       230,
                .part_arg           =       led_sub,
        },

};


int led_lcd (void)
{
        printf("=----rest----\n");
        int i;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        display_fb_n (pwin[0], background[1].scr_buf, 0, 0, position+0);
        for (i = 0; i < arry_size(led_icon);i++) {
                display_fb_n (pwin[1], led_icon[i].pic_buf, 
                      led_icon[i].x_offs, led_icon[i].y_offs, position+1);
                
        }

        while (1) {
                read_ts();
                for (i = 1; i < arry_size(led_icon);i++) {
                        if ( !check_area (led_icon + i, position + 1)) {
                                led_icon[i].part_arg(get_led_value());
                        }
                }
                if ( !check_area (led_icon, position + 1)) 
                        goto end;
        }
end:
        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

struct win_icon red_icon[] = {
        {
                .file_name      =       "./picture/icon/5.jpg",
                .x_offs         =       100,     /*back*/
                .y_offs         =       370,
        },{
                .file_name      =       "./picture/icon/10.jpg",
                .x_offs         =       300,     /*+*/
                .y_offs         =       230,
                .part           =       open_red,
        },{
                .file_name      =       "./picture/icon/11.jpg",
                .x_offs         =       500,     /*-*/
                .y_offs         =       230,
                .part           =       close_red,
        },
};


int red_lcd (void)
{
        printf("=----rest----\n");
        int i;

        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));

        display_fb_n (pwin[0], background[5].scr_buf, 0, 0, position+0);
        for (i = 0; i < arry_size(red_icon);i++) {
                display_fb_n (pwin[1], red_icon[i].pic_buf, 
                      red_icon[i].x_offs, red_icon[i].y_offs, position+1);
        }

        while (1) {
                read_ts();
                for (i = 1; i < arry_size(red_icon);i++) {
                        if ( !check_area (red_icon + i, position + 1)) {
                                if (red_icon[i].part)
                                        red_icon[i].part();
                        }
                }
                if ( !check_area (red_icon, position + 1)) 
                        goto end;
        }
end:
        close_win_n(1, fd[1], position + 1, pwin[1]);
        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        return 0;
}

int load_image (void)
{
        int i;
        for( i=0 ; i < arry_size(fb1_icon) ; i++ ) {
                add_icon_list (&scr1, fb1_icon + i);
        }

        for ( i = 0; i < arry_size(background); ++i ) {
                mk_image_rg888 (background[i].file_name, &(background[i].scr_buf));
        }

        for ( i = 0; i < arry_size(kaiji); ++i ) {
                mk_image_rg888 (kaiji[i].file_name, &(kaiji[i].scr_buf));
        }

        for ( i = 0; i < arry_size( guangai_icon); ++i ) {
                mk_image_rg888 ( guangai_icon[i].file_name, 
                                &( guangai_icon[i].pic_buf));
        }

        for ( i = 0; i < arry_size( tongfeng_icon); ++i ) {
                mk_image_rg888 ( tongfeng_icon[i].file_name, 
                                &( tongfeng_icon[i].pic_buf));
        }

        for ( i = 0; i < arry_size( juanlian_icon); ++i ) {
                mk_image_rg888 ( juanlian_icon[i].file_name, 
                                &( juanlian_icon[i].pic_buf));
        }

        for ( i = 0; i < arry_size( camera_icon); ++i ) {
                mk_image_rg888 ( camera_icon[i].file_name, 
                                &( camera_icon[i].pic_buf));
        }

        for ( i = 0; i < arry_size( led_icon); ++i ) {
                mk_image_rg888 ( led_icon[i].file_name, 
                                &( led_icon[i].pic_buf));
        }

        for ( i = 0; i < arry_size( red_icon); ++i ) {
                mk_image_rg888 ( red_icon[i].file_name, 
                                &( red_icon[i].pic_buf));
        }
        return 0;
}

void * display_stat(void * arg)
{
        while(1)
        {
                if(flag_stat) display_stat_(pwin[1], position+1);
                sleep(1);
        }
}

void * red_houtai(void * arg)
{
        int i;
        int ret;
	char buff[128] = {0};
        while(1)
        {
                printf("red_stat:%d\n", red_stat());
                printf("wangred:%d\n", wangred());
                if(red_stat () ) 
                        if(wangred()) {
                                beep_start();

                                sprintf(buff, "echo one > /tmp/webcom");
                                system(buff);
                                system("/bin/3g");	//3g 放在bin路径下

                                sleep(5);
                                beep_stop();
                        }
                i = smog();
                ret = (i>>16)* 10;
                ret |= i&0xff;
                if (ret > 50 && ret < 100) fan_open();
                else fan_close();
;                sleep(1);
        }
}

int main(int argc, char **argv)
{
	int ret;
        int i;
        pthread_t tid[2];
        struct list_head *pos , *n;
        struct win_icon * tmp;
        init_ts();
        chdir("/ge_lcd");

        load_image ();
#if 1
        fd[0] = init_win_0 ((void **)(pwin + 0));
        assert(fd[0] >= 0);

        fd[1] = init_win_n (1, position + 1, color + 1, (void **)(pwin+1));
        assert(fd[1] >= 0);

#if 1
        for ( i = 0; i < arry_size(kaiji); ++i ) {
                display_fb_n (pwin[0], kaiji[i].scr_buf, 0, 0, position+0);
                sleep(1);
        }
#endif
                                beep_start();
                                sleep(2);
                                beep_stop();
#if 1
        if( pthread_create (tid+1, NULL, red_houtai, NULL)) {
                perror("pthread_create");
                return -1;
        }

        flag_stat = 1;
        if( pthread_create (tid+1, NULL, display_stat, NULL)) {
                perror("pthread_create");
                return -1;
        }

#endif

flag:
        flag_stat = 1;
        display_watch (pwin[1], position+1);
        display_fb_n (pwin[0], background[0].scr_buf, 0, 0, position+0);
        
        list_for_each(pos, &scr1) {
                tmp = container_of (pos, struct win_icon, list);
                ret = display_fb_n(pwin[1], tmp->pic_buf, tmp->x_offs, 
                                tmp->y_offs, position +1 );
                if ( ret < 0 )
                        break;
        }

        while (1) {
                printf("-------------test1----------\n");
                read_ts();
                printf("-------------test2----------\n");
                list_for_each(pos, &scr1) {
                        tmp = container_of (pos, struct win_icon, list);
                        if ( !check_area (tmp, position + 1)) {
                                if(tmp->part) {
                                        flag_stat = 0;
                                        tmp->part();
                                }
                                goto flag;
                        }
                }
        }
#endif    
#if 0
                        printf("x:%d,y:%d\n",x_pos ,y_pos);
                        printf("hualidefengexian\n");
                sleep(3);
#endif
        




#if 0
        for (i = 0; i < 11; i++) {
                display_number (pwin[1], number_32x56[i], 32<<16|56, 
                                i*40, 100, position + 1);
                display_number (pwin[1], number_8x16[i], 8<<16|16, 
                                i*40, 200, position + 1);
        }
#endif
#if 0
        display_fb_n(pwin[1], camera_icon.pic_buf, camera_icon.x_offs, 
                        camera_icon.y_offs, position +1 );
        display_fb_n(pwin[1], back_icon.pic_buf, back_icon.x_offs, 
                        back_icon.y_offs, position +1 );

        sleep(5);
        return ret;
error:
        list_for_each_safe(pos, n, &scr1) {
                tmp = container_of (pos, struct win_icon, list);
                free (tmp->pic_buf);
        }
        close_win_n(1, fd[1], position + 1, pwin[1]);
#endif
        return ret;

}
