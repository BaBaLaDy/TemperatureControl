#include "./HC-SR04/HC_SR04.h"
#include "./WeiDai/WeiDai.h"
#include "./systick/bsp_systick.h"

int overcount=0; //记录定时器溢出次数

//PA6输出（Trig）  PA7输入(Echo)

void SR04_Int_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		//PA6输出，PA7输入		
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置为推挽输出
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		//定时器3初始化
		TIM_TimeBaseStructure.TIM_Period = 999; //设置自动重转载值为999
		TIM_TimeBaseStructure.TIM_Prescaler =71; //设置预分频系数
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能制定TIM3中断，允许更新中断
		//中断设置
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		TIM_Cmd(TIM3, DISABLE);
}


//测距函数
float Senor_Using(void) //单精度数据类型
{
		float length=0,sum=0;
		u16 tim;
		unsigned int i=0;
		//测五次数据计算一次平均值
		while(i!=5)
		{
			PAout(6)=1;   //拉高信号，作为触发信号
			Delay_us(20); //高电平信号超过10us
			PAout(6)=0;   //等待回响信号
			while(PAin(7)==RESET);//回响信号到来，开启定时器计数	
			TIM_Cmd(TIM3,ENABLE);
			i++; //每收到一次回响信号+1,收到5次就计算均值
			while(PAin(7)==SET);//回响信号消失
			TIM_Cmd(TIM3,DISABLE);//关闭定时器
			tim=TIM_GetCounter(TIM3); //获取计TIM3数寄存器中的计数值
			length=(tim+overcount*1000)/58.0; //通过回响信号计算距离
			sum=length+sum;
			TIM3->CNT=0; //将TIM3计数寄存器的计数值清零
			overcount=0; //中断溢出次数清零
			Delay_ms(100);
		}
			length=sum/5;
			return length; //距离作为函数返回值
}


