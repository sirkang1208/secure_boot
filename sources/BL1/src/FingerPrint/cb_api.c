#include "cb_api.h"

#define GPIO0_RST 		0x01
#define GPIO0_RST_LOW 	
#define GPIO1_IRQ		0x02
#define GPIO0_IRQ_LOW 
#define GPIO4_CSN		0x10
#define GPIO5			0x20

extern void delay_us(int us);
void delay_ms(uint32_t ms);

extern int dorca3_cipher_decipher(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, unsigned int len, int type,int last);
extern int dorca3_cipher_decipher_big_size(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, unsigned int len, int type,int last);

typedef enum {
     FLASH_OK = 0,
     FLASH_ERROR_INVALID_PARAM,
     FLASH_ERROR_WRITING_FAILED,
     FLASH_ERROR_UNKNOWN
} FlashRetCode;

//int compare(const void* a, const void* b, size_t len)
//{
//	const unsigned char* aa = (unsigned char*)a;
//	const unsigned char* bb = (unsigned char*)b;
//	size_t i;
//	volatile unsigned char res = 0;
//	//printf("\n");
//	for (res = 0, i = 0; i < len; i++) {
//		res |= (aa[i] ^ bb[i]);
//	}
//	return res;
//}

void GPIO_reset(){

//	CMSDK_GPIO0->INTPOLSET = 0xf;
//	CMSDK_GPIO0->INTENSET = 0;
	CMSDK_GPIO0->OUTENABLESET = 0xf;

	CMSDK_GPIO0->DATAOUT  |= 0x1;

	CMSDK_GPIO0->OUTENABLESET = 0;
	CMSDK_GPIO0->OUTENABLECLR = 0xe;

//	CMSDK_GPIO0->INTTYPESET = 0;
//	CMSDK_GPIO0->INTPOLSET = 0x0;
//	CMSDK_GPIO0->INTENSET = 0xe;

	CMSDK_GPIO0->OUTENABLECLR &= 0x70;
	CMSDK_GPIO0->OUTENABLESET |= 0x71;
	
	//interrupt clear
	CMSDK_GPIO0->INTENCLR |= 0x0f;
	CMSDK_GPIO0->INTTYPECLR |= 0x0f;
	CMSDK_GPIO0->INTPOLCLR |= 0x0f;
	
	//set GPIO pin 2 rising edge interrupt
	CMSDK_GPIO0->INTENSET |= 0x02;
	CMSDK_GPIO0->INTTYPESET |= 0x02;
	CMSDK_GPIO0->INTPOLSET |= 0x2;
	
	CMSDK_GPIO0->DATAOUT |=  GPIO4_CSN;
	CMSDK_GPIO0->DATAOUT |=  GPIO5;
	CMSDK_GPIO0->DATAOUT &=  0xFD;
	
	//CMSDK_GPIO0->DATAOUT  &= 0x0;//0xF;//0xA
	printf("RST_N RESET\r\n");
	
}

uint8_t CB_READ_IRQ(){  // IRQ pin = GPIO 1
    uint8_t irq_lev = 0;
    unsigned int irq_check = 0;

    irq_check = (CMSDK_GPIO0->DATA & GPIO1_IRQ);

	if(irq_check == GPIO1_IRQ){
        irq_lev = 1;
    }
#if 1
	printf("irq_check = %d ,", irq_check);
    printf("GPIO 1 HIGH DATAOUT = %d ,", CMSDK_GPIO0->DATAOUT);
    printf("GPIO 1 HIGH DATA = %d ,", CMSDK_GPIO0->DATA);
    printf("irq_lev = %d \r\n", irq_lev);
#endif
    return irq_lev;
}

void CB_btp_RESET(void){ // RST pin = GPIO 0
    CMSDK_GPIO0->DATAOUT  = 0x00;

	delay_ms(1);

	CMSDK_GPIO0->DATAOUT  = GPIO0_RST;
}

extern char irq_res;

