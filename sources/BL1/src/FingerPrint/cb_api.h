#ifndef CB_API
#define CB_API

#include <stdio.h>
#include <stdlib.h>

#include "CMSDK_CM3.h"

#include <d6_config.h>

#define eFLASH_BASE				((volatile unsigned int *)(0x10000000 ))//+ 0x0000))

#define BUF_SIZE 		11000
#define MAX_TPL_SIZE 	0x4000

#define FINGER_DATA             ((volatile unsigned int *)(0x20000000 + 0x4000))
//#define FINGER_FLASH_STORE_TEMP	((volatile unsigned int *)(0x10000000))
//#define FINGER_FLASH_STORE		((volatile unsigned int *)(0x10000000 + 0x4000))
//#define FINGER_FLASH_STORE1		((volatile unsigned int *)(0x10000000 + 0x8000))
//#define FINGER_FLASH_STORE2		((volatile unsigned int *)(0x10000000 + 0x10000))

#define FINGER_FLASH_STORE_TEMP	((volatile unsigned int *)(0x00040000))
#define FINGER_FLASH_STORE		((volatile unsigned int *)(0x00040000 + 0x4000))
#define FINGER_FLASH_STORE1		((volatile unsigned int *)(0x00040000 + 0x8000))
#define FINGER_FLASH_STORE2		((volatile unsigned int *)(0x00040000 + 0x10000))

#define AES_ARIA		1 // 0 = ARIA, 1 = AES

#define AES_ECB     	0
#define AES_CBC     	1
#define AES_OFB     	2
#define AES_CTR     	3
#define AES_CFB     	4	

#define USE_IV			1
#define AES_MODE    	AES_CFB

#define CIPHER			0
#define DECIPHER		1

#define CB_API_DEBUG	0

#define DEBUG			1

void CB_btp_RESET(void);
void GPIO_reset(void);
int wait_irq(int timeout);

int TPL_Save(int index, int size, unsigned char *tpl);
void* TPL_Load(int index);
int TPL_Delete(int index);
int TPL_Reset(void);

#endif
