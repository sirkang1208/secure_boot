#include "i2c_interface.h"

extern void delay_us(int us);

///////////////////////////////////////////////////////////////////////////
//
//						I2C_MASTER_MODE_INTERACE
//
///////////////////////////////////////////////////////////////////////////

void i2c_master_write(unsigned char *txd, int size){
	int i = 0;

	//*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | 0x12;
	*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | ((USE_SUB_ADDR) << 4) | WRITE_MODE;
	//						0x19			  08		01						02
	delay_us(1);
	*RG_I2C_SET_ADDR = (SUB_ADDR << 8) | DEV_ADDR;
	delay_us(1);
	
	*RG_I2C_CONTROL	= I2C_START;
	delay_us(1);
	
	for ( i = 0; i < size; i++ ){
		*RG_I2C_TXD = txd[i];
		delay_us(10);
		while((*RG_I2C_STATUS) != 4);
	}
	
	delay_us(1);
	*RG_I2C_CONTROL = I2C_END;
	
	delay_us(1);
	*RG_I2C_SET_ADDR = 0x00;
	
	return ;
}

void i2c_master_read(unsigned char *rxd, int size){
	int i = 0;
	
	//*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | 0x13;
	*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | ((USE_SUB_ADDR) << 4) | (WRITE_MODE | READ_MODE);
	delay_us(1);
	*RG_I2C_SET_ADDR = (SUB_ADDR << 8) | DEV_ADDR;
	delay_us(1);
	*RG_I2C_CONTROL	= I2C_START;
	
	for ( i = 0; i < size -1 ; i++ ){
		while((*RG_I2C_STATUS) != 2);
		rxd[i] = *RG_I2C_RXD;
	}
	
	delay_us(1);
	*RG_I2C_CONTROL = I2C_END;
	
	while((*RG_I2C_STATUS) != 2);
	rxd[i] = *RG_I2C_RXD;

	return ;
}

void i2c_master_sequencial_read(unsigned char *rxd, int size){
	int i = 0;
	
	//*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | 0x03;
	*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | (N_USE_SUB_ADDR << 4) | (WRITE_MODE | READ_MODE);
	delay_us(1);
	*RG_I2C_SET_ADDR = DEV_ADDR | 0x01;
	delay_us(1);
	*RG_I2C_CONTROL	= I2C_START;
	
	for ( i = 0; i < size -1 ; i++ ) {
		while((*RG_I2C_STATUS) != 2) ;
		rxd[i] = *RG_I2C_RXD;
	}
	
	delay_us(1);
	*RG_I2C_CONTROL = I2C_END;
	
	while((*RG_I2C_STATUS) != 2) ;
	rxd[i] = *RG_I2C_RXD;
	
	return ;
}

void i2c_master_current_read(unsigned char *rxd){
	//*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | 0x03;
	*RG_I2C_DEV_MODE = (CLK_SPEED << 8) | (N_USE_SUB_ADDR << 4) | (WRITE_MODE | READ_MODE);
	delay_us(1);
	*RG_I2C_SET_ADDR = DEV_ADDR | 0x01;
	delay_us(1);
	*RG_I2C_CONTROL	= I2C_START | I2C_END;
	
	while((*RG_I2C_STATUS) != 2) ;
	rxd[0] = *RG_I2C_RXD;
	
	return ;
}

///////////////////////////////////////////////////////////////////////////
//
//						I2C_SLAVE_MODE_INTERACE
//
///////////////////////////////////////////////////////////////////////////

void i2c_slave_write(unsigned char *txd, int size){
	int i = 0;
	
	//*RG_I2C_DEV_MODE = (DEV_ADDR << 16) | 0x92;
	*RG_I2C_DEV_MODE = (DEV_ADDR << 16) | (((SUB_ADDR >> 4) + USE_SUB_ADDR) << 4) | WRITE_MODE;
	//*RG_I2C_DEV_MODE	=	0x7a0092;
	*RG_I2C_TXD = 0x00;
	
	while((*RG_I2C_STATUS) != 4);
	*RG_I2C_TXD = 0;
	
	for(i = 0; i < size; i++){
		while((*RG_I2C_STATUS) != 4) ;
		*RG_I2C_TXD = txd[i];
	}
	
	*RG_I2C_CONTROL = I2C_END;

	return ;
}

void i2c_slave_sequential_write(unsigned char *txd, int size){
	int i = 0;
	
	//*RG_I2C_DEV_MODE = (DEV_ADDR << 16) | 0x82;
	*RG_I2C_DEV_MODE = (DEV_ADDR << 16) | (((SUB_ADDR >> 4) + USE_SUB_ADDR) << 4) | WRITE_MODE;
	
	for(i = 0; i < size; i++){
		*RG_I2C_TXD = txd[i];
		while((*RG_I2C_STATUS) != 4) ;
	}
	
	delay_us(1);
	*RG_I2C_CONTROL = I2C_END;

	return ;
}

void i2c_slave_read(unsigned char *rxd, int size){
	int i = 0;
	
	//*RG_I2C_DEV_MODE	=	(DEV_ADDR << 16) | 0x92;
	//*RG_I2C_DEV_MODE	=	0x7a0092;
	*RG_I2C_DEV_MODE	=	(DEV_ADDR << 16) | (((SUB_ADDR >> 4) + USE_SUB_ADDR) << 4) | WRITE_MODE;
	//							7a					80			01						02
	//printf("\r\nmode set : %08x\r\n", (DEV_ADDR << 16) | (((SUB_ADDR >> 4) + USE_SUB_ADDR) << 4) | WRITE_MODE);
	
	while((*RG_I2C_STATUS) != 4);
	*RG_I2C_TXD = 0;
	
	for ( i = 0; i < size -1 ; i++ ) {
		while((*RG_I2C_STATUS) != 2);
		rxd[i] = *RG_I2C_RXD;
	}
	
	while((*RG_I2C_STATUS) != 2);
	rxd[i] = *RG_I2C_RXD;
	
	return ;
}
