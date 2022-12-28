/*
 * barley_conf.h
 *
 *  Created on: 2017. 11. 17.
 *  Author: Embedded Team
 *
 *  Configuration for 'BARLEY'
 *
 */

#ifndef BARLEY_CONF_H_
#define BARLEY_CONF_H_
#include <stdbool.h>
//#include "barley_internal.h"
/* Includes ------------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
#define BARLEY_SYSTEM_CLOCK_MHz		40

#define BARLEY_PLL_MUL				((BARLEY_SYSTEM_CLOCK_MHz*1000000)/32768)		
/* Private function prototypes -----------------------------------------------*/

/* public function prototypes -----------------------------------------------*/

/**
 * @brief Set default enroll count.
 */
#define BARLEY_INIT_ENROLL_COUNT			8

/**
 * @brief Set default max template count.
 */
#define BARLEY_INIT_MAX_TPL_COUNT			10/*(256/BARLEY_INIT_MAX_TPL_SIZE)*/


/**
 * @brief Set default max template size.( Kbytes )
 */
#define BARLEY_INIT_MAX_TPL_SIZE			16
//#define BARLEY_INIT_MAX_TPL_SIZE			12

/**
 * @brief Set default learning flag. ( '1' is learning)
 */
#define BARLEY_INIT_LEARNING				1

/**
 * @brief Set default enroll ip count
 */
#define BARLEY_INIT_ENROLL_IP				100

/**
 * @brief Set default verify ip count
 */
#define BARLEY_INIT_VERIFY_IP				70

/**
 * @brief Set default security level
 */
#define BARLEY_INIT_SEC_LEVEL 					2


#define TIMEOUT_ENROLL_SEC 10
#define TIMEOUT_VERIFY_SEC 10
#define TIMEOUT_WAIT_FINGER_SEC 10
#define TIMEOUT_DELETE_SEC 5

#define VERIFY_MAX_TRY_COUNT 3
#define DELETE_MAX_TRY_COUNT 1

#define BARLEY_LIMIT_TEMPLATE_COUNT			16 // (256Kb / 16Kb)

#define ERR_ENROLL 1
#define ERR_VERIFY 2


//extern barley_config_t barley_config;


#endif /* BARLEY_CONF_H_ */
