/**
 * @file    barley_retno.h
 * @author  Embedded Team
 * @version V1.0.1
 * @date    24-April-2017
 * @brief   This file defines return-value for using BARLEY SDK.
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2016 Canvasbio <www.canvasbio.com>
 *
 */

#ifndef SRC_BARLEY_RETNO_H_
#define SRC_BARLEY_RETNO_H_
typedef enum{
	/**
	 * Enrollable index exceeded.
	 */
	BARLEY_ALL_INDICES_ARE_ALREADY_ENROLLED =-1,
	/**
	 * OK.
	 */
	BARLEY_OK = 0,

	/**
	 * One-time success during the enroll process.(But enroll process is not complete.)
	 */
	BARLEY_ENROLL_COUNT_INCREASED  = 1,

	/**
	 * The whole enroll process was successful.
	 */
	BARLEY_ENROLL_FINISHED = 2 ,

	/**
	 *The whole enroll process was successful.But the same fingerprint is already enrolled.
	 */
	BARLEY_ENROLL_FINISHED_BUT_MATCHED_IMG  ,
	/**
	 * Fail to enroll.
	 */
	BARLEY_ENROLL_FAIL,

	/**
	 * Invalid parameters.
	 */
	BARLEY_INVALID_PARAM,

	/**
	 * Success to verify fingerprint.
	 */
	BARLEY_VERIFY_SUCCESS,

	/**
	 * Fingerprint verify failed
	 */
	BARLEY_VERIFY_FAIL,

	/**
	 * Cancellation by user request.
	 */
	BARLEY_USER_CANCEL ,

	/**
	 * Reset complete.
	 */
	BARLEY_RESET_COMPLETE,

	/**
	 * Internal error.
	 */
	BARLEY_ERR_INTERNAL_ERROR,


	/**
	 * Invalid parameter.
	 */
	BARLEY_ERR_INVALID_PARAM,

	/**
	 * Sensor initialize fail.
	 */
	BARLEY_SENSOR_ERROR,

	/**
	 * SDK initialize fail.
	 */
	BARLEY_ERR_INIT_FAIL,

	/**
	 * Fingerprint image capture complete.
	 */
	BARLEY_IMG_CAPTURE_COMPLETE,

	/**
	 * Not supported function.
	 */
	BARLEY_NOT_SURPPORTED,

	/**
	 * Already enrolled finger index.
	 */
	BARLEY_ALREADY_ENROLLED_INDEX,
	BARLEY_ENROLL_FIRST,
	BARLEY_ENROLL_RETRY,
	BARLEY_TIMEOUT,
	BARLEY_ALL_SLOTS_IS_FULL,
	BARLEY_SLOT_IS_EMPTY,
	BARLEY_ENROLL_IS_BEGIN,
	BARLEY_INIT,
	BARLEY_TOUCH_ON_SENSOR,
	BARLEY_FINGERUP,
	BARLEY_FINGERDOWN,
	BARLEY_VERIFICATION_FAIL_OVER_TIME,
	BARLEY_PROCESS_FAIL,
	BARLEY_OUT_OF_MEMORY,
	BARLEY_FLASH_ERROR,
	BARLEY_INVALID_TEMPLATE,
	BARLEY_BAD_IMAGE,
	BARLEY_PROGRAM_BUG,
	BARLEY_ERROR_ON_ENROLL,
	BARLEY_ERROR_ON_VERIFY

}BARLEY_RETURN_CODE;

#endif /* SRC_BARLEY_RETNO_H_ */
/******************************** END OF FILE *********************************/