int wait_irq(int timeout){
	int time = timeout;

#if 0
	while(1) {
		if(CB_READ_IRQ() == 1) return 1;
		else if(timeout >= 0){
			time--;

			if (time <= 0)	break;
			
			delay_us(10);
		}
	}
	
	return 0;
#elif 1
//	printf("wfi__\r\n");
//	//__wfi();	//wait for interrupt
//	__WFI();
//	printf("wfi__out\r\n");
	
	if(irq_res == 1){
		irq_res = 0;
		return 1;
	}else{
		return 0;
	}
	
	return irq_res;
	
#else
	
	while(1){
		if(irq_res == 1){
			irq_res = 0;
			//printf("irq_res = %d \r\n", irq_res);
			return 1;
		}else if(timeout-- == 0){
			//printf("irq_res = %d \r\n", irq_res);
			break;
		}
		delay_us(10);
	}

	return 0;
#endif
	
}

int TPL_Save(int index, int size, unsigned char *tpl){
	void* store;
	unsigned char *enc_finger;
	unsigned char aes_key_h[32] = { 0x21, 0x92, 0xB3, 0x94, 0x25, 0xBB, 0xD0, 0x8B, 0x6E, 0x8E, 0x61, 0xC5, 0xD1, 0xF1, 0xBC, 0x9F, 0x42, 0x8F, 0xC5, 0x69, 0xFB, 0xC6, 0xF7, 0x8C, 0x0B, 0xC4, 0x8F, 0xCC, 0xDB, 0x0F, 0x42, 0xAE };
	unsigned char IV[16] = { 0x2d, 0xfb, 0x42, 0x9a, 0x48, 0x69, 0x7c, 0x34, 0x00, 0x6d, 0xa8, 0x86, 0x01, 0x02, 0x03, 0x04 };
	unsigned char debug_arr[100];
	
	enc_finger = malloc(MAX_TPL_SIZE);

	if(index < 0 || index > 20){
		return FLASH_ERROR_INVALID_PARAM;
	}
	if(size > MAX_TPL_SIZE){
		return FLASH_ERROR_INVALID_PARAM;
	}
	if(tpl == NULL){
		return FLASH_ERROR_INVALID_PARAM;
	}
	
	switch(index){
		case 0:
			store = FINGER_FLASH_STORE;
		break;
		case 1:
			store = FINGER_FLASH_STORE1;
		break;
		case 2:
			store = FINGER_FLASH_STORE2;
		break;
		default:
			store = FINGER_FLASH_STORE;
		break;
	}
#if USE_IV
	dorca3_cipher_decipher_big_size(CIPHER, 1, aes_key_h, 32, IV, (unsigned char*)enc_finger, tpl, MAX_TPL_SIZE, AES_MODE/*ecb*/, 1);
#else
	//dorca3_cipher_decipher_big_size(CIPHER, 1, aes_key_h, 32, NULL, (unsigned char*)enc_finger, tpl, MAX_TPL_SIZE, 0/*ecb*/, 1);
	dorca3_cipher_decipher_big_size(CIPHER, 1, aes_key_h, 32, NULL, (unsigned char*)enc_finger, tpl, MAX_TPL_SIZE, AES_MODE/*ecb*/, 1);
#endif
	memcpy((unsigned char *)store, (unsigned char *)enc_finger, size * sizeof(unsigned char));
	
#if DEBUG
	int i = 0;
	memcpy(debug_arr, tpl, 100 * sizeof(unsigned char));
	
	printf("input_PT_data (save) \r\n");
	for(i = 0; i < 100; i++){
		if(i != 0 && i % 32 == 0) printf("\r\n");
		
		printf("0x%02x ", debug_arr[i]);

	}
	printf("\r\n");
	
	printf("out_CT(enc)_data (save) \r\n");
	for(i = 0; i < 100; i++){
		if(i != 0 && i % 32 == 0) printf("\r\n");
		printf("0x%02x ", enc_finger[i]);
	}
	printf("\r\n");
	
	
#endif

	free(enc_finger);

	return FLASH_OK;
}

int first_load = 0;

