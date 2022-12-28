#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//#include "ARMCM3.h"
#include "CMSDK_CM3.h"

#include "barley_sdk.h"
#include "barley_retno.h"
#include "barley_def.h"
#include "barley_conf.h"

#include "cb_api.h"

#include "spi_interface.h"

extern void UartStdOutInit(void);
extern void delay_us(int us);
extern void delay_ms(uint32_t ms);

extern int lib_fp_enroll(BARLEY_CONTEXT* context, int i);
extern int barley_verify(BARLEY_CONTEXT* context);

static BARLEY_Mode g_cur_mode ; /// Mode used by fingerprint threadb

int finger_print_test(void);

BARLEY_CONTEXT *context = NULL;
extern char irq_status;

int finger_print_test(){
	int i = 0;
	unsigned char result = -1;

    //BARLEY_CONTEXT *context = NULL;
	
//	NVIC_EnableIRQ(27);
//	NVIC_EnableIRQ(28);
	
	
#if DEBUG

	printf("finger_sensor_test_start\r\n");
	
	switch(AES_ARIA){
		case 0:
			printf("ARIA - ");
			break;
		case 1:
			printf("AES - ");
			break;
		default:
			break;
	}
	
	switch(AES_MODE){
		case 0:
			printf("ECB\r\n");
			break;
		case 1:
			printf("CBC\r\n");
			break;
		case 2:
			printf("OFB\r\n");
			break;
		case 3:
			printf("CTR\r\n");
			break;
		case 4:
			printf("CFB\r\n");
			break;
		default:
			break;
	}
#endif
	
	BARLEY_INIT_PARAM init_param;
    init_param.enroll_count= BARLEY_INIT_ENROLL_COUNT;
    init_param.max_tpl_count = 1;
    init_param.max_tpl_size = BARLEY_INIT_MAX_TPL_SIZE;
    init_param.learn_flag = BARLEY_INIT_LEARNING;
	
    init_param.enroll_ip = BARLEY_INIT_ENROLL_IP;
    init_param.verify_ip = BARLEY_INIT_VERIFY_IP;
    init_param.sec_lev = BARLEY_INIT_SEC_LEVEL;

	static PLATFORM_API my_platform_api = {
        .malloc = malloc,
		.calloc = calloc,
        .free = free,
		.memset = memset,
		.memcpy = memcpy,
		.delay = delay_ms,
		
		.btp_irq = wait_irq,	
		.btp_reset = CB_btp_RESET,

		.spi_write = ssp_write,
		.spi_read = read_spi,
		.spi_write_read = ssp_read_write,

		.tpl_save = TPL_Save,
		.tpl_load = TPL_Load,
        .tpl_delete = TPL_Delete,

		.debug = printf,
		.get_tick = NULL
    };

	printf("\r\btp_irq = %d\r\n", my_platform_api.btp_irq);
	
//	printf("\r\nfinger_init before\r\n");
//	printf("\r\n");	__heapstats((__heapprt)fprintf,stderr);
//	printf("\r\n");__heapvalid((__heapprt) fprintf, stderr, 1);
	
	context = BARLEYAPI_init(init_param, my_platform_api);
	
	CMSDK_GPIO0->DATAOUT |= 0x20;	// verify signal(high to low) for rasberry pi 
	
//	printf("\r\nfinger_init after\r\n");
//	printf("\r\n");	__heapstats((__heapprt)fprintf,stderr);
//	printf("\r\n");__heapvalid((__heapprt) fprintf, stderr, 1);
	
	if(context == NULL ){
		g_cur_mode = MODE_ERROR;
		printf("fail\r\n");
	}else{
		g_cur_mode = MODE_STANDBY;
		printf("pass\r\n");
	}
	i++;
	
	
	//NVIC_EnableIRQ(6);
	
	for(i = 0; i < 8; i++){
		printf("\r\nfinger_catpure_enroll %d\r\n", i);
		result = lib_fp_enroll(context, i);
		printf("lib_fp_enroll result : %d \r\n", result);
	}
	printf("finger_catpure_enroll complete\r\n");
	
	//delay_ms(10);
	

#if 0
	//result = BARLEY_WaitingForFingerDown();
	result = BARLEYAPI_start_capture(context);
	irq_status = 0;
	printf("BARLEYAPI_start_capture result : %d \r\n", result);
	while(1){
		
	};
#else
	while(1){
		
		printf("finger_catpure_verify\r\n");
		result = barley_verify(context);
		printf("result : %d\r\n", result);
		if(result == 9){
			init_slave_spi();
			CMSDK_GPIO0->DATAOUT &= 0xFFFFFFDf;
			
			delay_us(1000);
			
			unsigned char* ret;
			
			ret = &result;
			
			write_spi_slave(ret, 1);
			
			return 0;
		}
	}
//expect modified 1 end
#endif
	return 0;
}