#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"  
#include "dma.h"
#include "spi.h"
#include "string.h"
#include <stdlib.h>
//ALIENTEK ̽����STM32F407������ ʵ��23
//DMA ʵ��-�⺯���汾  
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

extern rec_data_t recdata;


  
int main(void)
{ 
	float vol =0;
	u16 channel;
	u8 t=0; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);     //��ʼ����ʱ����
	uart_init(921600);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
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
			
		delay_ms(2);
	LED0=!LED0;
	}		    
}

