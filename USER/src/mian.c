#include "stm32f10x.h"
#include "tim2.h"
#include "adc.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "stdio.h" 

/*
����������
������ 		115200
��װ��ʽ		��ֱ
�ش�����		10HZ
��ʱʱ������Ϊ20ms < 100ms��10HZ��

���Ӵ���2���������ǵ�ʱ�� ע�� ��������������Ϊ��������
			usart2.c ��35��
*/

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


int main(){
	
	u32 a,b,c;
	
	float ch0_N,ch1_N,ch_N;
	float ch0_V,ch1_V;
	
Stm32_Clock_Init(9);	//ϵͳʱ������	
	
	/*��ʱ��2��ʼ��*/
	init_tim2();
	
	/*����1��ʼ����������Ϊ9600*/
	usart_init(115200);			//����1��Ϊ���
	
	/*����2��ʼ����������Ϊ9600*/
	//usart2_init(115200);			//����2��Ϊ������
	
	/*����3��ʼ����������Ϊ9600*/
	//usart3_init(19200);			//����3��������

	delay_init(72);					//��ʱ������ʼ��
	Adc_Init();							//PA1,����ADCͨ��һ
	
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







