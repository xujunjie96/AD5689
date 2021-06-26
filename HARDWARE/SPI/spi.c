#include "spi.h"
#include "delay.h"
#include "sys.h"
#include "string.h"
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


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
 
  //GPIOFB3,4,5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
ControlPin_Init();
	//SPI1_ReadWriteByte(0xff);//��������		 
}   
//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
 		    
}

void ControlPin_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);

}

static void delay(void)
{
  uint16_t i;
  for(i=0;i<500;i++)
  {
    __NOP(); 
  }  
}

void AD5689_SetRegisterValuet(uint8_t command,uint16_t channel,uint16_t data)
{
  uint8_t reg[3]={0};
	u8 i=0;
  
  reg[0]=(command<<4)|channel;
  reg[1]=(data>>8)&0xFF;
  reg[2]=data&0xFF;

  AD5689_SYNC=0;
  delay();
//	delay_ms(1);
	for(i=0;i<3;i++)
	{
		//*((uint8_t*)&(SPI1->DR) + 1 ) = reg[i];
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET){};
//		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == RESET){};
		SPI_I2S_SendData(SPI1, reg[i]); //ͨ������SPIx����һ��byte  ����
		
	//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) ==SET);
	}
	//delay_ms(1);
   delay();
  AD5689_SYNC=1;
}

void set_sync(uint8_t moudle,uint8_t val)
{
	if(moudle==0)
	{
		AD5689_SYNC=val;
	}
	else{
		AD5689_SYNC1=val;
	}
}

void AD5689_SetRegisterValue(uint8_t moudle,uint8_t command,uint16_t channel,uint16_t data)
{
//  uint8_t reg[DACNUM*3]={0};
//	u8 i=0;
//  
//	memset(reg,0,DACNUM*3);
//  reg[moudle*3]=(command<<4)|channel;
//  reg[1+moudle*3]=(data>>8)&0xFF;
//  reg[2+moudle*3]=data&0xFF;

	  uint8_t reg[3]={0};
	u8 i=0;
  
  reg[0]=(command<<4)|channel;
  reg[1]=(data>>8)&0xFF;
  reg[2]=data&0xFF;
	
	
	set_sync(moudle,0);
 // AD5689_SYNC=0;
  delay();
//	delay_ms(1);
	for(i=0;i<3;i++)
	{
		//*((uint8_t*)&(SPI1->DR) + 1 ) = reg[i];
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET){};
//		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == RESET){};
		SPI_I2S_SendData(SPI1, reg[i]); //ͨ������SPIx����һ��byte  ����
		
	//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) ==SET);
	}
	//delay_us(100);
  delay();
 // AD5689_SYNC=1;
	set_sync(moudle,1);
}


/**
  * @brief  LDAC���μĴ�������
  * @param  LDAC����ѡ��
  * @retval ��
  */
void AD5689_LDAC_MASK(uint16_t ldac1,uint16_t ldac2)
{
  uint8_t data=ldac1;
  data |=(ldac2<<3);
  AD5689_SetRegisterValue(0,CMD_LDAC_MASK,DAC_ALL,data);
	AD5689_SetRegisterValue(1,CMD_LDAC_MASK,DAC_ALL,data);
}

/**
  * @brief  �����λ
  * @param  ��
  * @retval ��
  */
void AD5689_Reset(void)
{
  AD5689_SetRegisterValue(0,CMD_RESET,DAC_ALL,0);
	AD5689_SetRegisterValue(1,CMD_RESET,DAC_ALL,0);
}

/**
  * @brief  ��������
  * @param  channel1��ͨ��1�ĵ���ģʽ
  *         channel2��ͨ��2�ĵ���ģʽ
  * @retval ��
  */
void AD5689_PowerDown(uint16_t channel1,uint16_t channel2)
{
  uint8_t data=0x3C;
  data |=((channel2<<6)|channel1);
  AD5689_SetRegisterValue(0,CMD_POWER_DOWN,DAC_ALL,data);
	AD5689_SetRegisterValue(1,CMD_POWER_DOWN,DAC_ALL,data);
}


/**
  * @brief  д��͸���DACͨ��n(��LDAC�޹�)
  * @param  channel��ͨ��
  *         data   ����ֵ
  * @retval ��
  */
void AD5689_WriteUpdate_DACREG(uint8_t moudle,uint16_t channel,uint16_t data)
{
  AD5689_SetRegisterValue(moudle,CMD_WRITE_UPDATEn,channel,data);
}


/**
  * @brief  ������Ĵ���n�����ݸ���DAC�Ĵ���n
  * @param  channel��ͨ��
  *         data   ����ֵ
  * @retval ��
//  */
//void AD5689_InputREG_toUpdate_DACREG(uint16_t channel,uint16_t data)
//{
//  AD5689_SetRegisterValue(CMD_WRITEn_UPDATEn,channel,data);
//}

/**
  * @brief  LDAC���μĴ�������
  * @param  �ջ�������
  * @retval ��
  */
void AD5689_DAISYCHAIN_OPERATION(uint16_t decn)
{
  AD5689_SetRegisterValue(0,CMD_DCEN_REG,DAC_ALL,decn);
	AD5689_SetRegisterValue(1,CMD_DCEN_REG,DAC_ALL,decn);
	// AD5689_SetRegisterValuet(CMD_DCEN_REG,DAC_ALL,decn);
}


/**
  * @brief  AD5689��ʼ��
  * @param  ��
  * @retval ��
  */
void AD5689_Init(void)
{
	SPI1_Init();
//  AD5689_RESRT=0;
//  delay_ms(5);
//  AD5689_RESRT=1;
//  delay_ms(5);
	
 // AD5689_Reset();
	
  AD5689_LDAC_MASK(LDAC_Disable,LDAC_Disable);
	//AD5689_DAISYCHAIN_OPERATION(DCEN_mode);
	
	//AD5689_LDAC_MASK(CMD_LDAC_MASK,LDAC_Disable);
}

void set_vol(uint8_t channel,float vol)
{
	uint16_t val =0;
	val=(int)(vol*6.5535);

	switch (channel)
	{
		case 0:
			AD5689_WriteUpdate_DACREG(0,DAC_A,val); 
		break;
		case 1:
			AD5689_WriteUpdate_DACREG(0,DAC_B,val); 
		break;
		case 2:
			AD5689_WriteUpdate_DACREG(1,DAC_A,val); 
		break;
		case 3:
			AD5689_WriteUpdate_DACREG(1,DAC_B,val); 
		break;
	}
		
			
}





