/**
 * @file    barley_sdk.h
 * @author  Embedded Team
 * @version V1.0.1
 * @date    10-April-2017
 * @brief   This file provides the function to operate barley_sdk.
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2016 Canvasbio <www.canvasbio.com>
 *
 */

#ifndef __BARLEY_SDK_H_
#define __BARLEY_SDK_H_

#pragma pack(1)
#include <stdint.h>
#include <stddef.h>
#include "barley_retno.h"

#define SELF_TEST_ERROR_NONE    (0)
#define SELF_TEST_ERROR         (-1)

#define LIVE_CAPTURE        881126
#define NORMAL_CAPTURE      0

typedef struct {
    const char *ver;
    const char *sensor_name;
    unsigned char chip_id;
    int sensor_size;
    int result;
    int is_support;
    int mask_level;
    int sec_level;
    int learn_flag;
    int neighbor_cnt;
} CSInit_Result;

typedef struct {
    int mask_level;
    int sec_level;
    int learn_flag;
    int neighbour_cnt;
    int enroll_ip;
    int verify_ip;
    char sdk_ver[3];
    char algo_ver[3];
}FW_Params;

typedef struct {
  // Enroll result
  int result;
  // Enroll index
  int enroll_idx;
  // template size
  int store_size;
  // unused
  int proc_time;
  // unused
  int ip_count;
  // Max heap peak size
  int heap_size;
} CSEnroll_Result;

typedef struct {
  // Verify result
  int result;
  // extracted ip in captured image
  int ip_count;
  // stored template size
  unsigned int store_size;
  // extract feature time
  unsigned int extract_time;
  // verify time (Average)
  unsigned int verify_feature_time;
  // Unsued
  unsigned int total_verify_time;
  // Image capture time
  unsigned int capture_time;
  // Image read time
  unsigned int img_read_time;
  // Max heap peak size
  int max_heap;
  // Returned socres
  int scores[32];
} CSVerify_Result;

/**
 * @brief BarleyResult
 */
typedef union _BARLEY_RESULT{
	CSInit_Result init_result;
	CSEnroll_Result enroll_result;
	CSVerify_Result verify_result;
}BARLEY_RESULT;

/**
 * @brief Barley initialization parameters.
 */
typedef struct barley_init_param{
	int enroll_count;
	int max_tpl_count;
	int max_tpl_size;
	int learn_flag;
	BARLEY_RESULT* results;
    int enroll_ip;
    int verify_ip;
    int sec_lev;
    int ext;
}BARLEY_INIT_PARAM;

/**
 * @brief Barley fingerprint template data.
 * @param size template size.
 * @param pdata template data pointer.
 */
typedef struct _BALREY_TPL{
	unsigned int size;					/*	Template size */
	unsigned char* pdata;				/* 	Template data pointer */
}BARLEY_TEMPLATE_DATA;

/**
 * @brief The return code for Flash operation.
 */
typedef enum {
	 FLASH_OK = 0 ,
	 FLASH_ERROR_INVALID_PARAM,
	 FLASH_ERROR_WRITING_FAILED,
	 FLASH_ERROR_UNKNOWN
} FlashRetCode;

/**
 * @brief Barley SDK properties.
 */
typedef struct st_barley_info{
	const char* sdk_ver;					/*	SDK Version */
	const char* algo_ver;				/*	Algorithm Version */
	char chip_id;							/*	BTP Chip ID  */
}BARLEY_PROPERTIES;

/**
 * @brief Barley SDK properties.
 */
typedef struct st_barley_context{
	const char* sdk_ver;					/*	SDK Version */
	const char* algo_ver;				/*	Algorithm Version */
	char chip_id;							/*	BTP Chip ID  */
	void* fp_image;
}BARLEY_CONTEXT;


/**
 * @brief Sensor properties.
 */
typedef struct st_sensor_prop{
	int flag;
	//char chip_id;
	char agc_table[3];
	char detect_agc_table[3];
	char boost_level;
	char fp_thres;
	char fp_register[2];
	char fp_mode;
    char get_image;
}SENSOR_PROP;

/**
 * @brief The flags for sensor properties.
 */
typedef enum {
	SET_AGC_TABLE	 = 0x01,
	SET_DETECT_AGC_TABLE = 0x02,
	SET_BOOST_LEVEL = 0x04,
	SET_FP_THRESHOLD	=0x08,
	SET_FP_MODE	= 0x10,
	SET_FP_REGISTER = 0x20,
	SET_CHIP_ID,
}SensorProp_flag;

/**
 * @brief Template properties.
 */
typedef struct st_tpl_prop{
	int flag;
	char max_tpl_count;
	char max_enroll_count;
	char max_tpl_size;
}TEMPLATE_PROP;

/**
 * @brief The flags for template properties.
 */
typedef enum{
	SET_MAX_TPL_COUNT	=	0x01,
	SET_MAX_ENROLL_COUNT	 = 0x02,
	SET_MAX_TPL_SIZE = 0x04,
}TemplateProp_flag;

