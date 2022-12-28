/*
 * ==========================================================================
 * ---------- IP Block Register Definition -----------------------------------
 * ==========================================================================
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
 
/* GPIO Registers*/
//#define GPIO_BASE             ((volatile unsigned int *)(0x40000000))
#define GPIO_DATA             ((volatile unsigned int *)(0x40000000))
#define GPIO_DIR              ((volatile unsigned int *)(0x40000000 + 0x0400))
#define GPIO_IE               ((volatile unsigned int *)(0x40000000 + 0x0410))

#define GPIO2_DATA            ((volatile unsigned short *)(0x40001000))
#define GPIO2_DIR             ((volatile unsigned short *)(0x40001000 + 0x0400))
#define GPIO2_IE              ((volatile unsigned short *)(0x40001000 + 0x0410))
/* Timer Registers*/
#define TIMER_BASE            ((volatile unsigned int *)(0x40002000 + 0x0400))
#define TIMER1_LOAD           ((volatile unsigned int *)(0x40002000 + 0x0400))
#define TIMER1_VALUE          ((volatile unsigned int *)(0x40002000 + 0x0404))
#define TIMER1_CONTROL        ((volatile unsigned int *)(0x40002000 + 0x0408))
#define TIMER1_INTCLR         ((volatile unsigned int *)(0x40002000 + 0x040C))
#define TIMER1_RIS            ((volatile unsigned int *)(0x40002000 + 0x0410))
#define TIMER1_MIS            ((volatile unsigned int *)(0x40002000 + 0x0414))
#define TIMER1_BGLOAD         ((volatile unsigned int *)(0x40002000 + 0x0418))
#define TIMER2_LOAD           ((volatile unsigned int *)(0x40002000 + 0x0420))
#define TIMER2_VALUE          ((volatile unsigned int *)(0x40002000 + 0x0424))
#define TIMER2_CONTROL        ((volatile unsigned int *)(0x40002000 + 0x0428))
#define TIMER2_INTCLR         ((volatile unsigned int *)(0x40002000 + 0x042C))
#define TIMER2_RIS            ((volatile unsigned int *)(0x40002000 + 0x0430))
#define TIMER2_MIS            ((volatile unsigned int *)(0x40002000 + 0x0434))
#define TIMER2_BGLOAD         ((volatile unsigned int *)(0x40002000 + 0x0438))

/* UART Registers*/
#define UART_BASE             ((volatile unsigned int *)(0x40002000))
#define UART_DR               ((volatile unsigned int *)(0x40002000 + 0x0000))
#define UART_RSR              ((volatile unsigned int *)(0x40002000 + 0x0004))
#define UART_ECR              ((volatile unsigned int *)(0x40002000 + 0x0004))
#define UART_FR               ((volatile unsigned int *)(0x40002000 + 0x0018))
#define UART_ILPR             ((volatile unsigned int *)(0x40002000 + 0x0020))
#define UART_IBRD             ((volatile unsigned int *)(0x40002000 + 0x0024))
#define UART_FBRD             ((volatile unsigned int *)(0x40002000 + 0x0028))
#define UART_LCR_H            ((volatile unsigned int *)(0x40002000 + 0x002C))
#define UART_CR               ((volatile unsigned int *)(0x40002000 + 0x0030))
#define UART_IFLS             ((volatile unsigned int *)(0x40002000 + 0x0034))
#define UART_IMSC             ((volatile unsigned int *)(0x40002000 + 0x0038))
#define UART_RIS              ((volatile unsigned int *)(0x40002000 + 0x003C))
#define UART_MIS              ((volatile unsigned int *)(0x40002000 + 0x0040))
#define UART_ICR              ((volatile unsigned int *)(0x40002000 + 0x0044))
#define UART_DMACR            ((volatile unsigned int *)(0x40002000 + 0x0048))
	
#define CODE_ADDRES	             ((volatile unsigned int *)(0x20000000 + 0x6000))
/* Asymcipher(Public Key Crypto Engine) Registers*/
#define PK_BASE               ((volatile unsigned int *)(0x40009000 ))
#define PK_PointerReg         ((volatile unsigned int *)(0x40009000 + 0x0))
#define PK_CommandReg         ((volatile unsigned int *)(0x40009000 + 0x4))
#define PK_ControlReg         ((volatile unsigned int *)(0x40009000 + 0x8))
#define PK_StatusReg          ((volatile unsigned int *)(0x40009000 + 0xC))
#define PK_VersionReg         ((volatile unsigned int *)(0x40009000 + 0x10))
#define PK_TimerReg           ((volatile unsigned int *)(0x40009000 + 0x14))
#define PK_HwConfigReg        ((volatile unsigned int *)(0x40009000 + 0x18))

