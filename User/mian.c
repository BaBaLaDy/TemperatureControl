#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "oled.h"
#include "DS18B20.h"
#include "PID.h"
#include "Tim.h"
#include "JDQ.h"
#include "valuepack.h"
#include "key.h"
extern float Water;
void PID_Init()            
{
    //pid.choose_model = MODEL_PID;
    
    pid.T=500;                //采样周期，定时器使用1ms，则最小执行PID的周期为330ms
    
		pid.Sv=40;                //用户设定值
    pid.Kp=29;                //比例系数
		pid.Ti=4500000;            //积分时间
    pid.Td=530;                    //微分时间
    pid.OUT0=1;	//一个维持的输出
    pid.OUT00 = 0;
    pid.pwmcycle = 200;    //PWM的周期
}
  
void hardwareinit(void)
{
	JDQ_Init();
  //USART_Config();
	initValuePack(115200);
	TIM3_Int_Init(10-1,7200-1); //1MS进行一次PID的运算
	TIM4_Int_Init(7500-1,7200-1); //0.75s进行一次温度的读取
	DS18B20_Init();
	Key_GPIO_Config();
	PID_Init();
	I2C_Configuration();
}

int main()
{
	char Temp[10];
	int flag=1;
//	char Temp1[10];
//	char pidout[10];
	hardwareinit();
	OLED_Init();
	OLED_CLS();
	OLED_ShowStr(0, 0, "Temp:", 2);

	while(1)	
	{
		PID_Calc(); //pid计算
		sprintf(Temp,"%.3f",pid.Pv);

		OLED_ShowStr(50,0, Temp, 2);
		
    OLED_ShowStr(0,2,"SetTemp:",2);
		OLED_ShowNum(65,2,pid.Sv,3,16);
		
//		//温度判断
//		if(pid.Pv<pid.Sv-8)
//		{
//			TIM_Cmd(TIM3, DISABLE);  //失能TIMx外设
//			JDQ=1;
//		}
//		else if((pid.Pv>pid.Sv-8) && flag)
//		{
//			JDQ=0;
//			OLED_ShowStr(80,2, "Stop", 2);
//			
//			for(int i=0;i<40000000;i++);
//			pid.Pv=DS18B20_Get_Temp();
//			PID_Calc(); //pid计算
//			OLED_ShowStr(80,2, "Star", 2);
//			TIM_Cmd(TIM3, ENABLE);
//			flag=0;
//		}
		
		
		if(readValuePack(&rxpack))
		{
			pid.Sv=rxpack.integers[0];
		  if(pid.Sv-pid.Pv>8)
			{
				flag=1;
			}

		}
		//按键控制目标温度
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			
			pid.Sv+=5;
			if(pid.Sv-pid.Pv>8)
			{
				flag=1;
			}
		} 
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			pid.Sv-=5;

		}
	}
}
