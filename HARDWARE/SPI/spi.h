#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define DACCHANNEL	2
#define DACNUM			2
#define ALLCHANNELNUM			DACCHANNEL*DACNUM

#define AD5689_SYNC PBout(6)	
#define AD5689_SYNC1 PBout(7)		 

#define     NormalOperation					0
#define     R1K2GND									1
#define     R100K2GND								2
#define     ThreeState							3

#define     DAC_A    								0x01
#define     DAC_B										0x08
#define     DAC_ALL									0x09

#define     LDAC_Enable     				0
#define     LDAC_Disable						1

#define     Standalone_mode 				0
#define     DCEN_mode								1 
	
#define CMD_WRITEn            	    1
#define CMD_WRITEn_UPDATEn          2
#define CMD_WRITE_UPDATEn           3
#define CMD_POWER_DOWN              4
#define CMD_LDAC_MASK               5
#define CMD_RESET               		6
#define CMD_DCEN_REG            		8
#define CMD_READ_BACK          		  9
					
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�

void set_vol(uint8_t channel,float vol);
void AD5689_Init(void);
void AD5689_WriteUpdate_DACREG(uint8_t moudle,uint16_t channel,uint16_t data);
void ControlPin_Init(void);
#endif

