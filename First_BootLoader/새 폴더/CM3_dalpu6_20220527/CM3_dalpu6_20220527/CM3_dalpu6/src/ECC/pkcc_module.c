#include <pkcc_module.h>

uint32_t uint32_inverse(uint32_t* input, uint32_t* output){
	
	for( int i=0, j=7; j>=0; j--) {
		output[i] =  ((((input[j])&0xff)<<0))
					|((((input[j])>>8)&0xff)<<8)
					|((((input[j])>>16)&0xff)<<16)
					|((((input[j])>>24)&0xff)<<24);
//		output[i] =  ((((input[j])&0xff)<<24))
//					|((((input[j])>>8)&0xff)<<16)
//					|((((input[j])>>16)&0xff)<<8)
//					|((((input[j])>>24)&0xff)<<0);
		i++;
	}
	
	return 0;
}	

uint32_t int_data_inverse(uint32_t* input, uint32_t* output, int byte){
	
	for( int i=0; i<byte; i++){
		output[i] = input[(byte-1)-i];
	}
	
	return 0;
}

#if 0
void pkcc_memory_setting(){
  	*RSAECC_ADDR_MOD			=	0x7480; //memory address 
	*RSAECC_ADDR_Px				=	0x7500;
	*RSAECC_ADDR_Py				=	0x7580;
	*RSAECC_ADDR_Pbx			=	0x7600;
	*RSAECC_ADDR_KEY			=	0x7680;
	*RSAECC_ADDR_Pby			=	0x7700;
	*RSAECC_ADDR_MSG			=	0x7780;
	*RSAECC_ADDR_oDx			=	0x7800;
	*RSAECC_ADDR_oDy			=	0x7880;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
//                                         ECC operator                                         //
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////

int ecc_set_op_status(unsigned char mode){
    int op_stauts = 0;
    
    switch(mode){
        case 0x00:	op_stauts	=	0x80000000;	break;	// 000 p-192
        case 0x01:	op_stauts	=	0x84000000; break;	// 001 p-224
        case 0x02:	op_stauts	=	0x88000000;	break;	// 010 p-256
        case 0x03:	op_stauts	=	0x8C000000;	break;	// 011 p-384
        case 0x04:	op_stauts	=	0x90000000;	break;	// 100 p-521
        default	:	break;
    }
    
    return op_stauts;
}

unsigned char ecc_set_length(unsigned char mode){
    unsigned char len = 0;
    
    switch(mode){
        case 0x00:	len	=	6 - 1;	break;	// 000 p-192
        case 0x01:	len	=	7 - 1;	break;	// 001 p-224
        case 0x02:	len	=	8 - 1;	break;	// 010 p-256
        case 0x03:	len	=	12 - 1;	break;	// 011 p-384
        case 0x04:	len	=	17 - 1;	break;	// 100 p-521
        default:	break;
    }
    return len;
}

//*0
unsigned char pkcc_ECPSM(int mode, unsigned int* modular_byte, unsigned int* cal_data, Point* org_point, Point* result_point){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;

    op_stauts = ecc_set_op_status(mode);    
    op_stauts += 0x00000;
  
    //*RSAECC_CMD_Reg_0 = op_stauts;
	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
    
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = org_point->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = org_point->y[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;			// Start
	delay_us(50);
	
	
    while(1){
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
    

	for(i = 0; i <= len; i++) result_point->x[len - i] = *RSA_ECC_READ_DATA;
	for(i = 0; i <= len; i++) result_point->y[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*1
unsigned char pkcc_ECPA(int mode, unsigned int* modular_byte, Point* cal_point1, Point* cal_point2, Point* result_point){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x10000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point1->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point1->y[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point2->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point2->y[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result_point->x[len - i] = *RSA_ECC_READ_DATA;
	for(i = 0; i <= len; i++) result_point->y[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ
	
    return len;
}

//*2
unsigned char ecc_ECPD(int mode, unsigned int* modular_byte, Point* org_point, Point* result_point){
    int i = 0;
    int op_stauts = 0;
    int len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x20000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = org_point->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = org_point->y[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result_point->x[len - i] = *RSA_ECC_READ_DATA;
	for(i = 0; i <= len; i++) result_point->y[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ
	
    return len;
}

//*3
unsigned char pkcc_MA(int mode, unsigned int* modular_byte, unsigned int* cal_data1, unsigned int* cal_data2, unsigned int* result){
    int i = 0;
   int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);    
    op_stauts |= 0x30000;


	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data1[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data2[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*4
unsigned char ecc_MS(int mode, unsigned int *modular_byte, unsigned int *cal_data1, unsigned int *cal_data2, unsigned int *result){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x40000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data1[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data2[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ
	
    return len;
}

//*5
unsigned char pkcc_MM(int mode, unsigned int* modular_byte, unsigned int* cal_data1, unsigned int* cal_data2, unsigned int* result){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x50000;
	
	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data1[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data2[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*6
unsigned char ecc_MD(int mode, unsigned int* modular_byte, unsigned int* cal_data1, unsigned int* cal_data2, unsigned int* result){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x60000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data1[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_data2[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*7
unsigned char pkcc_MI(int mode, unsigned int *modular_byte, unsigned int *org_data, unsigned int *result){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);    
    op_stauts |= 0x70000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = org_data[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*8
unsigned char pkcc_ECPS(int mode, unsigned int* modular_byte, Point* cal_point1, Point* cal_point2, Point* result_point){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    op_stauts |= 0x80000;

	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point1->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point1->y[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point2->x[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = cal_point2->y[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) result_point->x[len - i] = *RSA_ECC_READ_DATA;
	for(i = 0; i <= len; i++) result_point->y[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return len;
}

//*9
unsigned char pkcc_MSR(int mode, unsigned int* modular_byte, unsigned int* a_param, unsigned int* b_param, unsigned int* msg, Point* result_point){
    int i = 0;
    int op_stauts = 0;
    unsigned char len = 0;
    
    op_stauts = ecc_set_op_status(mode);
    
    op_stauts |= 0x90000;
    
	*RSAECC_PKCC_SET = op_stauts;
    
    len = ecc_set_length(mode);
	
	for(i = 0; i < len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i < len; i++) *RSAECC_WRITE_DATA = a_param[len - i];
	for(i = 0; i < len; i++) *RSAECC_WRITE_DATA = a_param[len - i];
	for(i = 0; i < len; i++) *RSAECC_WRITE_DATA = msg[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x80000001)	break;
		if(*RSAECC_STATUS_REG == 0xC0000000){
			*RSAECC_CTRL = 0x01;   // Clear IRQ
			return 1;
		}
	}
	
	for(i = 0; i < len; i++) result_point->x[len - i] = *RSA_ECC_READ_DATA;
	for(i = 0; i < len; i++) result_point->y[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//                                    ECC operator end
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////

int rsa_set_op_status(unsigned char mode){
    int op_stauts = 0;
    
    switch(mode){
        case 0x00:	op_stauts	=	0x00000000;	break;	//sel_RSA_KeyLen :1024 bit len = 4;
        case 0x01:	op_stauts	=	0x00400000; break;	//sel_RSA_KeyLen :2048 bit len = 8;
        case 0x02:	op_stauts	=	0x00800000;	break;	//sel_RSA_KeyLen :4096 bit len =16;
        default	:	op_stauts	=	0x00400000; break;  //sel_RSA_KeyLen :2048 bit len = 8;
    }
    
    return op_stauts;
}

uint32_t rsa_set_length(unsigned char mode){
    uint32_t len = 0;
    
    switch(mode){
//        case 0x00:	len	=	4;	break;	// 1024 bit
//        case 0x01:	len	=	8;	break;	// 2048 bit
//        case 0x02:	len	=	16;	break;	// 4096 bit
//        default:	len	=	8;	break;	// 2048 bit
		case 0x00:	len	=	64 - 1;		break;	// 1024 bit
        case 0x01:	len	=	64 - 1;	break;	// 2048 bit
        case 0x02:	len	=	256 - 1;	break;	// 4096 bit
        default:	len	=	128 - 1;	break;	// 2048 bit
    }
    return len;
}

uint32_t pkcc_RSA(int mode, uint32_t* modular_byte, uint32_t* key, uint32_t* msg, uint32_t* ret){
	int op_stauts = 0, i;
	uint32_t len = 0;
	unsigned char result = 0;
	
	op_stauts = rsa_set_op_status(mode);
	
#if 0
	*RSAECC_CMD_Reg_0 = op_stauts;
	
	len = rsa_set_length(mode);
	
	for(int i = 0; i < len; i++){
        *(RSAECC_ADDR_0 + ((len-1)-i)) = modular_byte[i];	// Mod_N
        *(RSAECC_ADDR_4 + ((len-1)-i)) = key[i];			// e or d
		*(RSAECC_ADDR_6 + ((len-1)-i)) = msg[i];			// msg
    }
	
	*RSAECC_CMD_Reg_1 = 0x00000001;			// Start
	
	while(1){
        if(*RSAECC_StatusReg == 0x1)		{ result = 0; break; }
        else if(*RSAECC_StatusReg == 0x2)	{ result = 1; break; }//  || *RSAECC_StatusReg == 0x3){ result = 1; break;}
		else if(*RSAECC_StatusReg == 0x00)	{ result = 0; }  
	}
	
	for(int i = 0; i < len; i++){
        ret[i] = *(RSAECC_ADDR_7 + ((len-1)-i)); // result
    }
    
    *RSAECC_CMD_Reg_2 = 0x3;   // Clear IRQ
    delay_us(5);
#endif
	*RSAECC_PKCC_SET = op_stauts;
	
	len = rsa_set_length(mode);
	

	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = modular_byte[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = key[len - i];
	for(i = 0; i <= len; i++) *RSAECC_WRITE_DATA = msg[len - i];
	
	delay_us(1);
	
	*RSAECC_CTRL = 0xC0000000;
	
	while(1){
       if(*RSAECC_STATUS_REG == 0x80000001)	break;
	}
	
	for(i = 0; i <= len; i++) ret[len - i] = *RSA_ECC_READ_DATA;
    
	*RSAECC_CTRL = 0x01;   // Clear IRQ
    delay_us(5);
	
	return len;
}


int prime_set_op_status(unsigned char mode){
    int op_stauts = 0;
    
    switch(mode){
        case 0x00:	op_stauts	=	0x00000000;	break;	// 000 PRIME_SIZE = 512bit
        case 0x01:	op_stauts	=	0x00000040; break;	// 001 PRIME_SIZE = 1024
        case 0x02:	op_stauts	=	0x00000080;	break;	// 010 PRIME_SIZE = 2048
        case 0x03:	op_stauts	=	0x00000090;	break;	// 011 PRIME_SIZE = N/A
        default	:	break;
    }
    
    return op_stauts;
}

int prime_set_length(unsigned char mode){
    int len = 0;
    
    switch(mode){
        case 0x00:	len = 16 - 1;
		case 0x01:	len = 32 - 1;
		case 0x02:	len = 64 - 1;
		case 0x03:	len = 0;
        default:	break;
    }
    return len;
}

unsigned char pkcc_prime_gen(int mode, unsigned int*prime_num){
	int i = 0;
	int op_status = 0;
	int len = 0;
	
	op_status = prime_set_op_status(mode);
	
	*RCM3_ST0_OPMODE = 0x01;
	*RCM3_ACCESS     = 0x00;
	*RCM3_ST0_OPMODE = 0x03;
	
	*RCM3_TRNG_RST = 0x01;
	*RCM3_TRNG_256 = 0x07; //oFixSel,oxor_en,o128_256Flag
	delay_us(29); //
	*RCM3_TRNG_REQ = 0x01;
	
	*RSAECC_PRIME_GEN_SET = op_status;
	
	*RSAECC_CTRL = 0x00800000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		//printf("\rstatus : %08x\n", *RSAECC_STATUS_REG);
        if(*RSAECC_STATUS_REG == 0x00800001)	break;
		if(*RSAECC_STATUS_REG == 0x00C00000){
			*RSAECC_CTRL = 0x01;   // Clear IRQ
			
			*RCM3_TRNG_256 = 0x00;
			*RCM3_ST0_OPMODE = 0x01;
			return 1;
		}
	}
	
	for(i = 0; i < len; i++) prime_num[len - i] = *RSA_ECC_READ_DATA;
	
	*RSAECC_CTRL = 0x01;   // Clear IRQ
	
	*RCM3_TRNG_256 = 0x00;
	*RCM3_ST0_OPMODE = 0x01;
	
	return 0;
}

unsigned char pkcc_key_gen(unsigned int* key, unsigned int* prime_p, unsigned int* prime_q, unsigned int* private_key, int prime_size){
	int i, j = 0;
	unsigned int prime_p_temp[64] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x4c776620, 0x17fb7413, 0xb1819f64, 0x3aba331d, 0xf1263e7a, 0xdeb2d272, 0x5f0dbf42, 0x06729f4c, 0xea0ad160, 0x5fa921fa, 0x67de7b02, 0x40ac9c35, 0xb1c7b51a, 0x74a317b0, 0xc0a7fcd6, 0x80284fbd };
	unsigned int prime_q_temp[64] = { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x8172f5d5, 0xfec2f224, 0x3de95279, 0x6e1effb7, 0x35c2946f, 0xff5b696c, 0x0fdf0abb, 0x345be9d3, 0xb94d7702, 0x00d934a1, 0xbaa59cbd, 0x69ca4d9e, 0x5659c4ec, 0x5da24e5b, 0x9e891bac, 0x92fdbe43 };
	
//	for(i = 64 - prime_size; i < 64; i++) prime_p_temp[i] = prime_p[j++];
//	j = 0;
//	for(i = 64 - prime_size; i < 64; i++) prime_q_temp[i] = prime_q[j++];
	
	for(i = 0; i < 8; i++) *RSAECC_WRITE_DATA = key[7 - i];
	for(i = 0; i < 64; i++) *RSAECC_WRITE_DATA = prime_p_temp[63 - i];
	for(i = 0; i < 64; i++) *RSAECC_WRITE_DATA = prime_q_temp[63 - i];
		
	delay_us(1);
	
	*RSAECC_CTRL = 0x0000C000;
	
	while(1){
	//	if(*RSAECC_StatusReg == 0x1)	break;
		
        if(*RSAECC_STATUS_REG == 0x00008001){
			printf("\rstatus 1 : %08x\n", *RSAECC_STATUS_REG);
			break; // pass
		}
		else if(*RSAECC_STATUS_REG == 0x0000C000){	//공개키가 모듈러n과 서로소가 아닌경우
			printf("\rstatus 2 : %08x\n", *RSAECC_STATUS_REG);
			*RSAECC_CTRL = 0x01;   // Clear IRQ
			return 1;
		}
		else if(*RSAECC_STATUS_REG == 0x0000A000){	//공개키가 짝수거나, 공개키가 10001보다 작을 경우
			printf("\rstatus 3 : %08x\n", *RSAECC_STATUS_REG);
			*RSAECC_CTRL = 0x01;   // Clear IRQ
			return 2;
		}
		
	}
	
	printf("KEY\r\n ");
	for(i = 0; i < 264; i++){
		//printf("%08x, ", *RSA_ECC_READ_DATA);
		private_key[263 - i] = *RSA_ECC_READ_DATA;
		delay_us(1);
	}
	printf("\r\n");
	
	*RSAECC_CTRL = 0x01;   // Clear IRQ
	
	return 0;
	
}

