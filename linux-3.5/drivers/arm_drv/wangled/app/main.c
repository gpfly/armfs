/*************************************************************************
	> File Name: main.c
	> Author: ch3cooh
	> Func: 
	> Created Time: 2016年10月24日 星期一 09时06分21秒
 ************************************************************************/

#include <stdio.h>

int main(void)
{
	int ret = 0;

	led_set(5);
	ret = get_led_value();
	printf("ret = %d\n", ret);
	
	sleep(1);
	led_set(0);
	ret = get_led_value();
	printf("ret = %d\n", ret);

	return 0;
}