/**
 * @brief Algorithm properties.
 */
typedef struct st_algo_prop{
	int flag;
	char mask_level;
	char learn_flag;
   int enroll_ip;
   int verify_ip;
   int sec_level;
   int neighbour_cnt;
}ALGO_PROP;

/**
 * @brief The flags for Algorithm properties.
 */
typedef enum {
	SET_MASK_LEVEL 	= 0x01,
	SET_LEARN_FLAG	= 0x02,
	SET_ENROLL_IP	=	0x04,
	SET_VERIFY_IP	=	0x08,
	SET_SEC_LEVEL	=	0x10,
	SET_NEIGHBOUR = 0x20,
}AlgoProp_flag;


/**
 * @brief Register set for pixel test.
 */

typedef struct st_regi_buf{
    unsigned char buf_size;
    unsigned char* buf;
}REG_BUF_SET;

typedef struct st_pixel_test{
    REG_BUF_SET* norm;
    REG_BUF_SET* invert;
    REG_BUF_SET* black;
    REG_BUF_SET* white;
}PIXEL_TEST_REG;

/**
 * @brief Structure of platform APIs.
 */
typedef struct _PLATFORM_API
{
	/* Memory */
	void* (*malloc) (size_t size);									/* Memory allocation */
	void* (*realloc)(void* ptr, size_t size);						/* Memory re-allocation */
	void* (*calloc) (size_t nmemb, size_t sizeb);					/* Memory c-allocation */
	void  (*free)   (void* ptr);									/* Memory free */
	void* (*memset) (void* s, int c, size_t n);					/* Memory set */
	void* (*memcpy) (void *dest, const void *src, size_t n);	/* Memory copy */

	/* BTP */
	int   (*btp_irq)(int timeout);									/* Check irq pin */
	void  (*btp_reset) (void);											/* Reset sensor */

	/* SPI */
	void  (*spi_write)(unsigned char *pBuf, unsigned int len);	/* SPI write */
	void  (*spi_read)(unsigned char *pBuf, unsigned int len);	/* SPI read */
	void  (*spi_write_read)(unsigned char *wBuf, unsigned int wLen, unsigned char *rBuf, unsigned int rLen);/* SPI write and read */

	/* Etc. */
	void   (*delay)  (unsigned int milliseconds);					/* Delay */

	/* Template */
	int (*tpl_save)(int idx, int size , unsigned char* tpl);	/* Save template */
	void* (*tpl_load)(int idx);										/* Load template */
	int (*tpl_delete)(int idx);										/* Delete template */

	/* Debug */
	void (*debug) (const char *fmt, ... );							/* Print debug message */
    unsigned int (*get_tick)(void);
} PLATFORM_API;


#if 1
/**
 *	@brief	Initialize Balrey SDK.
 *	@return SDK Properties.
 */
BARLEY_PROPERTIES BARLEY_Init(BARLEY_INIT_PARAM param , PLATFORM_API api);

/**
 *	@brief De-Initialize Barley SDK.
 *	@return BARLEY_OK if this operation has been successfully executed. , error code otherwise.
 *	-
 */
int BARLEY_DeInit(void);

/**
 *	@brief Enroll Fingerprint.
 *	@param idx To be stored index of fingerprint.
 *	@return BARLEY_ENROLL_COUNT_INCREASED & BARLEY_ENROLL_FINISHED if this operation has been successfully executed, error code otherwise.
 *			BARLEY_ENROLL_COUNT_INCREASED enroll operation has been successfully executed, but not has been finished enroll operation.
 *			If the number of successes exceeds MAX_ENROLL_COUNT, return BARLEY_ENROLL_FINISHED.
 *	- BARLEY_INVALID_PARAM
 *	- BARLEY_ALREADY_ENROLLED_INDEX
 *	- BARLEY_ERR_INIT_FAIL
 *	- BARLEY_ENROLL_FAIL
 *	- BARLEY_USER_CANCEL
 *
 */
int BARLEY_Enroll(int idx);

/**
 * @brief Verify Fingerprint.
 *	@return BARLEY_VERIFY_SUCCESS if this operation has been successfully executed, error code otherwise.
 *	- BARLEY_VERIFY_FAIL
 */
int BARLEY_Verify(void);
#endif

/**
 *	@brief Enroll Fingerprint with image.
 *	@param idx To be stored index of fingerprint.
 *	@param image To be used
 *	@return BARLEY_ENROLL_COUNT_INCREASED & BARLEY_ENROLL_FINISHED if this operation has been successfully executed, error code otherwise.
 *			BARLEY_ENROLL_COUNT_INCREASED enroll operation has been successfully executed, but not has been finished enroll operation.
 *			If the number of successes exceeds MAX_ENROLL_COUNT, return BARLEY_ENROLL_FINISHED.
 *	- BARLEY_INVALID_PARAM
 *	- BARLEY_ALREADY_ENROLLED_INDEX
 *	- BARLEY_ERR_INIT_FAIL
 *	- BARLEY_ENROLL_FAIL
 *	- BARLEY_USER_CANCEL
 *
 */
