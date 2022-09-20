#include "CM3_dma.h"
#define EXPECTED_BE             0


void delay(void){
	
	delay_us(1);
}

int dma_function_test(){
	int result=0;

	// Test banner message and revision number
	printf("\r\nCortex Microcontroller System Design Kit - DMA Test - revision $Revision: 371321 $\n");


	if (pl230_dma_detect()!=0) {
	return 0; /* Quit test if DMA is not present */
	}

	dma_done_irq_expected = 0;
	dma_done_irq_occurred = 0;
	dma_error_irq_expected = 0;
	dma_error_irq_occurred = 0;
	dma_data_struct_init();
	dma_pl230_init();

	result += dma_simple_test();
//	result += dma_interrupt_test();
//	result += dma_event_test();
	if (result==0) {
	printf ("\r\n** TEST PASSED **\n");
	} else {
	printf ("\r\n** TEST FAILED **, Error code = (0x%x)\n", result);
	}

	return 0;
}

/* --------------------------------------------------------------- */
/*  Detect if DMA controller is present or not                     */
/* --------------------------------------------------------------- */

int pl230_dma_detect(void)
{
  int result;

  unsigned const int pl230_id[12] = {
                                 0x30, 0xB2, 0x0B, 0x00,
                                 0x0D, 0xF0, 0x05, 0xB1};
  puts("\r\nDetect if DMA controller is present...");
  result = ID_Check(&pl230_id[0]   , CMSDK_PL230_BASE);
  if (result!=0) {
    puts("\r\n** TEST SKIPPED ** DMA controller is not present.\n");
    UartEndSimulation();
  }
  return(result);
}

int ID_Check(const unsigned int id_array[], unsigned int offset)
{
int i;
unsigned long expected_val, actual_val;
unsigned long compare_mask;
int           mismatch = 0;
unsigned long test_addr;

  /* Check the peripheral ID and component ID */
  for (i=0;i<8;i++) {
    test_addr = offset + 4*i + 0xFE0;
    expected_val = id_array[i];
    actual_val   = HW32_REG(test_addr);

    /* create mask to ignore version numbers */
    if (i==2) { compare_mask = 0xF0;}  // mask out version field
    else      { compare_mask = 0x00;}  // compare whole value

    if ((expected_val & (~compare_mask)) != (actual_val & (~compare_mask))) {
      printf ("\r\nDifference found: %x, expected %x, actual %x\n", test_addr, expected_val, actual_val);
      mismatch++;
      }

    } // end_for
return (mismatch);
}

/* --------------------------------------------------------------- */
/*  Initialize DMA data structure                                  */
/* --------------------------------------------------------------- */
void dma_data_struct_init(void)
{
  int          i;   /* loop counter */
  unsigned int ptr;

  int          ch_num;         /* number of channels */
  unsigned int blksize;        /* Size of DMA data structure in bytes */
  unsigned int blkmask;        /* address mask */


  ch_num  = (((CMSDK_DMA->DMA_STATUS) >> 16) & 0x1F)+1;
  blksize = ch_num * 32;
  if      (ch_num > 16) blkmask = 0x3FF; /* 17 to 32 */
  else if (ch_num > 8)  blkmask = 0x1FF; /*  9 to 16 */
  else if (ch_num > 4)  blkmask = 0x0FF; /*  5 to 8 */
  else if (ch_num > 2)  blkmask = 0x07F; /*  3 to 4 */
  else if (ch_num > 1)  blkmask = 0x07F; /*       2 */
  else                  blkmask = 0x03F; /*       1 */

  /* Create DMA data structure in RAM after stack
  In the linker script, a 1KB memory stack above stack is reserved
  so we can use this space for putting the DMA data structure.
  */

  ptr     = HW32_REG(0);                     /* Read Top of Stack */

  /* the DMA data structure must be aligned to the size of the data structure */
  if ((ptr & blkmask) != 0x0)
    ptr     = (ptr + blksize) & ~blkmask;

  if ((ptr + blksize) > (RAM_ADDRESS_MAX + 1)) {
    printf ("\r\nERROR : Not enough RAM space for DMA data structure.");
    UartEndSimulation();
    }

  /* Set pointer to the reserved space */
  dma_data = (pl230_dma_data_structure *) ptr;
  ptr = (unsigned int) &dma_data->Primary->SrcEndPointer;

  printf ("\r\ndma structure block address = %x\n", ptr);

  for (i=0; i<1; i++) {
    dma_data->Primary->SrcEndPointer  = 0;
    dma_data->Primary->DestEndPointer = 0;
    dma_data->Primary->Control        = 0;
    dma_data->Alternate->SrcEndPointer  = 0;
    dma_data->Alternate->DestEndPointer = 0;
    dma_data->Alternate->Control        = 0;
    }

  return;
}

