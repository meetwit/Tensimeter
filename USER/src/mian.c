#include "stm32f10x.h"
#include "tim2.h"
#include "adc.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 

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

float msg(u8 time){
	u8 time_temp;
	u32 mesg_total = 0;
	float ch_N = 0;
	
	time_temp = time;
	
	for(;time>0;time--)
		{
			mesg_total+=ReadCount();
		}
			
	//ch_N=c*50*9.8/8388608.0*2.55;
	
	ch_N=mesg_total/time_temp*50/8388608.0*2.55;

	return ch_N;

}

int main(){
	
	float biancha = 0;
	
Stm32_Clock_Init(9);	//系统时钟设置	
	
	/*定时器2初始化*/
	init_tim2();
	
	/*串口1初始化，波特率为9600*/
	usart_init(115200);			//串口1作为监控
	

	delay_init(72);					//延时函数初始化

	biancha = msg(10);
	printf("start biancha=%f\r\n",biancha);
	
	while(1){
		
		if(send_flag>=5||1){
			
//			printf("send_flag = %d\r\n",send_flag);
			printf("%.3f\r\n",msg(5)-biancha);
//			printf("send_flag = %d\r\n",send_flag);

			send_flag=0;
		}
	}

}







