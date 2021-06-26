#include "spi.h"
#include "delay.h"
#include "sys.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//SPI 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
 
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
// 
//	//这里只针对SPI口初始化
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

//	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
//	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
// 
//	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
ControlPin_Init();
	//SPI1_ReadWriteByte(0xff);//启动传输		 
}   
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
 		    
}

void ControlPin_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
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
  uint32_t reg=0;
  uint8_t i;
  
  reg=(command<<20)|(channel<<16);
  reg |=data;
  
  set_sync(moudle,0);
  delay();
  
	for(i=0;i<24;i++)
	{
		if (reg&0x800000)
		{
			AD5689_MOSI_HIGH();
		}
		else
		{
			AD5689_MOSI_LOW();
		}
		AD5689_SCK_HIGH();
		reg <<= 1;
    delay();
		AD5689_SCK_LOW();
    delay();
	}
	delay();
	set_sync(moudle,1);
  delay();
 
}




//void AD5689_SetRegisterValue(uint8_t moudle,uint8_t command,uint16_t channel,uint16_t data)
//{
////  uint8_t reg[DACNUM*3]={0};
////	u8 i=0;
////  
////	memset(reg,0,DACNUM*3);
////  reg[moudle*3]=(command<<4)|channel;
////  reg[1+moudle*3]=(data>>8)&0xFF;
////  reg[2+moudle*3]=data&0xFF;

//	  uint8_t reg[3]={0};
//	u8 i=0;
//  
//  reg[0]=(command<<4)|channel;
//  reg[1]=(data>>8)&0xFF;
//  reg[2]=data&0xFF;
//	
//	
//	set_sync(moudle,0);
// // AD5689_SYNC=0;
//  delay();
////	delay_ms(1);
//	for(i=0;i<3;i++)
//	{
//		//*((uint8_t*)&(SPI1->DR) + 1 ) = reg[i];
//		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
////	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET){};
////		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == RESET){};
//		SPI_I2S_SendData(SPI1, reg[i]); //通过外设SPIx发送一个byte  数据
//		
//	//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) ==SET);
//	}
//	//delay_us(100);
//  delay();
// // AD5689_SYNC=1;
//	set_sync(moudle,1);
//}


/**
  * @brief  LDAC屏蔽寄存器设置
  * @param  LDAC功能选择
  * @retval 无
  */
void AD5689_LDAC_MASK(uint16_t ldac1,uint16_t ldac2)
{
  uint8_t data=ldac1;
  data |=(ldac2<<3);
  AD5689_SetRegisterValue(0,CMD_LDAC_MASK,DAC_ALL,data);
	AD5689_SetRegisterValue(1,CMD_LDAC_MASK,DAC_ALL,data);
}

/**
  * @brief  软件复位
  * @param  无
  * @retval 无
  */
void AD5689_Reset(void)
{
  AD5689_SetRegisterValue(0,CMD_RESET,DAC_ALL,0);
	AD5689_SetRegisterValue(1,CMD_RESET,DAC_ALL,0);
}

/**
  * @brief  掉电设置
  * @param  channel1：通道1的掉电模式
  *         channel2：通道2的掉电模式
  * @retval 无
  */
void AD5689_PowerDown(uint16_t channel1,uint16_t channel2)
{
  uint8_t data=0x3C;
  data |=((channel2<<6)|channel1);
  AD5689_SetRegisterValue(0,CMD_POWER_DOWN,DAC_ALL,data);
	AD5689_SetRegisterValue(1,CMD_POWER_DOWN,DAC_ALL,data);
}


/**
  * @brief  写入和更新DAC通道n(与LDAC无关)
  * @param  channel：通道
  *         data   ：数值
  * @retval 无
  */
void AD5689_WriteUpdate_DACREG(uint8_t moudle,uint16_t channel,uint16_t data)
{
  AD5689_SetRegisterValue(moudle,CMD_WRITE_UPDATEn,channel,data);
}


/**
  * @brief  以输入寄存器n的内容更新DAC寄存器n
  * @param  channel：通道
  *         data   ：数值
  * @retval 无
//  */
//void AD5689_InputREG_toUpdate_DACREG(uint16_t channel,uint16_t data)
//{
//  AD5689_SetRegisterValue(CMD_WRITEn_UPDATEn,channel,data);
//}

/**
  * @brief  LDAC屏蔽寄存器设置
  * @param  菊花链操作
  * @retval 无
  */
void AD5689_DAISYCHAIN_OPERATION(uint16_t decn)
{
  AD5689_SetRegisterValue(0,CMD_DCEN_REG,DAC_ALL,decn);
	AD5689_SetRegisterValue(1,CMD_DCEN_REG,DAC_ALL,decn);
	// AD5689_SetRegisterValuet(CMD_DCEN_REG,DAC_ALL,decn);
}


/**
  * @brief  AD5689初始化
  * @param  无
  * @retval 无
  */
void AD5689_Init(void)
{
	SPI1_Init();
//  AD5689_RESRT=0;
//  delay_ms(5);
//  AD5689_RESRT=1;
//  delay_ms(5);
	
  AD5689_Reset();
	
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





