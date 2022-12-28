/**
  ******************************************************************************
  * @file    hardware_rng.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    14-April-2017
  * @brief   mbedtls alternate entropy data function.
  *          the mbedtls_hardware_poll() is customized to use the STM32 RNG
  *          to generate random data, required for TLS encryption algorithms.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Apache 2.0 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  * https://opensource.org/licenses/Apache-2.0
  *
  ******************************************************************************
  */

#include "mbedtls_config.h"

//#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT

//#include "main.h"
//#include "string.h"
//#include "stm32f4xx_hal.h"
#include "mbedtls/entropy_poll.h"

//extern RNG_HandleTypeDef hrng;

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



int mbedtls_hardware_poll( void *Data, unsigned char *Output, size_t Len, size_t *oLen )
{
  uint32_t index;
  uint32_t randomValue;
  uint32_t rnd_temp[8]; //= { 0xd0191141, 0xc1f9f2ab, 0x5398e383, 0xd4fd1efb, 0x99e67d44, 0xa4bca65b, 0x6acee415, 0xde1991c2,
						//	0x6549f913, 0xaa09d09f, 0xb9c3c744, 0xc5b3d3da, 0x3d28ab4f, 0x062e8fa2, 0x4f5fdff9, 0xff6ba643,
						//	0xcd2b9598, 0xdfede83d, 0xdc098ada, 0xa6d266a3, 0x22871be5, 0x33a2b4b4, 0x6ff31427, 0xc584313b,
						//	0xa2e59244, 0xd2ffbc33, 0x0dbae64f, 0xc7e9dbf5, 0xfe70cb9c, 0x01fa6352, 0xf3005a6e, 0x108209a1 }; 

 // printf("\r\n ***************************** Len : %d, oLen : %d********************************", Len, *oLen);

  for (index = 0; index < Len/4; index++)
  {
    #if 1 //data_fix debug
	 //RNG_ReadRandomNumber(rnd_temp);
    {
     //  printf("\r\nrandomValue: %08x", rnd_temp[2]);
      *oLen += 4;
      memset(&(Output[index * 4]), (int)randomValue, 4);
		//memset(&(Output[index * 4]), (int)rnd_temp[2], 4);
    }
	#else
	 RNG_ReadRandomNumber(rnd_temp);
    {
     //  printf("\r\nrandomValue: %08x", rnd_temp[2]);
      *oLen += 4;
      //memset(&(Output[index * 4]), (int)randomValue, 4);
		memset(&(Output[index * 4]), (int)rnd_temp[2], 4);
    }
	#endif
//    else
//    {
//      Error_Handler();
//    }
  }
  
//  printf("\r\n ***************************** Len : %d, oLen : %d********************************", Len, *oLen);

  return 0;
}


