#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

struct message_info {
	char cnnu[16];
	char phnu[16] ;
	char message[128] ;
};
 
struct pdu_info {
	char cnswap[32];
	char phswap[32];
};

/*
 * int tcgetattr(int fd, struct termios *termios_p);
 *	用于获取与终端相关的参数
 *
 * 第二个参数的结构体成员：
 *  tcflag_t c_iflag;       输入模式标志	控制终端输入方式
 *  tcflag_t c_oflag;       输出模式标志	控制终端输出方式
 *	tcflag_t c_cflag;       控制模式标志	指定终端硬件控制信息
 *	tcflag_t c_lflag;       本地模式 
 *	cc_t	 c_cc[NCCS];    控制字符 
 * */

/*
 * int cfsetispeed(struct termios *termios_p, speed_t speed);
 *	设置 termios 结构体中存储的输入波特率 为 speed。如果输入波特率为0，实际输入波特率将等于输出波特率
 * 
 * */

/*
 * int cfsetospeed(struct termios *termios_p, speed_t speed);
 *	设置 termios 结构体中存储的输出波特率 为 speed。
 * 
 * */

/* 
 * int tcsetattr(int fd, int optional_actions, struct termios *termios_p);
 *	设置与终端相关的参数
 * 
 * */

void serial_init(int fd)
{
	struct termios options;						/* tcgetattr 函数的第二个参数 */
	tcgetattr(fd, &options);					/* 该函数与设备文件绑定 */

	/* CLOCAL：忽略 modem 控制线。  CREAD：打开接受者 */
	options.c_cflag |= ( CLOCAL | CREAD );		/* 这两个保证程序不会成为终端的所有者，用于本地接受和发送*/
	options.c_cflag &= ~CSIZE;					/* 字符大小屏蔽  CSIZE：字符长度掩码 */
	options.c_cflag &= ~CRTSCTS;				/* 不使用数据控制流，启用RTS/CTS流控制  rts ctc 是一种通信协议*/
	options.c_cflag |= CS8;						/* 把数据设置为8位  CSIZE */
	options.c_cflag &= ~CSTOPB;					/* 设置两个停止位，而不是一个 */
	
	options.c_iflag |= IGNPAR;					/* 忽略帧错误和奇偶检验错误 */
	
	options.c_oflag = 0;
	
	options.c_lflag = 0;
	
	cfsetispeed(&options, B115200);				/*设置端口的输入波特率*/
	cfsetospeed(&options, B115200);				/*设置端口的输出波特率*/
	tcsetattr(fd, TCSANOW, &options);			/*激活配置，使其生效，第二个参数为 更改立即生效*/
}

void swap(char number[], char swap[])
{
	char ch1[] = "86";
	char tmp[16];
	int i;
	
	memset(swap, 0, 32);
	memset(tmp, 0, 16);
	strcpy(swap, number);
	strcat(swap, "f");
	strcat(ch1, swap);
	strcpy(swap, ch1);

	for (i = 0; i <= strlen(swap) - 1; i += 2) {
		tmp[i + 1] = swap[i];
		tmp[i] = swap[i + 1];
	}

	strcpy(swap, tmp);
}

int send(int fd, char *cmgf, char *cmgs, char *message)
{
	int nread, nwrite;
	char buff[128];
	char reply[128];

	memset(buff, 0, sizeof(buff));
	strcpy(buff, "at\r");

	nwrite = write(fd, buff, strlen(buff));
	printf("nwrite=%d,%s\n", nwrite, buff);

	memset(reply, 0, sizeof(reply));
	sleep(1);
	nread = read(fd, reply, sizeof(reply));
	printf("nread=%d,%s\n", nread, reply);

	memset(buff, 0, sizeof(buff));
	strcpy(buff, "AT+CMGF=");
	strcat(buff, cmgf);
	strcat(buff, "\r");
	nwrite = write(fd, buff, strlen(buff));
	printf("nwrite=%d,%s\n", nwrite, buff);

	memset(reply, 0, sizeof(reply));
	sleep(1);
	nread = read(fd, reply, sizeof(reply));
	printf("nread=%d,%s\n", nread, reply);

	memset(buff, 0, sizeof(buff));
	strcpy(buff, "AT+CMGS=\"");
	strcat(buff, cmgs);
	strcat(buff, "\"");
	strcat(buff, "\r");
	nwrite = write(fd, buff, strlen(buff));
	printf("nwrite=%d,%s\n", nwrite, buff);

	memset(reply, 0, sizeof(reply));
	sleep(1);
	nread = read(fd, reply, sizeof(reply));
	printf("nread=%d,%s\n", nread, reply);
	
	memset(buff, 0, sizeof(buff));
	strcpy(buff, message);
	nwrite = write(fd, buff, strlen(buff));
	printf("nwrite=%d,%s\n", nwrite, buff);

	memset(reply, 0, sizeof(reply));
	sleep(1);
	nread = read(fd, reply, sizeof(reply));
	printf("nread=%d,%s\n", nread, reply);
}

int send_en_message(int fd, struct message_info info)
{
	char cmgf[] = "1";
	int conter = 0;
	char cmgs[16] = {'\0'};
	char buff1[16] = "15686116871";
	char buff2[128] = "you ren tou cai, gan jin hui jia!";

	strcpy(info.phnu, buff1);

	strcpy(info.message, buff2);

	strcat(info.message, "\x1a");
	strcat(cmgs, info.phnu);
	
	send(fd, cmgf, cmgs, info.message);

}

int main(void)
{
	int fd;
	char choice;
	struct message_info info;
	
	fd = open( "/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (-1 == fd){
		perror("Can't Open Serial Port");
	}

	serial_init(fd);

	send_en_message(fd, info);

	close(fd);
	
	return 0;
}


