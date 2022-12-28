#include <stdio.h>

#include "barley_sdk.h"
#include "barley_retno.h"
#include "barley_def.h"

#include "CMSDK_CM3.h"

#define DEBUG_MODE 					0
#define BARLEY_INIT_MAX_TPL_COUNT	10/*(256/BARLEY_INIT_MAX_TPL_SIZE)*/

extern void paint_finger(void);

int lib_fp_enrolled_count()
{
    int enrolled_count = 0;
    int i=0;

    for(i=0; i<BARLEY_INIT_MAX_TPL_COUNT; i++)
    {
    	if(BARLEY_IsEnrolled(i)!=BARLEY_OK)
    	{
    		enrolled_count ++;
    	}
    }
	
#if DEBUG_MODE
    printf("\r\n[CB_LOG_DBG] [%s] enrolled_count %d\r\n",__func__, enrolled_count);
#endif
	
    return enrolled_count;
}

char* ct_modeStr(BARLEY_Mode mode)
{
    switch(mode)
    {
    case MODE_STANDBY:
        return "MODE_STANDBY";

    case MODE_ENROL:
        return "MODE_ENROL";

    case MODE_VERIFY:
        return "MODE_VERIFY";

    case MODE_RESET:
        return "MODE_RESET";

    case MODE_CAPTURE_IMG:
        return "MODE_CAPTURE_IMG";

    case MODE_PWR_ON_STATE:
        return "MODE_PWR_ON_STATE";

    case MODE_POWER_OFF:
        return "MODE_POWER_OFF";

    case MODE_WAIT_FP_UP:
        return "MODE_WAIT_FP_UP";

    case MODE_5VIN:
        return "MODE_5VIN";

    default:
        return "Mode error!!";
    }
}

extern char irq_res; // temp value for applye to wfi test 20210709

int lib_fp_enroll(BARLEY_CONTEXT * context, int i){
	int index = 0;
	int barley_ret = -1;
	int result = 0;
	
#if DEBUG_MODE
	printf("[CB_LOG_INFO] ***********************************\r\n");
	printf("[CB_LOG_INFO]           start		%s\r\n", __func__);
	printf("[CB_LOG_INFO] ***********************************\r\n");
#endif
	
	if(i == 0){
//		printf("\r\nfinger cr before\r\n");
//		printf("\r\n");	__heapstats((__heapprt)fprintf,stderr);
//		printf("\r\n");__heapvalid((__heapprt) fprintf, stderr, 1);
		index = BARLEY_GetNextFingerIndex();
		printf("BARLEY_GetNextFingerIndex index : %d \r\n", index);
//		printf("\r\nfinger cr before\r\n");
//		printf("\r\n");	__heapstats((__heapprt)fprintf,stderr);
//		printf("\r\n");__heapvalid((__heapprt) fprintf, stderr, 1);
	}
	
	if(index < 0){
		// All indices are already enrooled.
		result = EVENT_ENROL_FULL;
		goto FINISH;
	}
	
	irq_res = 0; // temp value for applye to wfi test 20210709
	barley_ret = BARLEY_WaitingForFingerup();
	printf("BARLEY_WaitingForFingerup barley_ret : %d \r\n", barley_ret);
	
	if(barley_ret != BARLEY_OK){
		// Handle Error
		result = EVENT_UNKNOWN_ERROR;
		goto FINISH;
	}
	
	irq_res = 1; // temp value for applye to wfi test 20210709
	barley_ret = BARLEYAPI_start_capture(context);
	printf("BARLEYAPI_start_capture barley_ret : %d \r\n", barley_ret);

	barley_ret = BARLEYAPI_enroll(context, index);
	printf("BARLEYAPI_enroll barley_ret : %d \r\n", barley_ret);
	
	switch(barley_ret){
    case BARLEY_ENROLL_COUNT_INCREASED:
        result = EVENT_ENROL_SUCCESS;
        break;
    case BARLEY_ENROLL_FINISHED:
        result = EVENT_ENROL_COMPLETE;
        break;
    case BARLEY_USER_CANCEL:
        result = EVENT_USER_CANCEL;
        break;
    case BARLEY_TIMEOUT:
        result = EVENT_SENSOR_TIME_OUT;
        break;
    case BARLEY_ALL_SLOTS_IS_FULL:
        result = EVENT_ENROL_FULL;
        break;
    default:
        result = EVENT_UNKNOWN_ERROR;
        break;
    }
	
FINISH:
#if DEBUG_MODE
    printf("result %d\r\n", result);
	printf("index %d\r\n", index);
	
    printf("[CB_LOG_INFO] **************************************\r\n");
    printf("[CB_LOG_INFO]          finish      %s\r\n",__func__);
    printf("[CB_LOG_INFO] **************************************\r\n");
#endif

    return result;
}



int barley_verify(BARLEY_CONTEXT * context){
	int barley_ret = -1;
	int result = 0;
	static int verify_fail_count = 0 ;
	
	printf("\r\nbarley verify test \r\n");
	
	barley_ret = BARLEY_WaitingForFingerup();
	printf("BARLEY_WaitingForFingerup barley_ret : %d \r\n", barley_ret);
	
	barley_ret = BARLEYAPI_start_capture(context);
	printf("BARLEYAPI_start_capture barley_ret : %d \r\n", barley_ret);
	
	barley_ret = BARLEYAPI_verify(context, -1);
	printf("BARLEYAPI_verify barley_ret : %d \r\n", barley_ret);

    printf("[CB_LOG_INFO], barley_ret %d\r\n", barley_ret);

    switch (barley_ret){

    case BARLEY_VERIFY_FAIL:
        result = EVENT_VERIFY_FAIL;
        verify_fail_count++;
        break;
    case BARLEY_VERIFY_SUCCESS:{
        result = EVENT_VERIFY_SUCCESS;
        verify_fail_count = 0 ;
        break;
    }
    case BARLEY_USER_CANCEL:
        result = EVENT_USER_CANCEL;
        break;
    default:
        result = EVENT_UNKNOWN_ERROR;
        break;
    }

    if(  verify_fail_count > MAX_OF_ATTEMPTS){
        result  = EVENT_VERIFY_FAIL_TOO_MANY_ATTEMPTS;
        verify_fail_count =0;
    }
	
#if DEBUG_MODE
    printf("[CB_LOG_INFO], %s result %s\r\n", __func__, ct_modeStr(result));
	printf("**************************************\r\n");
	printf("          finish      %s\r\n",__func__);\
	printf("**************************************\r\n");
#endif

    return result;
}
