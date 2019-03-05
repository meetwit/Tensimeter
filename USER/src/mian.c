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

	RCC ->APB2ENR |= (0x1<<2);//GPIOAʹ��
	GPIOA ->CRL &=~(0xF<<16);
	GPIOA ->CRL |=(0x8<<16);//in
	
	GPIOA ->CRL &=~(0Xf<<20);
	GPIOA ->CRL |=(0X3<<20);//out
	
 ADSK=0; //ʹ��AD��PD_SCK �õͣ�
 Count=0;
 while(ADDO); //ADת��δ������ȴ�������ʼ��ȡ
 for (i=0;i<24;i++)
 {
 ADSK=1; //PD_SCK �øߣ��������壩
 Count=Count<<1; //�½�����ʱ����Count����һλ���Ҳಹ��
 ADSK=0; //PD_SCK �õ�
 if(ADDO) Count++;
 }
 ADSK=1;
 //Count=Count^0x800000;//��25�������½�����ʱ��ת������
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
	
Stm32_Clock_Init(9);	//ϵͳʱ������	
	
	/*��ʱ��2��ʼ��*/
	init_tim2();
	
	/*����1��ʼ����������Ϊ9600*/
	usart_init(115200);			//����1��Ϊ���
	

	delay_init(72);					//��ʱ������ʼ��

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