/* --------------------------------------------------------------- */
/*  Initialize DMA PL230                                           */
/* --------------------------------------------------------------- */
void dma_pl230_init(void)
{
  unsigned int current_state;
  printf ("\r\nInitialize PL230");
  current_state = CMSDK_DMA->DMA_STATUS;
  printf ("\r\n- # of channels allowed : %d\n",(((current_state) >> 16) & 0x1F)+1);
  /* Debugging printfs: */
  /*printf ("- Current status        : %x\n",(((current_state) >> 4)  & 0xF));*/
  /*printf ("- Current master enable : %x\n",(((current_state) >> 0)  & 0x1));*/

  /* Wait until current DMA complete */
  current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
  if (!((current_state==0) || (current_state==0x8) || (current_state==0x9))) {
    printf ("\r\n- wait for DMA IDLE/STALLED/DONE");
    current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
    printf ("\r\n- Current status        : %x\n",(((current_state) >> 4)  & 0xF));

    }
  while (!((current_state==0) || (current_state==0x8) || (current_state==0x9))){
    /* Wait if not IDLE/STALLED/DONE */
    current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
    printf ("\r\n- Current status        : %x\n",(((current_state) >> 4)  & 0xF));
    }
  CMSDK_DMA->DMA_CFG = 0; /* Disable DMA controller for initialization */
  CMSDK_DMA->CTRL_BASE_PTR = (unsigned int) &dma_data->Primary->SrcEndPointer;
                           /* Set DMA data structure address */
  CMSDK_DMA->CHNL_ENABLE_CLR = 0xFFFFFFFF; /* Disable all channels */
  CMSDK_DMA->CHNL_ENABLE_SET = (1<<0); /* Enable channel 0 */
  CMSDK_DMA->DMA_CFG = 1;              /* Enable DMA controller */

  return;
}

/* --------------------------------------------------------------- */
/*  DMA memory copy                                                */
/* --------------------------------------------------------------- */
void dma_memory_copy (unsigned int src, unsigned int dest, unsigned int size, unsigned int num)
{
  unsigned long src_end_pointer =  src + ((1<<size)*(num-1));
  unsigned long dst_end_pointer = dest + ((1<<size)*(num-1));
  unsigned long control         = (size << 30) |  /* dst_inc */
                                  (size << 28) |  /* dst_size */
                                  (size << 26) |  /* src_inc */
                                  (size << 24) |  /* src_size */
                                  (size << 21) |  /* dst_prot_ctrl - HPROT[3:1] */
                                  (size << 18) |  /* src_prot_ctrl - HPROT[3:1] */
                                  (0    << 14) |  /* R_power */
                                  ((num-1)<< 4) | /* n_minus_1 */
                                  (0    <<  3) |  /* next_useburst */
                                  (2    <<  0) ;  /* cycle_ctrl - auto */

  /* By default the PL230 is little-endian; if the processor is configured
   * big-endian then the configuration data that is written to memory must be
   * byte-swapped before being written.  This is also true if the processor is
   * little-endian and the PL230 is big-endian.
   * Remove the __REV usage if the processor and PL230 are configured with the
   * same endianness
   * */
  dma_data->Primary->SrcEndPointer  = (EXPECTED_BE) ? __REV(src_end_pointer) : (src_end_pointer);
  dma_data->Primary->DestEndPointer = (EXPECTED_BE) ? __REV(dst_end_pointer) : (dst_end_pointer);
  dma_data->Primary->Control        = (EXPECTED_BE) ? __REV(control        ) : (control        );
  /* Debugging printfs: */
  /*printf ("SrcEndPointer  = %x\n", dma_data->Primary->SrcEndPointer);*/
  /*printf ("DestEndPointer = %x\n", dma_data->Primary->DestEndPointer);*/

  CMSDK_DMA->CHNL_ENABLE_SET = (1<<0); /* Enable channel 0 */
  CMSDK_DMA->CHNL_SW_REQUEST = (1<<0); /* request channel 0 DMA */

  return;
}

