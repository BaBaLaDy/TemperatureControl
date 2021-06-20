#include "JDQ.h"
#include "pid.h"
u16 pw=0;
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void JDQ_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA,PD端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 			 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_11);						 					 //PA.8 输出高
}

void PID_out()  //输出PID运算结果到负载---每1ms被调用1次
{
	 pw++;
	 if(pw>=pid.pwmcycle)  
	 {
	   pw=0;
	 } 
	 if(pw<pid.OUT)
	 {
		 JDQ=1;//加热
	 }
	 else
	 {
	   JDQ=0;//停止加热
	 }	 
}
