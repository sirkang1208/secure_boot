#include "spi_interface.h"

#define DEBUG 0	

extern void delay_us(int us);

int gSleep = 0;

#define SPO 			0
#define SPH 			0

#define MASTER 			0
#define SLAVE			1

#define GPIO4_CSN		0x10
#define GPIO4_CSN_LOW	0xFFFFFFEf

#define FINGER_PRINT	0

void init_master_spi(void){
	*SSP0_CPSR = 12;

	*SSP0_CR0 =  0 /*[15-8]*/| (SPH<<7) | (SPO<<6) | (0 <<5) |(0 <<4) | 7 ;

	*SSP0_CR1 = (0<<3) | (MASTER << 2) | (1<<1)| 0;
    
    *RCM3_SSP_MS_EN |= 0x01;
}

void write_spi(uint8_t * buffer, unsigned int size){
	int i = 0;
	unsigned short  flag = 0;
	
	unsigned char gar_temp = 0;
    
    for(i = 0; i < size ; i++)
    {
		flag = *SSP0_SR & (1<<1);
		
		while( flag == 0) flag = *SSP0_SR & (1<<1) ;
		
		*SSP0_DR = buffer[i];
	}
	
	for(i = 0; i < size; i++){
		flag = *SSP0_SR & ( 1<<2) ;
		while( flag ==  0) {
			flag = *SSP0_SR & ( 1<<2) ; // wating for recieving
		}	
		gar_temp = *SSP0_DR;
	}
}

//void read_spi(uint8_t * buffer, unsigned int size){
//	int i = 0;
//	unsigned short  flag = 0;
//	volatile unsigned k = 0;

//    for(i = 0; i < size ; i++){
//		flag = *SSP0_SR & ( 1<<2) ;
//		while( flag ==  0) {
//			flag = *SSP0_SR & ( 1<<2) ; // wating for recieving
//		}	
//		buffer[i]= *SSP0_DR;
//    }
//}

//int read_spi_data(uint8_t *buffer, unsigned int size){
//	int i = 0;
//	for( i = 0 ; i < size ; i++)
//	{

//		read_spi(buffer+i,1);
//	}
//	
//	return 1;
//}

void spi_read_write(uint8_t *txd, unsigned int tSize, uint8_t *rxd, unsigned int rSize){
	int i = 0;
	unsigned char gar_temp = 0;
	unsigned short  flag = 0;
	
	for(i = 0; i < tSize + rSize ; i++)
    {
		flag = *SSP0_SR & (1<<1);
		
		while( flag == 0) flag = *SSP0_SR & (1<<1) ;
		
		if(i < tSize) *SSP0_DR = txd[i];
		else{
			*SSP0_DR = 0x00;
		}
			
		flag = *SSP0_SR & ( 1<<2);
		while( flag ==  0) {
			flag = *SSP0_SR & ( 1<<2);
		
		}
		if(i < tSize) gar_temp = *SSP0_DR;
		else{
			rxd[i-tSize]= *SSP0_DR;
			
//			if(rxd[i-tSize] & 0x00000080) printf(" ");
//			else printf("8");
//			if( (i-tSize) % 108 == 0 ) printf("\r\n");
		}
    }
#if FINGER_PRINT
	for(i = 0; i <= rSize; i++){
		int j = 0;
		
		for(j = 0; j < 112; j++){
			if(rxd[i-tSize] & 0x00000080) printf(" ");
			else printf("8");
			
			i++;
		}
		i--;
		printf("\r\n");
	}
#endif
}


void ssp_write(uint8_t * buffer, unsigned int size){
	CMSDK_GPIO0->DATAOUT &= GPIO4_CSN_LOW;
	write_spi(buffer, size);
	delay_us(4);
	CMSDK_GPIO0->DATAOUT |=  GPIO4_CSN;
	delay_us(3);
	
	return ;
}

void ssp_read_write(uint8_t *txd, unsigned int tSize, uint8_t *rxd, unsigned int rSize){
	CMSDK_GPIO0->DATAOUT &= GPIO4_CSN_LOW;
	spi_read_write(txd, tSize, rxd, rSize);
	delay_us(3);
	CMSDK_GPIO0->DATAOUT |=  GPIO4_CSN;
	
	return ;
}

////////////////////////////////	SPI SLAVE MODE	/////////////////////////////////////////////////

void init_slave_spi(void){
	*SSP0_CPSR = 12;

	*SSP0_CR0 =  0 /*[15-8]*/| (SPH<<7) | (SPO<<6) | (0 <<5) |(0 <<4) | 7 ;
	//*SSP0_CR0 =  0 /*[15-8]*/| (SPH<<7) | (SPO<<6) | (FRF<<4) | 7 ;

	 *SSP0_CR1 = (0<<3) | (SLAVE << 2) | (1<<1)| 0;
    
    *RCM3_SSP_MS_EN |= 0x00;
}

void wait_fifo_empty(void)
{
	unsigned short  flag = 0;
		flag = *SSP0_SR & 1 ;
			while( flag == 0)
				flag = *SSP0_SR & 1;
}

