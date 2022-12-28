#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "barley_sdk.h"

#include "CMSDK_CM3.h"

extern void delay_us(uint32_t us);

void init_interrupt()
{
	int i = 0;
		printf("\r\n NORMAL MODE TEST\r\n");
	for( i= 0; i < 255; i++)
		NVIC_EnableIRQ(i);	
}

void GPIO_SETHIGH()
{
	printf("\r\n GPIO_SETHIGH");
	CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA	
	CMSDK_GPIO0->DATAOUT  =  0xF;//0
	CMSDK_GPIO0->DATA  =  0xF;//0
	
	CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA	

}
void GPIO_SETLOW()
{
	printf("\r\n GPIO_SETLOW");
CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA	
	CMSDK_GPIO0->DATAOUT  =  0;
	CMSDK_GPIO0->DATA  =  0;
	
	CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA	
}

//void initSys()
//{
//	*RCM0_SEL_IO  |= 0x2;

//	//for( i = 0; i < 6; i++)
//	NVIC_EnableIRQ(3);
//	NVIC_EnableIRQ(4);

//	*GPIO_DIR  |=    1<<5 ;
//	*GPIO_DATA  = 0;

//}

//#define GPIO_OUT_TEST

void irq_test(){
	
	for(int i=0; i<32; i++)
	{
		printf("\r\ntttt");
		delay_us(1000);
	}
//	CMSDK_GPIO0->OUTENABLESET = 0;
//	CMSDK_GPIO0->INTTYPESET = 0;
//	CMSDK_GPIO0->INTPOLSET = 0x2;
//	CMSDK_GPIO0->INTENSET = 0;
	
//	CMSDK_GPIO0->DATAOUT  = 0x0;//0xF;//0xA
	printf("RST_N RESET\r\n");
	

//	init_interrupt();
	//NVIC_EnableIRQ(16);

	
	//fMSDK_GPIO0->OUTENABLECLR = 0x30;
//	CMSDK_GPIO0->INTENSET |= 0x02; //GPIO_0 interrupt enable
//	delay_us(100);
//	printf("\r\n testll");
	
	
	int timeout = 10;
	while(timeout--){
		printf("WFI IN\r\n");
		__WFI();
		printf("WFI OUT\r\n");
		delay_us(100);
	};
	
	return;
}

void timer_test(){
	int i = 0;

	NVIC_EnableIRQ(2);  //GPIO_0 interrupt no.2
	NVIC_EnableIRQ(3);  //GPIO_0 interrupt no.3
	NVIC_EnableIRQ(10);
	NVIC_EnableIRQ(16);  //GPIO_0 interrupt no.16
	
	#if 0
	TimerTest1();
	#else
	TimerTest2();
	#endif
	
	printf("\r\n start \r\n");
	
	while(1) printf("\r\n %d ", i++);
}

void systick_test(){
	SysTick_Config(SystemCoreClock / 100);
	
}
