#include "CMSDK_CM3.h"


#include <stdio.h>
#include "uart_stdout.h"

//#include "config_id.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))
volatile int dma_done_irq_occurred;
volatile int dma_done_irq_expected;
volatile int dma_error_irq_occurred;
volatile int dma_error_irq_expected;

int  pl230_dma_detect(void);
int  ID_Check(const unsigned int id_array[], unsigned int offset);
void dma_memory_copy (unsigned int src, unsigned int dest, unsigned int size, unsigned int num);
void dma_data_struct_init(void);
void dma_pl230_init(void);
int  dma_simple_test(void);
int  dma_interrupt_test(void);
int  dma_event_test(void);

                              /* Maximum to 32 DMA channel */
#define MAX_NUM_OF_DMA_CHANNELS   32
                              /* SRAM in example system is 256K bytes */
#define RAM_ADDRESS_MAX       0x2003FFFF

typedef struct /* 4 words */
{
  volatile unsigned long SrcEndPointer;
  volatile unsigned long DestEndPointer;
  volatile unsigned long Control;
  volatile unsigned long unused;
} pl230_dma_channel_data;


typedef struct /* 8 words per channel */
{ /* only one channel in the example uDMA setup */
  volatile pl230_dma_channel_data Primary[MAX_NUM_OF_DMA_CHANNELS];
  volatile pl230_dma_channel_data Alternate[MAX_NUM_OF_DMA_CHANNELS];
} pl230_dma_data_structure;

pl230_dma_data_structure *dma_data;

volatile unsigned int source_data_array[4];  /* Data array for memory DMA test */
volatile unsigned int dest_data_array[4];    /* Data array for memory DMA test */


