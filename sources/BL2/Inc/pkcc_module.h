#ifndef PKCC_MODULE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "CMSDK_CM3.h"

#include "cm0ik_reg.h"


extern void delay_us(int us);

typedef struct {
    uint32_t x[32];
    uint32_t y[32];
}Point;

void pkcc_memory_setting(void);
uint32_t uint32_inverse(uint32_t* input, uint32_t* output);

unsigned char pkcc_ECPSM(int mode, unsigned int *modular_byte, unsigned int *cal_data, Point *org_point, Point *result_point);
unsigned char pkcc_ECPA(int mode, unsigned int *modular_byte, Point *cal_point1,  Point *cal_point2, Point *result_point);
unsigned char pkcc_MA(int mode, unsigned int *modular_byte, unsigned int *cal_data1, unsigned int *cal_data2, unsigned int *result);
unsigned char pkcc_MM(int mode, unsigned int *modular_byte, unsigned int *cal_data1, unsigned int *cal_data2, unsigned int *result);
unsigned char pkcc_MI(int mode, unsigned int *modular_byte, unsigned int *org_data, unsigned int *result);

unsigned char pkcc_ECPS(int mode, unsigned int *modular_byte,  Point *cal_point1,  Point *cal_point2,  Point *result_point);
unsigned char pkcc_MSR(int mode, unsigned int *modular_byte, unsigned int *a_param, unsigned int *b_param, unsigned int *msg,  Point *result_point);

uint32_t pkcc_RSA(int mode, uint32_t* modular_byte, uint32_t* key, uint32_t* msg, uint32_t* ret);

unsigned char ecc_ECPD(int mode, unsigned int* modular_byte, Point* org_point, Point* result_point);
unsigned char ecc_MS(int mode, unsigned int *modular_byte, unsigned int *cal_data1, unsigned int *cal_data2, unsigned int *result);
unsigned char ecc_MD(int mode, unsigned int* modular_byte, unsigned int* cal_data1, unsigned int* cal_data2, unsigned int* result);

#endif