/* --------------------------------------------------------------- */
/*  Simple software DMA test                                       */
/* --------------------------------------------------------------- */
int dma_simple_test(void)
{
  int return_val=0;
  int err_code=0;
  int i;
  unsigned int current_state;


  printf("\r\nuDMA simple test");
  CMSDK_DMA->CHNL_ENABLE_SET = (1<<0); /* Enable channel 0 */

  /* setup data for DMA */
  for (i=0;i<4;i++) {
    source_data_array[i] = i+1;
    dest_data_array[i]   = 0;
  }

  dma_memory_copy ((unsigned int) &source_data_array[0],(unsigned int) &dest_data_array[0], 2, 4);
  do { /* Wait until PL230 DMA controller return to idle state */
    current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
  } while (current_state!=0);

  for (i=0;i<4;i++) {
    /* Debugging printf: */
    /*printf (" - dest[i] = %x\n", dest_data_array[i]);*/
    if (dest_data_array[i]!= source_data_array[i]){
      printf ("\r\nERROR:dest_data_array[%d], expected %x, actual %x\n", i, i, dest_data_array[i]);
      err_code |= (1<<i);
    }
  }

  /* Generate return value */
  if (err_code != 0) {
    printf ("\r\nERROR : simple DMA failed (0x%x)\n", err_code);
    return_val=1;
  } else {
    printf ("\r\n-Passed");
  }

  return(return_val);
}
///* --------------------------------------------------------------- */
///*  Simple DMA interrupt test                                      */
///* --------------------------------------------------------------- */
//int dma_interrupt_test(void)
//{
//  int return_val=0;
//  int err_code=0;
//  int i;
//  unsigned int current_state;


//  puts("DMA interrupt test");
//  puts("- DMA done");

//  CMSDK_DMA->CHNL_ENABLE_SET = (1<<0); /* Enable channel 0 */

//  /* setup data for DMA */
//  for (i=0;i<4;i++) {
//    source_data_array[i] = i;
//    dest_data_array[i]   = 0;
//  }

//  dma_done_irq_expected = 1;
//  dma_done_irq_occurred = 0;
//  NVIC_ClearPendingIRQ(DMA_IRQn);
//  NVIC_EnableIRQ(DMA_IRQn);

//  dma_memory_copy ((unsigned int) &source_data_array[0],(unsigned int) &dest_data_array[0], 2, 4);
//  delay();
//  /* Can't guarantee that there is sleep support, so use a polling loop */
//  do { /* Wait until PL230 DMA controller return to idle state */
//    current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
//	  printf("\r\n current_state = %d", current_state);
//  } while (current_state!=0);

//  for (i=0;i<4;i++) {
//    /* Debugging printf: */
//    /*printf (" - dest[i] = %x\n", dest_data_array[i]);*/
//    if (dest_data_array[i]!= i){
//      printf ("ERROR:dest_data_array[%d], expected %x, actual %x\n", i, i, dest_data_array[i]);
//      err_code |= (1<<i);
//    }
//  }

//  if (dma_done_irq_occurred==0){
//    puts ("ERROR: DMA done IRQ missing");
//    err_code |= (1<<4);
//  }

//  puts("- DMA err");
//  CMSDK_DMA->CHNL_ENABLE_SET = (1<<0); /* Enable channel 0 */

//  /* setup data for DMA */
//  for (i=0;i<4;i++) {
//    source_data_array[i] = i;
//    dest_data_array[i]   = 0;
//  }

//  dma_error_irq_expected = 1;
//  dma_error_irq_occurred = 0;
//  NVIC_ClearPendingIRQ(DMA_IRQn);
//  NVIC_EnableIRQ(DMA_IRQn);

//  /* Generate DMA transfer to invalid memory location */
//  dma_memory_copy ((unsigned int) &source_data_array[0],0x1F000000, 2, 4);
//  delay();
//  /* Can't guarantee that there is sleep support, so use a polling loop */
//  do { /* Wait until PL230 DMA controller return to idle state */
//    current_state = (CMSDK_DMA->DMA_STATUS >> 4)  & 0xF;
//  } while (current_state!=0);

//  if (dma_error_irq_occurred==0){
//    puts ("ERROR: DMA err IRQ missing");
//    err_code |= (1<<5);
//  }


//  /* Clear up */
//  dma_done_irq_expected = 0;
//  dma_done_irq_occurred = 0;
//  dma_error_irq_expected = 0;
//  dma_error_irq_occurred = 0;
//  NVIC_DisableIRQ(DMA_IRQn);

//  /* Generate return value */
//  if (err_code != 0) {
//    printf ("ERROR : DMA done interrupt failed (0x%x)\n", err_code);
//    return_val=1;
//  } else {
//    puts ("-Passed");
//  }

//  return(return_val);
//}

