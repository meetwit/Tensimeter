#include "stm32f10x.h"
#include "tim2.h"
#include "adc.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 

/*
陀螺仪配置
波特率 		115200
安装方式		垂直
回传速率		10HZ
超时时间设置为20ms < 100ms（10HZ）

连接串口2配置陀螺仪的时候 注意 将串口引脚设置为浮空输入
			usart2.c 第35行
*/

#define  ADDO PAin(4)
#define  ADSK PAout(5)

u32 ReadCount(void)
{
 u32 Count;
 u8 i;

	RCC ->APB2ENR |= (0x1<<2);//GPIOA使能
	GPIOA ->CRL &=~(0xF<<16);
	GPIOA ->CRL |=(0x8<<16);//in
	
	GPIOA ->CRL &=~(0Xf<<20);
	GPIOA ->CRL |=(0X3<<20);//out
	
 ADSK=0; //使能AD（PD_SCK 置低）
 Count=0;
 while(ADDO); //AD转换未结束则等待，否则开始读取
 for (i=0;i<24;i++)
 {
 ADSK=1; //PD_SCK 置高（发送脉冲）
 Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零
 ADSK=0; //PD_SCK 置低
 if(ADDO) Count++;
 }
 ADSK=1;
 //Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据
 ADSK=0;
 return(Count);
}


int main(){
	
	u32 a,b,c;
	
	float ch0_N,ch1_N,ch_N;
	float ch0_V,ch1_V;
	
Stm32_Clock_Init(9);	//系统时钟设置	
	
	/*定时器2初始化*/
	init_tim2();
	
	/*串口1初始化，波特率为9600*/
	usart_init(115200);			//串口1作为监控
	
	/*串口2初始化，波特率为9600*/
	//usart2_init(115200);			//串口2作为陀螺仪
	
	/*串口3初始化，波特率为9600*/
	//usart3_init(19200);			//串口3无线连接

	delay_init(72);					//延时函数初始化
	Adc_Init();							//PA1,开启ADC通道一
	
	printf("start\r\n");
	
	while(1){
		
		//Task_Pc();
		//Task_Pc2();
		//Task_Pc3();
		
		if(send_flag>=2){
			a=Get_Adc(6);	//ch0
			b=Get_Adc(7);	//ch1
			c=ReadCount();
			
			ch_N=c*50*9.8/8388608.0*2.55;
			
			ch0_N=a*30*9.8/4096.0;
			ch1_N=b*30*9.8/4096.0;
			
			ch0_V=a*3.3/4096.0;
			ch1_V=b*3.3/4096.0;
			
//			printf("ch0=,%d,,ch1=,%d,\r\n",a,b);
//			printf("ch0_V=,%f,V,ch1_V=,%fV,\r\n",ch0_V,ch1_V);
//		printf("ch0_N=,%f,N,ch1_N=,%fN\r\n",ch0_N,ch1_N);
//			printf("%f,%f",ch0_N,ch1_N);
//		printf("c=%d,c=%fN,\r\n",c,ch_N);
				printf("%f\r\n",ch_N);
				send_flag=0;
		}
	}

}