//#define SIM_COUNT             ((volatile unsigned int *)(0x20000100))
// Data Area used by the PK IP Core
#define Address_0             ((volatile unsigned int *)(0x20000000 + 0x0))
#define Address_1             ((volatile unsigned int *)(0x20000000 + 0x200))
#define Address_2             ((volatile unsigned int *)(0x20000000 + 0x400))
#define Address_3             ((volatile unsigned int *)(0x20000000 + 0x600))
#define Address_4             ((volatile unsigned int *)(0x20000000 + 0x800))
#define Address_5             ((volatile unsigned int *)(0x20000000 + 0xA00))
#define Address_6             ((volatile unsigned int *)(0x20000000 + 0xC00))
#define Address_7             ((volatile unsigned int *)(0x20000000 + 0xE00))
#define Address_8             ((volatile unsigned int *)(0x20000000 + 0x1000))
#define Address_9             ((volatile unsigned int *)(0x20000000 + 0x1200))
#define Address_A             ((volatile unsigned int *)(0x20000000 + 0x1400))
#define Address_B             ((volatile unsigned int *)(0x20000000 + 0x1600))
#define Address_C             ((volatile unsigned int *)(0x20000000 + 0x1800))
#define Address_D             ((volatile unsigned int *)(0x20000000 + 0x1A00))
#define Address_E             ((volatile unsigned int *)(0x20000000 + 0x1C00))
#define Address_F             ((volatile unsigned int *)(0x20000000 + 0x1E00))

/* Asymcipher(RSAECC) Registers*/
#define RSAECC_BASE					((volatile unsigned int *)(0x4000A000 + 0x0000))
#define RSAECC_PKCC_SET				((volatile unsigned int *)(0x4000A000 + 0x0000))
#define RSAECC_PRIME_GEN_SET		((volatile unsigned int *)(0x4000A000 + 0x0004))
#define RSAECC_CTRL					((volatile unsigned int *)(0x4000A000 + 0x0008))
#define RSAECC_WRITE_DATA			((volatile unsigned int *)(0x4000A000 + 0x000C))
#define RSAECC_STATUS_REG			((volatile unsigned int *)(0x4000A000 + 0x0010))
#define RSA_ECC_READ_DATA			((volatile unsigned int *)(0x4000A000 + 0x0014))

#if 0
#define RSAECC_BASE				((volatile unsigned int *)(0x4000A000 + 0x0000))
#define RSAECC_CMD_Reg_0			((volatile unsigned int *)(0x4000A000 + 0x0000))
#define RSAECC_CMD_Reg_1			((volatile unsigned int *)(0x4000A000 + 0x0004))
#define RSAECC_CMD_Reg_2			((volatile unsigned int *)(0x4000A000 + 0x0008))
#define RSAECC_StatusReg			((volatile unsigned int *)(0x4000A000 + 0x000C)) // pkcc root operater true(1)/fail(0) result first(1) bit
#define RSAECC_STATUS_REG			((volatile unsigned int *)(0x4000A000 + 0x0010))
#define RSA_ECC_READ_DATA			((volatile unsigned int *)(0x4000A000 + 0x0014))
	
#define RSAECC_ADDR_MOD				((volatile unsigned int *)(0x4000A000 + 0x0010))
#define RSAECC_ADDR_Px				((volatile unsigned int *)(0x4000A000 + 0x0014))
#define RSAECC_ADDR_Py				((volatile unsigned int *)(0x4000A000 + 0x0018))
#define RSAECC_ADDR_Pbx				((volatile unsigned int *)(0x4000A000 + 0x001C))
#define RSAECC_ADDR_KEY 			((volatile unsigned int *)(0x4000A000 + 0x0020))
#define RSAECC_ADDR_Pby				((volatile unsigned int *)(0x4000A000 + 0x0024))
#define RSAECC_ADDR_MSG				((volatile unsigned int *)(0x4000A000 + 0x0028))
#define RSAECC_ADDR_oDx				((volatile unsigned int *)(0x4000A000 + 0x002C))
#define RSAECC_ADDR_oDy				((volatile unsigned int *)(0x4000A000 + 0x0030))
	
