#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define ADC_CH4  4  	//通道4		 	  						  	 	    
#define ADC_CH5  5  	//通道5	
#define ADC_CH6  6  	//通道6		 	  						  	 	    
#define ADC_CH7  7  	//通道7	 
#define ADC_CH8  8  	//通道8		 	  						  	 	    
#define ADC_CH9  9  	//通道9	 	
#define ADC_CH10  10  	//通道10		 	  						  	 	    
#define ADC_CH11  11  	//通道11	 	    
	   									   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道10次采样的平均值 	  
#endif 















