#ifndef I2C_INTERFACE
#define I2C_INTERFACE

#include "CMSDK_CM3.h"
//#include "ARMCM3.h"
#include "stdio.h"
#include "config.h"

#include <d6_config.h>

#define RCM3_I2C_ADDR00		((volatile unsigned int *)(0x4000D000 + 0x0000))
#define RCM3_I2C_ADDR04		((volatile unsigned int *)(0x4000D000 + 0x0004))
#define RCM3_I2C_ADDR08		((volatile unsigned int *)(0x4000D000 + 0x0008))
#define RCM3_I2C_ADDR0C		((volatile unsigned int *)(0x4000D000 + 0x000C))
#define RCM3_I2C_ADDR10		((volatile unsigned int *)(0x4000D000 + 0x0010))
#define RCM3_I2C_ADDR14		((volatile unsigned int *)(0x4000D000 + 0x0014))
#define RCM3_I2C_ADDR18		((volatile unsigned int *)(0x4000D000 + 0x0018))
	
#define RG_I2C_DEV_MODE		RCM3_I2C_ADDR00
#define RG_I2C_INT			RCM3_I2C_ADDR04
#define RG_I2C_CONTROL		RCM3_I2C_ADDR08
#define RG_I2C_SET_ADDR		RCM3_I2C_ADDR0C
#define RG_I2C_TXD			RCM3_I2C_ADDR10
#define RG_I2C_STATUS		RCM3_I2C_ADDR14
#define RG_I2C_RXD			RCM3_I2C_ADDR18

#define MASTER				0x08
#define SLAVE				0x00

#define USE_SUB_ADDR		0x01
#define N_USE_SUB_ADDR		0x00

#define WRITE_MODE			0x02
#define READ_MODE			0x01

#define DEV_ADDR			0x7A
#define SUB_ADDR			0x80

#define SPD_100K			0x60
#define SPD_350K			0x19
#define SPD_500K			0x13
#define SPD_1M				0x09
#define CLK_SPEED			SPD_500K

#define I2C_START			0x01
#define I2C_END				0x02

//#define I2C_CURRENT_MODE	0x03

void i2c_master_write(unsigned char *txd, int size);
void i2c_master_read(unsigned char *rxd, int size);
void i2c_master_sequencial_read(unsigned char *rxd, int size);
void i2c_master_current_read(unsigned char *rxd);

void i2c_slave_write(unsigned char *txd, int size);
void i2c_slave_sequential_write(unsigned char *txd, int size);
void i2c_slave_read(unsigned char *rxd, int size);

#endif
