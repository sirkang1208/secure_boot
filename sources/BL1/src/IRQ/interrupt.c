#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "barley_sdk.h"

#include "CMSDK_CM3.h"

void Interrupt0()
{
	printf("\r\n I have PK interrupt");
	printf("\r\n I have PK interrupt");
	printf("\r\n I have PK interrupt");
	printf("\r\n I have PK interrupt");
	printf("\r\n I have PK interrupt");
}
void Interrupt1()
{
	printf("\r\n Interrupt1");
}
void Interrupt2()
{
	printf("\r\n Interrupt2");
}

void Interrupt3()
{
	printf("\r\n Interrupt3");
}
void Interrupt4()
{
	printf("\r\n Interrupt4");
}
void Interrupt5()
{
	printf("\r\n Interrupt5");
}

extern void ssp_write(uint8_t * buffer, unsigned int size);

void Interrupt6()
{
    //int time ;
	//uint8_t data[3]  = { 0xa9, 0x09, 0x00};
	printf("\r\n Interrupt6");
	
//	NVIC_DisableIRQ(6);
//	printf(" disable\r\n");
	
//  NVIC_ClearPendingIRQ(6);	
//  NVIC_EnableIRQ(6);
	*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0xFFFF; // 0x038 = interrupt clear address
	//NVIC_DisableIRQ(6);
	//wait_irq(100);
	
	//time = 10000;
	//while(time--) {
	//	delay_us(10);
	//}

	//*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x2;
	//ssp_write(data, 3);
	//CMSDK_GPIO0->INTENCLR &= 0x02; //GPIO_0 interrupt enable
	
	return ;
	
}
void Interrupt7()
{
	printf("\r\n Interrupt7");
}
void Interrupt8()
{
	printf("\r\n Interrupt8");
}
void Interrupt9()
{
	printf("\r\n Interrupt9");
}
int kkk = 0;
void Interrupt10()
{
	printf("\r\n Interrupt10");
	CMSDK_DUALTIMER1->TimerIntClr = 1;
	CMSDK_DUALTIMER2->TimerIntClr = 1;
								
	GPIO_SETHIGH();
	GPIO_SETLOW();
	return;
	CMSDK_GPIO0->OUTENABLESET = 0xF;
	CMSDK_GPIO0->DATA  = 0xF;
	CMSDK_GPIO0->DATAOUT  = 0xF;
	printf("\r\n INT  %d",kkk++);
	CMSDK_GPIO0->DATA  = ~0xF;
	CMSDK_GPIO0->DATAOUT  = ~0xF;;
//	__WFI();
//				CMSDK_GPIO0->DATA  = 0x1;
//				CMSDK_GPIO0->DATAOUT  = 0x1;
//				printf("\r\n INT ");
}
void Interrupt11()
{
	printf("\r\n Interrupt11");
}
void Interrupt12()
{
	printf("\r\n Interrupt12");
}

void Interrupt13()
{
	printf("\r\n Interrupt13");
}
void Interrupt14()
{
	printf("\r\n Interrupt14");
}
void Interrupt15()
{
	printf("\r\n Interrupt15 PKE");
//	*PK_ControlReg = 2;
}
void Interrupt16()
{
	
	printf("\r\n Interrupt16");
	printf("\r\n data out %x",CMSDK_GPIO0->DATA);
	//__WFI();
	//CMSDK_GPIO0->DATAOUT  = 0;
	*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0xFFFF; // 0x038 = interrupt clear address
	return;
	printf("\r\n data out %x",CMSDK_GPIO0->DATA);
	CMSDK_GPIO0->DATAOUT  = 0xf;
	*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x1;
	
}

char irq_status;
char irq_res;

extern BARLEY_CONTEXT *context;
extern int barley_verify(BARLEY_CONTEXT* context);

void Interrupt17()
{
#if 1
	printf("\r\n Interrupt17");
	//printf("\r\n data out %x",CMSDK_GPIO0->DATA);
	//CMSDK_GPIO0->DATAOUT  = 0;
	//*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0xFFFF;
	
	irq_res = 1;
	*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x2;
	
	return;	
	//printf("\r\n data out %x",CMSDK_GPIO0->DATA);
	//CMSDK_GPIO0->DATAOUT  = 0x1;
	//*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x2;
#elif 0
	unsigned char result = -1;
	
	//NVIC_ClearPendingIRQ(17);
	
	if(irq_status == 1){
		irq_status = 0;
		irq_res = 1;
		printf("finger_catpure_verify\r\n");
		result = barley_verify(context);
		printf("result : %d\r\n", result);
		irq_status = 1;
		*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x2;
		
		return ;
	}else if(irq_status == 0){
		//printf("test\r\n");
		irq_res = 1;
		*((unsigned int *)(CMSDK_GPIO0_BASE  + 0x038)) = 0x2;
		
		return ;
	}
#endif
	
}

void TimerTest2() // 1sec
{
	printf("\r\n TIMER 2 TEST");
		CMSDK_DUALTIMER2->TimerControl |=  (1<<2) // 01 4 stages of prescale, clock is divided by 16.
		|(1<<1)	// 1 32-bit counter.
		|(1<<6)	// 1 32-bit counter.
		|(1<<7);	// 1 32-bit counter.

	CMSDK_DUALTIMER2->TimerLoad = 50000000/4/4;// 50 Mhz every 16/4 sec intruppt generation 
	CMSDK_DUALTIMER2->TimerBGLoad = 50000000/4/4;// 50 Mhz every 16/4 sec intruppt generation 
	//CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA
}

void TimerTest1() // 0.5sec
{
	printf("\r\n TIMER 1 TEST");
		CMSDK_DUALTIMER1->TimerControl |=  (1<<2) // 01 4 stages of prescale, clock is divided by 16.
		|(1<<1)	// 1 32-bit counter.
		|(1<<6)	// 1 32-bit counter.
		|(1<<7);	// 1 32-bit counter.

	//CMSDK_DUALTIMER1->TimerLoad = 50000000/4/4;// 50 Mhz every 16/4 sec intruppt generation 
	//CMSDK_DUALTIMER1->TimerBGLoad = 50000000/4/4;// 50 Mhz every 16/4 sec intruppt generation 
	//CMSDK_DUALTIMER1->TimerLoad = 50000000/2;// 50 Mhz every 16/4 sec intruppt generation 
	//CMSDK_DUALTIMER1->TimerBGLoad = 50000000/2;// 50 Mhz every 16/4 sec intruppt generation 
	CMSDK_DUALTIMER1->TimerLoad = 50000000/4/8;// 50 Mhz every 16/4 sec intruppt generation 
	CMSDK_DUALTIMER1->TimerBGLoad = 50000000/4/8;// 50 Mhz every 16/4 sec intruppt generation 
	
	//CMSDK_GPIO0->OUTENABLESET = 0xF;//0xA
	printf("\r\n 0.5sec");
}



//void SysTick_Handler(void)
//{
//	printf("SysTick_Handler\r\n");
//}
