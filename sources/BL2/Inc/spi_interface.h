#ifndef SPI_INTERFACE
#define SPI_INTERFACE

#include "CMSDK_CM3.h"
//#include "ARMCM3.h"
#include "stdio.h"
#include "config.h"

#include "d6_config.h"

void init_master_spi(void);
void init_slave_spi(void);
void write_spi(uint8_t *buffer, unsigned int size);
//void read_spi(uint8_t * buffer, unsigned int size);
//int read_spi_data(uint8_t *buffer, unsigned int size);
void read_spi(unsigned char * buffer, int size);
int read_spi_data(unsigned char *buffer, int size);
void spi_read_write(uint8_t *txd, unsigned int tSize, uint8_t *rxd, unsigned int rSize);

void ssp_write(uint8_t * buffer, unsigned int size);
void ssp_read_write(uint8_t *txd, unsigned int tSize, uint8_t *rxd, unsigned int rSize);

void write_spi_slave(unsigned char * buffer, int size);

#endif
