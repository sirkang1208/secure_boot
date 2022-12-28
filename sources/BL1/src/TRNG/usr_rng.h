/************************** (C) COPYRIGHT 2020 Oanny's Roman *****************
* File Name       : 
* Author            : Oanny
* Version          : Ver 0.1
* Date               : 2020/03/11
* Description    : 
*
* Revision History
* Date          Version       Name   Description
* 2020/03/11  	0.1          Oanny   Proto.
*******************************************************************************/
#ifndef __USR_RNG_H
#define __USR_RNG_H

//#include "usr_system.h"

#define TRNGSIZE 32
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
typedef struct
{
  __IO  uint32_t  RNG_OPMODE;     /* RNG  register,  Address offset : 0x008 */
				uint32_t  RESERVED0[20];
  __IO  uint32_t  RNG_ACCESS;     /* RNG  register,  Address offset : 0x05C */
				uint32_t  RESERVED1[8];	
  __IO  uint32_t  RNG_RST;        /* RNG  register,  Address offset : 0x080 */
  __IO  uint32_t  RNG_256;        /* RNG  register,  Address offset : 0x084 */
  __IO  uint32_t  RNG_XOR;        /* RNG  register,  Address offset : 0x088 */
  __IO  uint32_t  RNG_REQ;        /* RNG  register,  Address offset : 0x08c */
  __IO  uint32_t  RNG_DONE;       /* RNG  register,  Address offset : 0x090 */
				uint32_t  RESERVED2[987];
  __IO  uint32_t  RNG_BUFEN;      /* RNG  register,  Address offset : 0x1000 */
}RNG_TypeDef;

#define RCM3_BASE 									(0x00100000UL)
#define RNG                         ((RNG_TypeDef *) (RCM3_BASE + 0x08UL))

#define RNG_DR                      ((volatile unsigned int *) (RCM3_BASE + 0x300UL))
	
/* Private functions ---------------------------------------------------------*/
uint32_t Get_TRNG(uint32_t *chpData, uint16_t nhDatalen);

#endif 
/******************* (C) COPYRIGHT 2020 Oanny's Roman *****END OF FILE****/
