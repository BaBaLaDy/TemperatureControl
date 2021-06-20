#ifndef __DS18B20x_H
#define __DS18B20x_H

#include "stm32f10x.h"
#include "WeiDai.h"

#include "bsp_systick.h"



//IO口方向
#define DS18B20x_IO_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define DS18B20x_IO_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
#define DS18B20x_DQ_OUT    PCout(6) //PE6
#define DS18B20x_DQ_IN     PCin(6)  //PE6 

#define DS18B20_CLK_GPIO  RCC_APB2Periph_GPIOC
#define DS18B20_GPIO 			GPIOC
#define DS18B20_Pin  			GPIO_Pin_6;

extern float T;

u8 DS18B20x_Init(void);//初始化DS18B20
float DS18B20x_Get_Temp(void);//获取温度
short DS18B20x_Get_Temp2(void);
float DS18B20x_Get_Temp3(void);
void DS18B20x_Start(void);//开始温度转换
void DS18B20x_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20x_Read_Byte(void);//读出一个字节
u8 DS18B20x_Read_Bit(void);//读出一个位
u8 DS18B20x_Check(void);//检测是否存在DS18B20
void DS18B20x_Rst(void);//复位DS18B20    


#endif