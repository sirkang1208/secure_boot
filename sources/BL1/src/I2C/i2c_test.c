#include "i2c_interface.h"

extern void delay_us(int us);

void i2c_test(){
	unsigned char txd[128] = { 0xaa, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
	unsigned char rxd[128] = { 0, };
	unsigned char data[128] = { 0, };
	int i = 0, j = 0;
	unsigned int count = 1;
	unsigned int Delay = 10000;
	printf("\r\n=======================================================================\n");
	printf("\r I2C TEST\n");
	printf("\r=======================================================================\n");

#if 0	//master mode test
	//for(i = 0; i < 10; i++){
	while(1) {
		i2c_master_write(txd, 8);
		delay_us(1000);
		
		i2c_master_read(rxd, 8);
		delay_us(1000);
		
//		i2c_master_sequencial_read(rxd, 8);
//		delay_us(1000);
		
//		i2c_master_current_read(rxd);
//		delay_us(1000);
	
		printf("\r\n%d, rxd : ", i);
		for(j = 0; j < 8; j++){
			printf("%02x, ", rxd[j]);
		}
		printf("\r\n%d, txd : ", i);
		for(j = 0; j < 8; j++){
			printf("%02x, ", txd[j]);
		}

#if 0
		printf("\r\nrxd : ");
		for(j = 0; j < 8; j++){
			printf("%02x, ", rxd[j]);
			//txd[j] = rxd[j] ^ 0x01;
		}
		
#else
		count++;
		for(j = 0; j < 8; j++){
			if ((txd[j] ^ 0x01) != rxd[j] ) {
				printf("\r\n Test Count : %d", count);
				printf("\r\n Test Delay : %d", Delay);
				printf("\r\n Test Speed : %0x", CLK_SPEED);
				return;
			}
		}
		delay_us(Delay);
#endif		
		delay_us(3000);
	}
#else	//slave mode test
	while(1){
		i2c_slave_read(rxd, 8);

		for(j = 0; j < 8; j++){
			data[j] = rxd[j] ^ 0x01;
		}

		i2c_slave_write(txd, 8);
		delay_us(1000);
		
//		i2c_slave_sequential_write(data, 8);
//		delay_us(1000);
		
//		i2c_slave_sequential_write(data, 1);
//		delay_us(1000);
		
		printf("\r\nrxd : ");
		for(j = 0; j < 8; j++){
			printf("%02x, ", rxd[j]);
			txd[j] = rxd[j] ^ 0x01;
		}
		
		printf("\r\ntxd : ");
		for(j = 0; j < 8; j++){
			printf("%02x, ", data[j]);
		}
		printf("\r\n");

		printf("SLAVE READ WRITE CYCLE DONE\r\n");
	}
#endif
	
	return;
}