// Data Area used by the PKCC IP Core : 128KB
#define RSAECC_ADDR_0					((volatile unsigned int *)(0x2001D200 + 0x0000))	/*Mod		*/
#define RSAECC_ADDR_1					((volatile unsigned int *)(0x2001D200 + 0x0200))	/*Px        */
#define RSAECC_ADDR_2					((volatile unsigned int *)(0x2001D200 + 0x0400))	/*Py        */
#define RSAECC_ADDR_3					((volatile unsigned int *)(0x2001D200 + 0x0600))	/*Pbx /     */
#define RSAECC_ADDR_4					((volatile unsigned int *)(0x2001D200 + 0x0800))	/*Key /     */
#define RSAECC_ADDR_5					((volatile unsigned int *)(0x2001D200 + 0x0A00))	/*Pby /     */
#define RSAECC_ADDR_6					((volatile unsigned int *)(0x2001D200 + 0x0C00))	/*Msg /     */
#define RSAECC_ADDR_7					((volatile unsigned int *)(0x2001D200 + 0x0E00))	/*oDATAx    */
#define RSAECC_ADDR_8					((volatile unsigned int *)(0x2001D200 + 0x1000))	/*oDATAy    */
#endif
	
// Data Area used by the PKCC IP Core : 32KB
//#define RSAECC_ADDR_0					((volatile unsigned int *)(0x20007200 + 0x0000))	/*Mod						*/
//#define RSAECC_ADDR_1					((volatile unsigned int *)(0x20007200 + 0x0200))	/*Px            */
//#define RSAECC_ADDR_2					((volatile unsigned int *)(0x20007200 + 0x0400))	/*Py            */
//#define RSAECC_ADDR_3					((volatile unsigned int *)(0x20007200 + 0x0600))	/*Pbx / Key     */
//#define RSAECC_ADDR_4					((volatile unsigned int *)(0x20007200 + 0x0800))	/*Pby / Msg     */
//#define RSAECC_ADDR_5					((volatile unsigned int *)(0x20007200 + 0x0A00))	/*oDATAx        */
//#define RSAECC_ADDR_6					((volatile unsigned int *)(0x20007200 + 0x0C00))	/*oDATAy        */


/* SPI MASTER Registers */
#define SPIM_BASE           			((volatile unsigned int *)(0x40009000 + 0x0000))
#define SPIM_CONTROL	      			((volatile unsigned int *)(0x40009000 + 0x0000))
#define SPIM_CLKDIV		      			((volatile unsigned int *)(0x40009000 + 0x0004))
#define SPIM_TX_TOTAL		      		((volatile unsigned int *)(0x40009000 + 0x0008))
#define SPIM_TX_WORD		      		((volatile unsigned int *)(0x40009000 + 0x000C))
#define SPIM_WR_DATA	      			((volatile unsigned int *)(0x40009000 + 0x0010))
#define SPIM_WR_ADDR	      			((volatile unsigned int *)(0x40009000 + 0x0014))
#define SPIM_WR_START	      			((volatile unsigned int *)(0x40009000 + 0x0018))
#define SPIM_IRQ_CLR	      			((volatile unsigned int *)(0x40009000 + 0x001C))
#define SPIM_IRQ_STATUS	      			((volatile unsigned int *)(0x40009000 + 0x0020))
#define SPIM_FIFO_STATUS      			((volatile unsigned int *)(0x40009000 + 0x0024))
#define SPIM_RD_DATA	      			((volatile unsigned int *)(0x40009000 + 0x0028))

#define SSP_BASE							((volatile unsigned short *)(0x40003000))
#define SSPCR0								((volatile unsigned short *)(0x40003000 + 0x0000))
#define SSPCR1								((volatile unsigned short *)(0x40003000 + 0x0004))
#define SSPDR									((volatile unsigned short *)(0x40003000 + 0x0008))
#define SSPSR									((volatile unsigned char*)(0x40003000 + 0x000C))
#define SSPCPSR								((volatile unsigned char*)(0x40003000 + 0x0010))
#define SSPIMSC								((volatile unsigned char*)(0x40003000 + 0x0014))
#define SSPRIS								((volatile unsigned char*)(0x40003000 + 0x0018))
#define SSPMIS								((volatile unsigned char*)(0x40003000 + 0x001C))
#define SSPICR								((volatile unsigned char*)(0x40003000 + 0x0020))
	
