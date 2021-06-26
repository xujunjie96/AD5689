#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"  
#include "dma.h"
#include "spi.h"
#include "string.h"
#include <stdlib.h>
//ALIENTEK 探索者STM32F407开发板 实验23
//DMA 实验-库函数版本  
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

extern rec_data_t recdata;


  
int main(void)
{  uint16_t val =0;
	float vol =0;
	u16 channel;
	u8 t=0; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(921600);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
AD5689_Init();
	while(1)
	{

		
		if(recdata.dataflag==1)
		{
			recdata.dataflag=0;
				printf("commond:%s\r\n",recdata.databuf);
			if(recdata.databuf[0]=='$' && recdata.databuf[2]==',' && recdata.databuf[1]>='0'&& recdata.databuf[1]<='3')
			{
				channel =  recdata.databuf[1]-'0';
				vol=atol(&recdata.databuf[3]);
				if(vol>=5000)
				{
					vol=5000;
				}
				set_vol(channel,vol);
				printf("commond ok:channel:%d,vol:%fmv\r\n",channel,vol);
			}
			else
			{
					printf("commond error\r\n");
			}
		
			
			
			memset(recdata.databuf,0,recdata.datalen);
		}
			
	//	delay_ms(2);
	LED0=!LED0;
	}		    
}

