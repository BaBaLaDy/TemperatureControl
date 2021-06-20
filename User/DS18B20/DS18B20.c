#include "ds18b20.h"

//short temperature;
float T;

//复位DS18B20
void DS18B20_Rst(void)	   
{                 
		DS18B20_IO_OUT(); 		//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 	//拉低DQ
    Delay_us(750);    	//拉低750us
    DS18B20_DQ_OUT=1; 	//DQ=1 
	  Delay_us(15);     	//15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();	//SET PG11 INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(void) 	 
{
    u8 data;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT
    DS18B20_DQ_OUT=0; 
	Delay_us(2);
    DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();	//SET PG11 INPUT
	Delay_us(12);
	if(DS18B20_DQ_IN)data=1;
    else data=0;	 
    Delay_us(50);           
    return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(void)     
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;	// Write 1
            Delay_us(2);                            
            DS18B20_DQ_OUT=1;
            Delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;	// Write 0
            Delay_us(60);             
            DS18B20_DQ_OUT=1;
            Delay_us(2);                          
        }
    }
}
//开始温度转换
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
	DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DS18B20_CLK_GPIO, ENABLE);	 //使能PORTG口时钟 
	
 	GPIO_InitStructure.GPIO_Pin = DS18B20_Pin;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);

 	GPIO_SetBits(DS18B20_GPIO,GPIO_Pin_6);    //输出1

	DS18B20_Rst();

	return DS18B20_Check();
}  
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
float DS18B20_Get_Temp()
{
		int temp1;
    u8 TL,TH;
		float T;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
      TH=~TH;
      TL=~TL; 
			temp1=(TH<<8)|TL;
			T=temp1/16.0;								//因为第八位里面的最后思四位存的是小数，相当于把整数部分扩大了16倍
			return -T;										//除于16.0保留小数点
    }  
		else{
			temp1=(TH<<8)|TL;				//温度为正	  	
			T=temp1/16.0;								//因为第八位里面的最后思四位存的是小数，相当于把整数部分扩大了16倍
			return T;										//除于16.0保留小数点
		}
}

short DS18B20_Get_Temp2(void)
{
    u8 temp;
    u8 TL,TH;
		short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//温度为负  
    }else temp=1;				//温度为正	  	  
    tem=TH; 					//获得高八位
    tem<<=8;    
    tem+=TL;					//获得底八位
    tem=(float)tem*0.625;		//转换     
	if(temp)return tem; 		//返回温度值
	else return -tem;    
}

float DS18B20_Get_Temp3()
{
		int temp1;
    u8 TL,TH;
		float T;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
		temp1=(TH<<8)|TL;
		T=temp1/16.0;								//因为第八位里面的最后思四位存的是小数，相当于把整数部分扩大了16倍
		return T;										//除于16.0保留小数点
}



 