int BARLEY_Enroll_with_image(int idx,unsigned char* image);

/**
 * @brief Verify Fingerprint.
 *	@param image To be used
 *	@return BARLEY_VERIFY_SUCCESS if this operation has been successfully executed, error code otherwise.
 *	- BARLEY_VERIFY_FAIL
 */
int BARLEY_Verify_with_image(unsigned char* image);

/**
 *	@brief Wait for finger up on sensor.
 *	@return BARLEY_VERIFY_SUCCESS if this operation has been successfully executed, error code otherwise.
 *	- BARLEY_USER_CANCEL
 *	- BARLEY_VERIFY_FAIL
 */
int BARLEY_WaitingForFingerup(void);

/**
 *	@brief Wait for finger up on sensor.
 *	@return BARLEY_VERIFY_SUCCESS if this operation has been successfully executed, error code otherwise.
 *	- BARLEY_USER_CANCEL
 *	- BARLEY_VERIFY_FAIL
 */
int BARLEY_WaitingForFingerDown(void);


/**
 * @brief Get current properties about template.
 * @param tpl_prop Template properties to be set.
 *
 */
int BARLEY_SetTemplateProperties(TEMPLATE_PROP* tpl_prop);

/**
 * @brief Get current properties about template.
 * @return Current properties about template.
 */
TEMPLATE_PROP BARLEY_GetTemplateProperties(void);


/**
 * @brief Get current properties about sensor.
 * @return Current properties about sensor.
 */
void BARLEY_GetSensorProperties(SENSOR_PROP* sensor_prop);

/**
 * @brief Get current properties about sensor.
 * @param tpl_prop Sensor properties to be set.
 *
 */
int BARLEY_SetSensorProperties(SENSOR_PROP* sensor_prop);

/**
 * @brief Get current properties about algorithm.
 * @param tpl_prop Algorithm properties to be set.
 *
 */
ALGO_PROP* BARLEY_GetAlgorithmProperties(void);

/**
 * @brief Get current properties about algorithm.
 * @param tpl_prop Algorithm properties to be set.
 */
int BARLEY_SetAlgorithmProperties(ALGO_PROP* algo_prop);


/**
 * @brief Return the fingerprint sensor id.
 */
unsigned char BARLEY_GetSensorId(void);

/**
 * @brief Return the fingerprint sensor size.
 */
int BARLEY_GetSensorSize(void);

/**
 * @brief Return a table of stored finger print template.
 */
int BARLEY_GetNextFingerIndex(void);

/**
 * @brief Start image capture.
 */
int BARLEY_CaptureImage(unsigned char* buf , int buf_size);

/**
 * @brief Check the target finger index is enrolled in target template store.
 * @param idx Finger index to check.
 * @return TRUE(1) or False(0)
 */
int BARLEY_IsEnrolled(int idx);

/**
 * @brief Erase a fingerprint's template file.
 * @param idx Index to be deleted fingerprint.
 *	@return BARLEY_OK if this operation has been successfully executed, error code otherwise.
 */
int BARLEY_Erase(int idx);

/**
 * @brief Cancel the operation(Enroll or Verify).
 *	@return BARLEY_VERIFY_SUCCESS if this operation has been successfully executed, error code otherwise.
 */
int BARLEY_UserCancel(void);

/**
 * @brief Set sensor sleepmode.
 */
int BARLEY_sleep(void);

/***
 * New Interface
 */
BARLEY_CONTEXT* BARLEYAPI_init(BARLEY_INIT_PARAM param , PLATFORM_API api);
int BARLEYAPI_deinit(BARLEY_CONTEXT* context);
int BARLEYAPI_start_capture(BARLEY_CONTEXT* context);
int BARLEYAPI_enroll(BARLEY_CONTEXT* context , int idx);
int BARLEYAPI_verify(BARLEY_CONTEXT* context , int idx);
int BARLEYAPI_restore(void *ec);


/**
 * @brief Start self test.
 */
int BARLEY_StartSelfTest(unsigned char* buf, int buf_size);
int BARLEY_VersionCheck(unsigned char* version);
int BARLEY_DefaultSettingCheck(ALGO_PROP* algo_prop, TEMPLATE_PROP* tpl_prop);
int BARLEY_ChipIDCheck(void);
int BARLEY_IrqTest(void);
int BARLEY_PixelTest(PIXEL_TEST_REG* reg_set, int buf_size);
int BARLEY_ZoneTest(PIXEL_TEST_REG* reg_set, int buf_size);
int BARLEY_EndSelfTest(void);


#endif//__BARLEY_SDK_H_
/******************************** END OF FILE *********************************/
