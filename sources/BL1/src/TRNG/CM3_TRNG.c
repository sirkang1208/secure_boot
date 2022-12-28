#include "config.h"
#include "CMSDK_CM3.h"
#include <usr_rng.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RNG_GENERATION_TIMEOUT   100000

/* Private macro ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name : 
* Parameters    : 
* Return        : 
* Description   : 
*******************************************************************************/
#define RNG_RUN_RUN   0x01
#define RNG_RUN_STOP  0x00

void CM3_STATE_INIT()
{
	*RCM3_ST0_OPMODE = 0x01;
	*RCM3_ACCESS  = 0x01;
}

//uint32_t RNG_ReadRandomNumber(void)
uint32_t RNG_ReadRandomNumber(uint32_t *Dat)
{
	uint32_t Rngloop;
	volatile unsigned int *pReadMem32;
	uint32_t ulTimeout = RNG_GENERATION_TIMEOUT;

	RNG->RNG_OPMODE = 0x01;
	RNG->RNG_ACCESS = 0x00;
	RNG->RNG_OPMODE = 0x03;
	
	RNG->RNG_RST = 0x02;
	RNG->RNG_256 = 0x11;
	RNG->RNG_XOR = 0xffff0000;
	RNG->RNG_REQ = 0x01;
	while(RNG->RNG_DONE != 0x01 &&( ulTimeout != 0x00)){ulTimeout--;};	
	if(ulTimeout == 0x00){          
		return 0;      
	}	
	
	RNG->RNG_BUFEN = RNG_RUN_RUN;
	pReadMem32 = RNG_DR;

	for(Rngloop = 0; Rngloop < 8; Rngloop++){
		Dat[Rngloop] = pReadMem32[Rngloop];	 
	}
	RNG->RNG_BUFEN = RNG_RUN_STOP;
	
	CM3_STATE_INIT();

	return 0;
}

/*******************************************************************************
* Function Name : 
* Parameters    : 
* Return        : 
* Description   : 
*******************************************************************************/
uint32_t Get_TRNG(uint32_t *chpData, uint16_t nhDatalen)
{
	uint32_t Rngloop;

	for(Rngloop = 0; Rngloop < nhDatalen; Rngloop++){
	}
	return 0;
}



void TRNG_DATA_get_para(ST_Buffer *buf, int size)
{
    unsigned int expected_result32[8];
    unsigned char readData[32], temp32[32], temp16[16], temp[16], temp_test[16];
	unsigned int ADDR = 0;
	int i,j,round;
	int index = 0;

	for( i=0; i<size; i++) temp_test[i] = i+1;

	for( round = 0; round <8; round++) { 	
		//trng_test(expected_result32);
		
		RNG_ReadRandomNumber(expected_result32);
			
		for( i = 0, j = 0; i < 8; i++) {
			temp32[j+0] = (expected_result32[i] >> 24) & 0xFF;
			temp32[j+1] = (expected_result32[i] >> 16) & 0xFF;
			temp32[j+2] = (expected_result32[i] >> 8) & 0xFF;
			temp32[j+3] = (expected_result32[i]) & 0xFF;
			j += 4;
		}
		
		#if 1
		for( i = 0 , j = 0; i < 32; i++) {
			if( i%2 != 0){ temp16[j] = temp32[i]; j++;}
		}
		
		temp_test[index++] = temp16[7];
		temp_test[index++] = temp16[15];
		#else
		
		temp_test[index++] = temp32[31-round];
		temp_test[index++] = temp32[23-round];
		#endif
	}

	buf->buffer = temp_test;

}

int get_trg_data_parameter(ST_Buffer *buf, unsigned char addr, int size)
{
	TRNG_DATA_get_para(buf,size);
}

void trng_test(){
	unsigned int expected_result32[8];
    unsigned char readData[32], temp32[32], temp16[16], temp[16], temp_test[16];
	unsigned int ADDR = 0;
	int i,j,round;
	int index = 0;

	//for( i=0; i<32; i++) temp_test[i] = i+1;

	for( round = 0; round <8; round++) { 	
		//trng_test(expected_result32);
		
	RNG_ReadRandomNumber(expected_result32);
		
//	for( i = 0, j = 0; i < 8; i++) {
//		temp32[j+0] = (expected_result32[i] >> 24) & 0xFF;
//		temp32[j+1] = (expected_result32[i] >> 16) & 0xFF;
//		temp32[j+2] = (expected_result32[i] >> 8) & 0xFF;
//		temp32[j+3] = (expected_result32[i]) & 0xFF;
//		j += 4;
//	}
	
	printf("\r\n trng :");
	for( i=0; i<8; i++) printf(" 0x%08x", expected_result32[i]);
	}
}