void clear_recv_fifo(int count)
{
    unsigned short flag = 0; 	//----2 RNE Read Receive FIFO not empty (read-only):
    unsigned char dummy = 0;
    /*
    2 RNE Read Receive FIFO not empty (read-only):
    0 = Receive FIFO is empty
    1 = Receive FIFO is not empty.
   */
    flag =  *SSP0_SR & ( 1 << 2 );
    while(flag == (1<<2) )  //--- 1 = Receive FIFO is not empty.
    {
			dummy = *SSP0_DR ;
			flag =  *SSP0_DR & ( 1 << 2 );
    }
}

void wait_clk(void){
	unsigned char buffer[128];
	unsigned char flag = 0;
	int i = 0;
	
	while(1){
		//if(CMSDK_GPIO0->DATAOUT && GPIO4_CSN != GPIO4_CSN){}
		flag = *SSP0_SR & ( 1<<2) ;
		while( flag ==  0) {
			flag = *SSP0_SR & ( 1<<2) ; // wating for recieving
		}	
		buffer[i]= *SSP0_DR;
		printf("%02x ", buffer[i++]);
	}
	
}


int read_spi_header(unsigned char *buffer, int size){
	int i = 0;

	for( i = 0 ; i < size ; i++)
	{
		read_spi(buffer+i,1);
	}

	return 1;
}

void read_spi(unsigned char * buffer, int size){
	int i = 0;
	unsigned short  flag = 0;
	volatile unsigned k = 0;
	/*
    2 RNE Read Receive FIFO not empty (read-only):
    0 = Receive FIFO is empty
    1 = Receive FIFO is not empty.
   */
    for(i = 0; i < size ; i++)
    {
		flag = *SSP0_SR & ( 1<<2) ;
		while( flag ==  0) {
			flag = *SSP0_SR & ( 1<<2) ; // wating for recieving
					
		}
		buffer[i]= *SSP0_DR;
    }
}

int read_spi_data(unsigned char *buffer, int size){
	int i = 0, j=0;
	unsigned short  flag = 0;

	for( i = 0 ; i < size ; i++)
	{
	//	write_spi(txbuffer,1);
		j = i;
	//	read_spi(buffer+i,1);
		flag = *SSP0_SR & ( 1<<2) ;
		
		while( flag ==  0) {
			flag = *SSP0_SR & ( 1<<2) ; 
		}
		buffer[i]= *SSP0_DR;
	}
		
	return 1;
}

void write_spi_slave(unsigned char * buffer, int size){
	int i = 0;
	unsigned short  flag = 0;
	/*
	1 TNF Read Transmit FIFO not full (read-only):
	0 = Transmit FIFO is full
	1 = Transmit FIFO is not full.
   */
    for(i = 0; i < size ; i++){
		flag = *SSP0_SR & (1<<1) ;
		while( flag == 0)
			flag = *SSP0_SR & (1<<1) ;
		
		*SSP0_DR = buffer[i];
   	}
		
		/*
			0 TFE Read Transmit FIFO empty (read-only):
			0 = Transmit FIFO is not empty
			1 = Transmit FIFO is empty.
			*/
			// TX bufferС ۴֧Ү¶ Ңՙذՙ 
}

int write_spi_data(unsigned char *buffer, int size){
	int i = 0;
	
	for( i = 0 ; i < size ; i++){
		write_spi(buffer+i,1);
	}

	return 1;
}

unsigned char EEP_INDEX;
unsigned char buffer_para[256];
unsigned char RSA_INDEX = 0xff;
unsigned char USE_PRIVATE_EXPO = 0xff;

void RUN_SPI_SLAVE(){
	int global_size = 0;
	int size = 32;
	int i;
	
	unsigned char header[16];
	unsigned char addr = 0;

	ST_Buffer buf;
	int high_num;
	int low_num;
	
	while(1){
		int ret_read = 0;
		
		//ret_read = read_spi_header(header,3);
		read_spi(header, 3);
		printf("\r\nSPI READ DATA : ");
		for(i = 0; i < 16; i++) printf("%02x ", header[i]);
//		if(ret_read == 0)
//			continue;
		
		USE_PRIVATE_EXPO = ( header[1] >> 4) & 0x0f;
		RSA_INDEX = header[1] & 0x0f;
		addr = header[2];
		high_num = header[3];
		low_num = header[4];
		size = (high_num<<8 )|low_num;
		
		if(addr >= 0xF0 && addr <= 0xF7){
			EEP_INDEX = addr;
			global_size = size;
			continue;
		}

		if(header[0] == SPI1_WRITE_DATA){
			read_spi_data(buffer_para,size);	

			delay_us(1000);
			wait_fifo_empty();
			
			if(size == 0)
			{
				size = global_size;
			}		
			//set_parameters(buffer_para,addr,size);
		}
		
		else if(header[0] == SPI1_READ_DATA){
			unsigned char dummy = 0xff;
	
            //get_parameters(&buf,addr,size);
			write_spi_data(buf.buffer,size);

			wait_fifo_empty();

			clear_recv_fifo(0);

		}
	}
	
}