#define SSP_BASE_2						((volatile unsigned short *)(0x40002300))
#define SSPCR0_2							((volatile unsigned short *)(0x40002300 + 0x0000))
#define SSPCR1_2							((volatile unsigned short *)(0x40002300 + 0x0004))
#define SSPDR_2								((volatile unsigned short *)(0x40002300 + 0x0008))
#define SSPSR_2								((volatile unsigned char*)(0x40002300 + 0x000C))
#define SSPCPSR_2							((volatile unsigned char*)(0x40002300 + 0x0010))
#define SSPIMSC_2							((volatile unsigned char*)(0x40002300 + 0x0014))
#define SSPRIS_2							((volatile unsigned char*)(0x40002300 + 0x0018))
#define SSPMIS_2							((volatile unsigned char*)(0x40002300 + 0x001C))
#define SSPICR_2							((volatile unsigned char*)(0x40002300 + 0x0020))

/* eFLASH REGION */
#define eFLASH_BASE						((volatile unsigned int *)(0x10000000 ))//+ 0x0000))
	


#define RG_ST0_OPMODE					((volatile unsigned int *)(0x00000000 + 0x10604))
//                                        
//#define RCM3_EEP_CTRL                   ((volatile unsigned int *)(0x00020000 + 0x00))
//#define RCM3_EEP_BYFB_LEN_ADDR          ((volatile unsigned int *)(0x00020000 + 0x04))
//#define RCM3_ST0_OPMODE                 ((volatile unsigned int *)(0x00020000 + 0x08))
//#define RCM3_SPI_TEST                   ((volatile unsigned int *)(0x00020000 + 0x0C))
//#define RCM3_SPI_LENGTH                 ((volatile unsigned int *)(0x00020000 + 0x10))
//#define RCM3_EE_KEY_AES_CTRL            ((volatile unsigned int *)(0x00020000 + 0x14))
//#define RCM3_BUF_CTRL                   ((volatile unsigned int *)(0x00020000 + 0x18))
//#define RCM3_AES_CTRL                   ((volatile unsigned int *)(0x00020000 + 0x1C))
//#define RCM3_ST1_SYMCIP_OPMODE          ((volatile unsigned int *)(0x00020000 + 0x20))
//#define RCM3_ST2_SYMCIP_OPMODE          ((volatile unsigned int *)(0x00020000 + 0x24))
//#define RCM3_KL_CTRL                    ((volatile unsigned int *)(0x00020000 + 0x28))
//#define RCM3_RSCREATE_CTRL              ((volatile unsigned int *)(0x00020000 + 0x2C))
//#define RCM3_ST3_SYMCIP_KEYLOAD_OPMODE  ((volatile unsigned int *)(0x00020000 + 0x30))
//#define RCM3_ST3_SYMCIP_RSCREATE_OPMODE ((volatile unsigned int *)(0x00020000 + 0x34))
//#define RCM3_TLS_ADDR                   ((volatile unsigned int *)(0x00020000 + 0x38))
//#define RCM3_TLS_xN_WR                  ((volatile unsigned int *)(0x00020000 + 0x3C))
//#define RCM3_TLS_xN_RD                  ((volatile unsigned int *)(0x00020000 + 0x40))
//#define RCM3_TLS_MODE                   ((volatile unsigned int *)(0x00020000 + 0x44))
//#define RCM3_EET_CTRL                   ((volatile unsigned int *)(0x00020000 + 0x48))
//#define RCM3_EET_OPMODE                 ((volatile unsigned int *)(0x00020000 + 0x4C))
//#define RCM3_EET_BYOB_LEN               ((volatile unsigned int *)(0x00020000 + 0x50))
//#define RCM3_EET_BYOB_ADDR              ((volatile unsigned int *)(0x00020000 + 0x54))
//#define RCM3_ST1_MEM_TEST_OPMODE        ((volatile unsigned int *)(0x00020000 + 0x58))
//#define RCM3_ACCESS                     ((volatile unsigned int *)(0x00020000 + 0x5C))
//#define RCM3_ST0_CUR                    ((volatile unsigned int *)(0x00020000 + 0x60))
//#define RCM3_EEP_TIME                   ((volatile unsigned int *)(0x00020000 + 0x64))
//#define RCM3_RG_EEBUF_EN                ((volatile unsigned int *)(0x00020000 + 0x1000))
//#define RCM3_RG_EEBUF_ADDR              ((volatile unsigned int *)(0x00020000 + 0x6C))
//#define RCM3_RG_EEBUF_DATA              ((volatile unsigned int *)(0x00020000 + 0x70))
//#define RCM3_EEBUF300                   ((volatile unsigned int *)(0x00020000 + 0x300))
//#define RCM3_EEBUF32F                   ((volatile unsigned int *)(0x00020000 + 0x32F))
//#define RCM3_EEBUF33F                   ((volatile unsigned int *)(0x00020000 + 0x33F))
//#define RCM3_EEBUF400                   ((volatile unsigned int *)(0x00020000 + 0x400))
//#define RCM3_TRNG_RST                   ((volatile unsigned int *)(0x00020000 + 0x80))
//#define RCM3_TRNG_256                   ((volatile unsigned int *)(0x00020000 + 0x84))
//#define RCM3_TRNG_XOR                   ((volatile unsigned int *)(0x00020000 + 0x88))
//#define RCM3_TRNG_REQ                   ((volatile unsigned int *)(0x00020000 + 0x8C))
//#define RCM3_TRNG_DONE                  ((volatile unsigned int *)(0x00020000 + 0x90))
//#define RCM3_SLEEP_REQ                  ((volatile unsigned int *)(0x00020000 + 0x94))
//#define RCM3_SLEEP_CLR                  ((volatile unsigned int *)(0x00020000 + 0x98))
//#define RCM3_WAKEUP_REQ                 ((volatile unsigned int *)(0x00020000 + 0x9C))
//#define RCM3_WAKEUP_CLR                 ((volatile unsigned int *)(0x00020000 + 0xA0))
//#define RCM3_IRQ_STATUS                 ((volatile unsigned int *)(0x00020000 + 0xA4))
//#define RCM3_BUFFER_CLR                 ((volatile unsigned int *)(0x00020000 + 0xA8))
//#define RCM3_SEL_IO                     ((volatile unsigned int *)(0x00020000 + 0xB8))
//#define RCM3_SCK_INV                    ((volatile unsigned int *)(0x00020000 + 0xBC))
//#define RCM3_SSP_MS_EN                  ((volatile unsigned int *)(0x00020000 + 0xC0))