void* TPL_Load(int index){
	void* rtn = NULL;
	unsigned char *dec_finger;
	unsigned char aes_key_h[32] = { 0x21, 0x92, 0xB3, 0x94, 0x25, 0xBB, 0xD0, 0x8B, 0x6E, 0x8E, 0x61, 0xC5, 0xD1, 0xF1, 0xBC, 0x9F, 0x42, 0x8F, 0xC5, 0x69, 0xFB, 0xC6, 0xF7, 0x8C, 0x0B, 0xC4, 0x8F, 0xCC, 0xDB, 0x0F, 0x42, 0xAE };
	unsigned char IV[16] = { 0x2d, 0xfb, 0x42, 0x9a, 0x48, 0x69, 0x7c, 0x34, 0x00, 0x6d, 0xa8, 0x86, 0x01, 0x02, 0x03, 0x04 };

	dec_finger = malloc(MAX_TPL_SIZE);

	switch(index){
		case 0:
			rtn = FINGER_FLASH_STORE;
		break;
		case 1:
			rtn = FINGER_FLASH_STORE1;
		break;
		case 2:
			rtn = FINGER_FLASH_STORE2;
		break;
		default:
			rtn = FINGER_FLASH_STORE;
		break;
	}
	
#if USE_IV
	dorca3_cipher_decipher_big_size(DECIPHER, 1, aes_key_h, 32, IV, dec_finger, (unsigned char *)rtn, MAX_TPL_SIZE, AES_MODE/*ecb*/, 1);
#else
	//dorca3_cipher_decipher_big_size(DECIPHER, 1, aes_key_h, 32, NULL, dec_finger, (unsigned char *)rtn, MAX_TPL_SIZE, 0/*ecb*/, 1);
	dorca3_cipher_decipher_big_size(DECIPHER, 1, aes_key_h, 32, NULL, dec_finger, (unsigned char *)rtn, MAX_TPL_SIZE, AES_MODE/*ecb*/, 1);
#endif	
	memcpy((unsigned char *)FINGER_FLASH_STORE_TEMP, (unsigned char *)dec_finger, MAX_TPL_SIZE * sizeof(unsigned char));
	
#if DEBUG
	unsigned char debug_arr[100];
	int i = 0;
	memcpy(debug_arr, rtn, 100 * sizeof(unsigned char));
	
	printf("input_CT_data (load) \r\n");
	for(i = 0; i < 100; i++){
		if(i != 0 && i % 32 == 0) printf("\r\n");
		
		printf("0x%02x ", debug_arr[i]);
	}
	printf("\r\n");
	
	printf("ouput_PT(dec)_data (load) \r\n");
	for(i = 0; i < 100; i++){
		if(i != 0 && i % 32 == 0) printf("\r\n");
		printf("0x%02x ", dec_finger[i]);
	}
	printf("\r\n");
	

#endif
	
	free(dec_finger);
	
	return FINGER_FLASH_STORE_TEMP;
}

int TPL_Delete(int index){
	memset((unsigned char *)FINGER_FLASH_STORE + (MAX_TPL_SIZE * index), 0x00, MAX_TPL_SIZE * sizeof(unsigned char));

	return FLASH_OK;
}

int TPL_Reset(void){
	memset((unsigned char *)FINGER_FLASH_STORE, 0x00, (MAX_TPL_SIZE * 20) * sizeof(unsigned char));

	return FLASH_OK;
}

#if 0
void paint_finger(unsigned char* tpl)
    int i = 0;
    int j = 0;
    
    printf("\r\n");

    for(i = 0; i < BUF_SIZE; i++){
			
//		printf("finger %d = %8x \r\n" , i, *(FINGER_DATA+i));

				
		for(j = 0; j <= 108; j++){

//            if(*((FINGER_FLASH_STORE + (MAX_TPL_SIZE * index)+i)) & 0x80000000) printf(" ");
//				else printf("8");
//            if(*((FINGER_FLASH_STORE + (MAX_TPL_SIZE * index)+i)) & 0x00800000) printf(" ");
//				else printf("8");
//			if(*((FINGER_FLASH_STORE + (MAX_TPL_SIZE * index)+i)) & 0x00008000) printf(" ");
//				else printf("8");
//			if(*((FINGER_FLASH_STORE + (MAX_TPL_SIZE * index)+i)) & 0x00000080) printf(" ");
//				else printf("8");
			
//			if(tpl[i] & 0x80000000) printf(" ");
//			else printf("8");
//			if(tpl[i] & 0x00800000) printf(" ");
//			else printf("8");
//			if(tpl[i] & 0x00008000) printf(" ");
//			else printf("8");
			if(tpl[i] & 0x00000080) printf(" ");
			else printf("8");

            //*(eFLASH_BASE + i) =  *(FINGER_DATA + i);

            i++;
        }
        i--;
        printf("\r\n");
        
    }
    
    //memcpy((unsigned int *)eFLASH_BASE, (unsigned int *)FINGER_DATA, BUF_SIZE);
    
    //*(eFLASH_BASE + 3) = 0xA5;
    
    printf("\r\n\n");
}
#endif
