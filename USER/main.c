#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "usart2.h"
#include "timer.h"
#include "gizwits_product.h"
#include "math.h"
#include "SG90.h"
#include "dht11.h"
#include "adc.h"
#include "stm32f10x_adc.h"
//声明
int flag=0;
int MQ2_flag=0;
uint16_t MQ2;
int MQ2_threshold;
u8 data_temp,data_hum;//定义温度数据和湿度数据的变量

int main(void)
{	

	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	adc_Init();
	OLED_Init();
	OLED_Clear();
	TIM3_Int_Init(71,999);				//一毫秒定时
  SG90_Init(1999,719);
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	usart3_init((u32)9600);//WIFI初始化
	gizwitsInit();//缓冲区初始化
//	gizwitsSetMode(WIFI_RESET_MODE);
//	delay_ms(500);
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	gizwitsSetMode(WIFI_SOFTAP_MODE);
	OLED_ShowString(0,2,"MQ2:",16);
		while(DHT11_Init())//等待DHT11初始化完成
	{;}
	while(1)
	{
		MQ2 = AD_GetValue(ADC_Channel_1);//读取烟雾传感器的数据
		OLED_ShowNumber(70,2,MQ2,3,16);
		
		data_hum=DHT11_Read_hum();
		OLED_ShowString(80,6,"HUM",16);
		OLED_ShowNumber(103,6,data_hum,3,16);
		
		//显示温度值
		data_temp=DHT11_Read_temp();
		OLED_ShowString(80,4,"TMP",16);
		OLED_ShowNumber(103,4,data_temp,3,16);
		
		if(MQ2>MQ2_threshold)
		{
			MQ2_flag=1;
		}
		else
		{
			MQ2_flag=0;
		}
		userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
		if(flag==1)
		{
			steer();
//			flag=0;
		}
		else
		{
			TIM_SetCompare1(TIM4,1945);
		}

}
}