//trng
#define RCM3_RG_EEBUF_EN                ((volatile unsigned int *)(0x00100000 + 0x1000))
#define RCM3_TRNG_RST                   ((volatile unsigned int *)(0x00100000 + 0x80))
#define RCM3_TRNG_256                   ((volatile unsigned int *)(0x00100000 + 0x84))
#define RCM3_TRNG_XOR                   ((volatile unsigned int *)(0x00100000 + 0x88))
#define RCM3_TRNG_REQ                   ((volatile unsigned int *)(0x00100000 + 0x8C))
#define RCM3_TRNG_DONE                  ((volatile unsigned int *)(0x00100000 + 0x90))
	
#define RCM3_IRQ_STATUS                 ((volatile unsigned int *)(0x00100000 + 0xA4))
#define RCM3_SLEEP_REQ                  ((volatile unsigned int *)(0x00100000 + 0x94))
#define RCM3_SLEEP_CLR                  ((volatile unsigned int *)(0x00100000 + 0x98))

#define SHA256_BASE						((volatile unsigned int *)(0x4000C000 + 0x0000))
#define SHA256_TEXT_EN					((volatile unsigned int *)(0x4000C000 + 0x0000))
#define SHA256_TEXT_DAT					((volatile unsigned int *)(0x4000C000 + 0x0004))
#define SHA256_CMD_I					((volatile unsigned int *)(0x4000C000 + 0x0008))
#define SHA256_CMD_W_I					((volatile unsigned int *)(0x4000C000 + 0x000C))
#define SHA256_IRQ_CLR					((volatile unsigned int *)(0x4000C000 + 0x0010))
#define SHA256_STATUS					((volatile unsigned int *)(0x4000C000 + 0x0014))
#define SHA256_CMD_0					((volatile unsigned int *)(0x4000C000 + 0x0018))
#define SHA256_WR_ADDR					((volatile unsigned int *)(0x4000C000 + 0x001C))
#define SHA256_OUT_0					((volatile unsigned int *)(0x4000C000 + 0x0020))
#define SHA256_OUT_1					((volatile unsigned int *)(0x4000C000 + 0x0024))
#define SHA256_OUT_2					((volatile unsigned int *)(0x4000C000 + 0x0028))
#define SHA256_OUT_3					((volatile unsigned int *)(0x4000C000 + 0x002C))
#define SHA256_OUT_4					((volatile unsigned int *)(0x4000C000 + 0x0030))
#define SHA256_OUT_5					((volatile unsigned int *)(0x4000C000 + 0x0034))
#define SHA256_OUT_6					((volatile unsigned int *)(0x4000C000 + 0x0038))
#define SHA256_OUT_7					((volatile unsigned int *)(0x4000C000 + 0x003C))
	
#define SHA256_DATA         			((volatile unsigned int *)(0x2001EE00 + 0x0000))

