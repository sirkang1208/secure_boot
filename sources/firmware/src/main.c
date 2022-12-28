#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #include "ARMCM3.h"
#include "CMSDK_CM3.h"
#include "sha256_t.h"
#include <mbedtls.h>

extern void UartStdOutInit(void);
extern void delay_us(int us);

extern int sha256_test();
extern void sha256_hmac_test(uint8_t *result);
extern void sha256_hmac_test_sw(uint8_t *result);
extern int ecdh_pkcc_function_call_test();
extern void ec_elgamal_test();
extern void ecdsa_sign_function_test();
extern void ecdsa_verify_function_test();
extern void rsa_test_encrypt(uint32_t *msg, uint32_t *res);
extern void rsa_test_decrypt(uint32_t *msg, uint32_t *res, uint32_t *n, uint32_t *e);
extern void aes_gcm_test();

extern void ecc_ECPD_256_test();
extern void ecc_MS_256_test();
extern void ecc_MD_256_test();

extern void GPIO_reset();
extern int finger_print_test();
extern void init_master_spi(void);
extern void spi_read_write(uint8_t *txd, unsigned int tSize, uint8_t *rxd, unsigned int rSize);

#define RAM_START ((volatile unsigned int *)(0x20020000))

extern void init_interrupt();

unsigned int sys_msec = 1;

void SysTick_Handler(void)
{
	// printf("SysTick_Handler\r\n");
	sys_msec++;
}

int main()
{
	int ret = 1;

	// initialize Uart
	UartStdOutInit();

	// initialize spi
	init_master_spi();

	// General Purpose Input/Output reset
	// RST_N RESET
	GPIO_reset();

	printf("\n\nFirmware jump success\n\n");

	// give delay to board
	delay_us(1000);

	printf("changed Firmware");

	printf("\r\n=======================================================================\n");

	printf("\r\n ALL TEST END\n");

	return 0;
}
