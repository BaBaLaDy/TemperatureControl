#ifndef _pid_H
#define _pid_H
#include "stm32f10x.h"
//#define     MODEL_P       1
//#define     MODEL_PI      2
//#define     MODEL_PID     3

typedef struct
{
    u8 choose_model;        //使用哪个模式调节
    
    float Sv;     //用户设定值
    float Pv;     //当前值，实际值
 
    float Kp;        //比例系数
    float T;      //PID计算周期--采样周期
    u16   Tdata;    //判断PID周期到没到
    float Ti;        //积分时间常数
    float Td;       //微分系数
    
    
    
    float Ek;          //本次偏差
    float Ek_1;        //上次偏差
    float SEk;         //历史偏差之和
    
    float Iout;        //积分输出
    float Pout;        //比例输出    
    float Dout;        //微分输出
    
    float OUT0;        //一个维持的输出，防止失控
    float OUT00;
    int OUT;        //计数最终得到的值
    
    
    u16 pwmcycle;//pwm周期
    
    
}PID;


extern PID pid; //存放PID算法所需要的数据
void PID_Calc(void); //pid计算
void PID_Init(void);        //PID初始化 
#endif








