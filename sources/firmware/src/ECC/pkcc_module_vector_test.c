#include <pkcc_module.h>

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"

#include <string.h>

static const uint32_t ECDSA_MODULAR_P[8]	= {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

void uint32_print_32(const char* label, const uint32_t* bytes, uint32_t byte)
{
    const size_t bytes_per_line = 32;

    printf("\r\n %s:\r\n\t", label);
    for(int i = 0; i < byte; i++){if( (i%8 == 0) && (i!=0)) printf("\r\n\t"); printf(" %08x", bytes[i]); }
   // putc('\n', stdout);
	printf("\r\n");
}

void mbed_mm_test(){
	mbedtls_mpi k, e, t, s, d;
//	uint32_t temp[8]  = { 0x3c07bc85, 0x6eddda43, 0x8ff6e546, 0x7d98d3ae, 0x27306ebb, 0x0d6685d7, 0xba8cc65d, 0x09a321b9 };
//	uint32_t temp2[8] = { 0x1c119654, 0x1cf45e96, 0x541ef99c, 0x4b718db3, 0x487c7d4e, 0xa334cb2d, 0x65725eec, 0x00c86ecf };
	uint32_t temp[8]   = { 0x5496111c, 0x965ef41c, 0x9cf91e54, 0xb38d714b, 0x4e7d7c48, 0x2dcb34a3, 0xec5e7265, 0xcf6ec800 };
	uint32_t temp2[8]  = { 0x85bc073c, 0x43dadd6e, 0x46e5f68f, 0xaed3987d, 0xbb6e3027, 0xd785660d, 0x5dc68cba, 0xb921a309 };
	uint32_t order_n[8] =  { 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xBCE6FAAD, 0xA7179E84, 0xF3B9CAC2, 0xFC632551 };
	
//	mbedtls_mpi_init( &k ); mbedtls_mpi_init( &e ); mbedtls_mpi_init( &t ); mbedtls_mpi_init( &s ); mbedtls_mpi_init( &d );

//	k.s = 0x01;
//	k.n = 0x10;
//	//memcpy(k.p, temp, 32);
//	uint32_inverse(temp, k.p);
//	
//	d.s = 0x01;
//	d.n = 0x08;
//	//memcpy(d.p, temp2, 32);
//	uint32_inverse(temp2, d.p);
//	
//	mbedtls_mpi_mul_mpi( &s, &k, &d );
//	
//	uint32_print_32("mul k*d(sw)",s.p, s.n);
//	
//	mbedtls_mpi_add_mpi( &s, &k, &d );
//	uint32_print_32("add k+d(sw)",s.p, s.n);
//	
	uint32_t input1[8], input2[8], output[8];
	
	memcpy(input1, temp, 32);
	memcpy(input2, temp2, 32);
	memset(output, 0, 32);
	
	//pkcc_memory_setting();	
	
//	pkcc_MM(2, order_n, input1, input2, output);
//	uint32_print_32("mul k*d(hw)",output, 8);
//	
//	pkcc_MA(2, order_n, input1, input2, output);
//	uint32_print_32("mul k+d(hw)",output, 8);
	
	uint32_t temp3[8] = {0x5f6bd7fe, 0xca8f6e30, 0x23209db2, 0x69e4c307, 0x20bab6f4, 0x90382c52, 0xd50c0cef, 0xd13aa278 };
	uint32_t k_inv[8];
	
}

void ecc_ecpsm_256_test(){
	int i = 0;
	uint32_t mod[32]; // = { 0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	uint32_t key[8] = { 0xE2EFE6B3, 0x875FD27C, 0x80C81E75, 0xD382D54E, 0x9691975C, 0x784A4B5B, 0x8D99B4F1, 0x1D22578D };
	uint32_t p_x[8] = { 0x6B17D1F2, 0xE12C4247, 0xF8BCE6E5, 0x63A440F2, 0x77037D81, 0x2DEB33A0, 0xF4A13945, 0xD898C296 };
	uint32_t p_y[8] = { 0x4FE342E2, 0xFE1A7F9B, 0x8EE7EB4A, 0x7C0F9E16, 0x2BCE3357, 0x6B315ECE, 0xCBB64068, 0x37BF51F5 };
	memcpy(mod, ECDSA_MODULAR_P, 32);
	
	Point plain;
	Point result;
	
	for( i = 0; i < 8; i++){
		plain.x[i] = p_x[i];
		plain.y[i] = p_y[i];
	}
	
	pkcc_ECPSM(0x02, mod, key, &plain, &result);
	
	printf("\r\n ecc_ecpsm_256_test");
	printf("\r\n result_x  : "); for(i = 0; i < 8; i++){ printf("%08x ", result.x[i]); }
	printf("\r\n result_y  : "); for(i = 0; i < 8; i++){ printf("%08x ", result.y[i]); }
	
	return ;
}

void ecc_ECPS_256_test(){
	int i = 0;
	uint32_t mod[32]; // = { 0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	uint32_t p_x1[8] = { 0x6B17D1F2, 0xE12C4247, 0xF8BCE6E5, 0x63A440F2, 0x77037D81, 0x2DEB33A0, 0xF4A13945, 0xD898C296 };
	uint32_t p_y1[8] = { 0x4FE342E2, 0xFE1A7F9B, 0x8EE7EB4A, 0x7C0F9E16, 0x2BCE3357, 0x6B315ECE, 0xCBB64068, 0x37BF51F5 };
	uint32_t p_x2[8] = { 0x8C61862B, 0xE2C69E7D, 0xD3EEBA1C, 0xEB56CA5B, 0xCB8ED4EC, 0xE40478CF, 0x9639F6CC, 0x465AD10E };
	uint32_t p_y2[8] = { 0xE47609B0, 0x506FA822, 0x9A97BEDA, 0x67616411, 0x838B0EC8, 0x9A0BF40F, 0x28F134E6, 0x0EA229CB };
	uint32_t ex_x[8] = { 0xef43e2e7, 0x037da3a3, 0x78ba2a40, 0x169baba2, 0x32bc181c, 0x63e435ce, 0x9cfdf772, 0xa17e5b41 };
	uint32_t ex_y[8] = { 0x16ac21f7, 0xc885a954, 0xf398573f, 0xe18e36ba, 0x11e26f96, 0x1d6dee74, 0x0291f577, 0x143381e7 };
	memcpy(mod, ECDSA_MODULAR_P, 32);

	Point plain1;
	Point plain2;
	Point result;
	
	for( i = 0; i < 8; i++){
		plain1.x[i] = p_x1[i];
		plain1.y[i] = p_y1[i];
		plain2.x[i] = p_x2[i];
		plain2.y[i] = p_y2[i];
	}
	
	pkcc_ECPS(0x02, mod, &plain1, &plain2, &result);
	
	printf("\r\n ecc_ECPS_256_test");
	printf("\r\n expect x  : "); for(i = 0; i < 8; i++){ printf("%08x ", ex_x[i]); }
	printf("\r\n result y  : "); for(i = 0; i < 8; i++){ printf("%08x ", result.x[i]); }
	printf("\r\n expect x  : "); for(i = 0; i < 8; i++){ printf("%08x ", ex_y[i]); }
	printf("\r\n result y  : "); for(i = 0; i < 8; i++){ printf("%08x ", result.y[i]); }
}

void ecc_MI_256_test(){
	int i = 0;
	uint32_t mod[32]; // = { 0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	uint32_t org[8] = { 0xBE411E11, 0xA31A6D3C, 0x732145B5, 0x3253844C, 0xC16395DA, 0x87F27B61, 0x5735668E, 0xAAF465D5 };
	uint32_t exp[8] = { 0xd12f9fb8, 0x0b40161b, 0xa87f117e, 0xcdbfcebd, 0x7c44ef25, 0x7f816ad2, 0x5ad0fcd9, 0xe88a0181 };
	uint32_t res[8] = { 0, };
	memcpy(mod, ECDSA_MODULAR_P, 32);
	
	pkcc_MI(0x02, mod, org, res);
	
	printf("\r\n ecc_MI_256_test");
	printf("\r\n expect  : "); for(i = 0; i < 8; i++){ printf("%08x ", exp[i]); }
	printf("\r\n result  : "); for(i = 0; i < 8; i++){ printf("%08x ", res[i]); }
	
	return ;
}

void ecc_MM_256_test(){
	#if 1
	uint32_t r[8]		 = { 0x3c07bc85, 0x6eddda43, 0x8ff6e546, 0x7d98d3ae, 0x27306ebb, 0x0d6685d7, 0xba8cc65d, 0x09a321b9 };
	uint32_t pri_key[8]  = { 0x1c119654, 0x1cf45e96, 0x541ef99c, 0x4b718db3, 0x487c7d4e, 0xa334cb2d, 0x65725eec, 0x00c86ecf };
	uint32_t order_n[8] =  { 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xBCE6FAAD, 0xA7179E84, 0xF3B9CAC2, 0xFC632551 };
	//uint32_t order_n[8] =    { 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xadfae6bc, 0x849e17a7, 0xc2cab9f3, 0x512563fc};
	//uint32_t order_n[8]  = { 0xffffffff, 0x00000000, 0xffffffff, 0xffffffff, 0xbce6faad, 0xa7179e84, 0xf3b9cac2, 0xfc632551 };
//	uint32_t pri_key[8]  = { 0x00c86ecf, 0x65725eec, 0xa334cb2d, 0x487c7d4e, 0x4b718db3, 0x541ef99c, 0x1cf45e96, 0x1c119654 };
//	uint32_t r[8]		 = { 0x09a321b9, 0xba8cc65d, 0x0d6685d7, 0x27306ebb, 0x7d98d3ae, 0x8ff6e546, 0x6eddda43, 0x3c07bc85 };
	#else
	uint32_t order_n[8] = {  0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
	uint32_t pri_key[8]  = { 0xD8CA8B63, 0x81E23B0C, 0x2548DD75, 0xFACD8F14, 0xCC398C0A, 0x3EF6D0F2, 0x9483C8BE, 0x521A43D9 };
	uint32_t r[8]		 = { 0xBF088502, 0xA4749034, 0x4A1B3A61, 0x50320B98, 0x9C333F47, 0xABCBF795, 0xB24BB2A2, 0x53F850F1 };
	uint32_t expect[8] 	 = { 0xe090f4a6, 0x6d3ce04f, 0xd696cae4, 0xb30c8405, 0x3c44c1c9, 0xfcdaf66e, 0x8f9ca7e4, 0xd6b4a574 };
	#endif
	int i = 0;
	unsigned int output[8] = {0,};
	
	//pkcc_memory_setting();	
	
	pkcc_MM(2, order_n, pri_key, r, output);
    delay_us(1000);
    printf("\r\necc_MM_256_test");
    
	printf("\r\n mod_N  : "); for(i = 0; i < 8; i++){ printf("%08x ", order_n[i]); }
	printf("\r\n input 1: "); for(i = 0; i < 8; i++){ printf("%08x ", pri_key[i]); }
	printf("\r\n input 2: "); for(i = 0; i < 8; i++){ printf("%08x ", r[i]); }
	
    printf("\r\n output.x : "); for(i = 0; i < 8; i++){ printf("%08x ", output[i]); }
	
//	print_bytes(" temp(hw)", r, 32);    
//	print_bytes(" temp2(hw)", pri_key, 32);   
//	print_bytes(" ecdsa.order_n(hw)", order_n, 32);    
//	print_bytes(" dr(hw)", output, 32);  
	
}


void ecc_MA_256_test(){
    unsigned int mod_N[8] = { 0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    unsigned int arr1[8] = { 0xA1BECD8F, 0xE5C7B6ED, 0xBADBF03F, 0x8FDA6282, 0x185AE7A7, 0x2A1E14E0, 0x77F55CE6, 0xB5AF5F81 };
    unsigned int arr2[8] = { 0xFBD85FB9, 0x3DF46EAC, 0x7ED103D8, 0xB53364E0, 0xE5EA566A, 0x01408257, 0x7BFE1310, 0x60F96F15 };
    unsigned int ext[8] = { 0x9d972d4a, 0x23bc2599, 0x39acf418, 0x450dc762, 0xfe453e10, 0x2b5e9737, 0xf3f36ff7, 0x16a8ce97 };
	
	unsigned int output[8] = {0,};
	int i = 0;
    
    pkcc_MA(2, mod_N, arr1, arr2, output);
    delay_us(1000);
    printf("\r\necc_MA_256_test");
	
	printf("\r\n input 1: "); for(i = 0; i < 8; i++){ printf("%08x ", arr1[i]); }
	printf("\r\n input 2: "); for(i = 0; i < 8; i++){ printf("%08x ", arr2[i]); }
    
    printf("\r\n output.x : "); for(i = 0; i < 8; i++){ printf("%08x ", output[i]);}
    
    printf("\r\n expect.x : "); for(i = 0; i < 8; i++){ printf("%08x ", ext[i]); }
    
    for(i = 0; i < 8; i++){
        if(output[i] != ext[i]){ printf("\r\n value unmatching\r\n"); break; }
    }
    printf("\r\n");
    
    return;
}


void ecc_ECPA_256_test_org(){
    Point g_a;
    Point g_b;
    Point output;
    Point ext;
    unsigned int mod_N[8] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
    int i = 0;
  	//pkcc_memory_setting();  
    g_a.x[7] = 0xD898C296;
    g_a.x[6] = 0xF4A13945;
    g_a.x[5] = 0x2DEB33A0;
    g_a.x[4] = 0x77037D81;
    g_a.x[3] = 0x63A440F2;
    g_a.x[2] = 0xF8BCE6E5;
    g_a.x[1] = 0xE12C4247;
    g_a.x[0] = 0x6B17D1F2;
    
    g_a.y[7] = 0x37BF51F5;
    g_a.y[6] = 0xCBB64068;
    g_a.y[5] = 0x6B315ECE;
    g_a.y[4] = 0x2BCE3357;
    g_a.y[3] = 0x7C0F9E16;
    g_a.y[2] = 0x8EE7EB4A;
    g_a.y[1] = 0xFE1A7F9B;
    g_a.y[0] = 0x4FE342E2;
    
    g_b.x[7] = 0x465AD10E;
    g_b.x[6] = 0x9639F6CC;
    g_b.x[5] = 0xE40478CF;
    g_b.x[4] = 0xCB8ED4EC;
    g_b.x[3] = 0xEB56CA5B;
    g_b.x[2] = 0xD3EEBA1C;
    g_b.x[1] = 0xE2C69E7D;
    g_b.x[0] = 0x8C61862B;
    
    g_b.y[7] = 0x0EA229CB;
    g_b.y[6] = 0x28F134E6;
    g_b.y[5] = 0x9A0BF40F;
    g_b.y[4] = 0x838B0EC8;
    g_b.y[3] = 0x67616411;
    g_b.y[2] = 0x9A97BEDA;
    g_b.y[1] = 0x506FA822;
    g_b.y[0] = 0xE47609B0;
    
    ext.x[7] = 0x75a42681;
    ext.x[6] = 0x0d41b3f0;
    ext.x[5] = 0xbf788759;
    ext.x[4] = 0x7032c618;
    ext.x[3] = 0xb604574d;
    ext.x[2] = 0x29642930;
    ext.x[1] = 0x9fc28488;
    ext.x[0] = 0x5ad2d858;
    
    ext.y[7] = 0x37aa7b7d;
    ext.y[6] = 0x6e806a5e;
    ext.y[5] = 0x57f00879;
    ext.y[4] = 0xea9b6341;
    ext.y[3] = 0xa2027b91;
    ext.y[2] = 0x0f303994;
    ext.y[1] = 0x0d0ec853;
    ext.y[0] = 0x7f8900eb;
    
	memcpy(mod_N, ECDSA_MODULAR_P,32);
   
    pkcc_ECPA(2, mod_N, &g_a, &g_b, &output);

    printf("\r\necc_ECPA_256_test");
    
    printf("\r\n output.x\r\n"); for(i = 0; i < 8; i++){ printf("%08x ", output.x[i]); }
    
    printf("\r\n expect.x\r\n"); for(i = 0; i < 8; i++){  printf("%08x ", ext.x[i]);  }
    printf("\r\n output.y\r\n"); for(i = 0; i < 8; i++){ printf("%08x ", output.y[i]); }
    printf("\r\n expect.y\r\n"); for(i = 0; i < 8; i++){  printf("%08x ", ext.y[i]);   }
    printf("\r\n");
    
    for(i = 0; i < 6; i++){
        if(output.x[i] != ext.x[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
        if(output.y[i] != ext.y[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
    }
    
    return;
}

void ecc_ECPA_256_test(){
	
	Point verify_data, verify1, verify2;
	uint32_t v1x[8] = {0x293a0e3b, 0x5fab3bd1, 0x162819ba, 0x03c0d4a9, 0xfb7ec282, 0x803bb538, 0xec7668c9, 0xa72d9ed0};
	uint32_t v1y[8] = {0xb7fbbbae, 0x5bf77acb, 0xe8767a2e, 0xcf66f156, 0x965104bd, 0x7b42119b, 0x8564e6a2, 0xaeed38a6};
	uint32_t modular_p[8];	
	uint32_t v2x[8] = {0x737810bd, 0xa826811b, 0x4df3687b, 0x6909d2d8, 0xc93236a7, 0xf3f61cf6, 0x72abad93, 0x52b316a8};
	uint32_t v2y[8] = {0xf9b56094, 0xbbe5f93b, 0xec9cde0e, 0xd60d3b72, 0x4b6cc2ee, 0xe384d1b1, 0x7e978152, 0xe5f727fe};
	
	//static const uint32_t ecdsa_p256_modular_p[8]	= { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
	
	//pkcc_memory_setting();
	
	memcpy(modular_p, ECDSA_MODULAR_P, 32);
	//memcpy(modular_p, ecdsa_p256_modular_p, 32);
	memcpy(verify1.x, v1x, 32);
	memcpy(verify1.y, v1y, 32);
	
	memcpy(verify2.x, v2x, 32);
	memcpy(verify2.y, v2y, 32);
	
	printf("\r\n modular_p\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", modular_p[i]); }	
	printf("\r\n verify1.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify1.x[i]); }	
	printf("\r\n verify1.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify1.y[i]); }	
	printf("\r\n verify2.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify2.x[i]); }	
	printf("\r\n verify2.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify2.y[i]); }	
	printf("\r\n");
	pkcc_ECPA(2, modular_p, &verify1, &verify2, &verify_data);
	//pkcc_ECPA(bob.pkcc_mode, bob.modular_p, &verify2, &verify1, &verify_data);
	delay_us(100);
	printf("\r\n verify_data.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify_data.x[i]); }	
	printf("\r\n verify_data.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify_data.y[i]); }	
	printf("\r\n");
}

void ecc_ECPD_256_test(){
    Point g;
    Point output;
    Point ext;
    //unsigned int mod_N[8] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
    int i = 0;
	uint32_t modular_p[8];	
    memcpy(modular_p, ECDSA_MODULAR_P, 32);
	
    g.x[0] = 0x6B17D1F2;
    g.x[1] = 0xE12C4247;
    g.x[2] = 0xF8BCE6E5;
    g.x[3] = 0x63A440F2;
    g.x[4] = 0x77037D81;
    g.x[5] = 0x2DEB33A0;
    g.x[6] = 0xF4A13945;
    g.x[7] = 0xD898C296;
    
    g.y[0] = 0x4FE342E2;
    g.y[1] = 0xFE1A7F9B;
    g.y[2] = 0x8EE7EB4A;
    g.y[3] = 0x7C0F9E16;
    g.y[4] = 0x2BCE3357;
    g.y[5] = 0x6B315ECE;
    g.y[6] = 0xCBB64068;
    g.y[7] = 0x37BF51F5;
    
    ext.x[0] = 0x7cf27b18;
    ext.x[1] = 0x8d034f7e;
    ext.x[2] = 0x8a523803;
    ext.x[3] = 0x04b51ac3;
    ext.x[4] = 0xc08969e2;
    ext.x[5] = 0x77f21b35;
    ext.x[6] = 0xa60b48fc;
    ext.x[7] = 0x47669978;
    
    ext.y[0] = 0x07775510;
    ext.y[1] = 0xdb8ed040;
    ext.y[2] = 0x293d9ac6;
    ext.y[3] = 0x9f7430db;
    ext.y[4] = 0xba7dade6;
    ext.y[5] = 0x3ce98229;
    ext.y[6] = 0x9e04b79d;
    ext.y[7] = 0x227873d1;
	
	//pkcc_memory_setting();
    
    ecc_ECPD(2, modular_p, &g, &output);
    
    printf("\r\necc_ECPD_256_test");
    
    printf("\r\n output.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", output.x[i]);
    }
    
    printf("\r\n expect.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", ext.x[i]);
    }
    
    printf("\r\n output.y\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", output.y[i]);
    }
    
    printf("\r\n expect.y\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", ext.y[i]);
    }
    printf("\r\n");
    
    for(i = 0; i < 8; i++){
        if(output.x[i] != ext.x[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
        if(output.y[i] != ext.y[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
    }
    
    return;
}

void ecc_MS_256_test(){
    //unsigned int mod_N[8] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
    unsigned int arr1[8] = { 0xF167B8F6, 0x3844E133, 0xF1D36D30, 0xAED66E50, 0x293AD5B3, 0xB483C11A, 0x2258C7F3, 0x0509DE2F };
    unsigned int arr2[8] = { 0xC7C0B7A0, 0xB43DAA6B, 0x87300ABF, 0xAE70B63C, 0x3D6EA7C7, 0xC51FE010, 0xB9FB7A53, 0x863263F7 };
    unsigned int output[8] = { 0, };
    unsigned int ext[8] = { 0x29a70155, 0x840736c8, 0x6aa36271, 0x0065b813, 0xebcc2deb, 0xef63e109, 0x685d4d9f, 0x7ed77a38 };
    int i = 0;
	uint32_t modular_p[8];	
    memcpy(modular_p, ECDSA_MODULAR_P, 32);
	
	//pkcc_memory_setting();
    
    ecc_MS(2, modular_p, arr1, arr2, output);
    
    printf("\r\necc_MS_256_test");
    
    printf("\r\n output.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", output[i]);
    }
    
    printf("\r\n expect.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", ext[i]);
    }
    
    for(i = 0; i < 8; i++){
        if(output[i] != ext[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
    }
    printf("\r\n");
    
    return;
}

void ecc_MD_256_test(){
    //unsigned int mod_N[8] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF };
    unsigned int arr1[8] = { 0xB7BAC902, 0x06A83FEB, 0xA154031A, 0x50E12F7B, 0x5C5238A4, 0x2F20E4F1, 0x0AFCCAEB, 0x8F820F1E };
    unsigned int arr2[8] = { 0xA3ECA163, 0xD180D476, 0xDBEF7FD4, 0xDE963E22, 0xBCBDD27E, 0x6B6CF2BE, 0x88A18C82, 0x942B2FB4 };
    unsigned int output[8] = {0,};
    unsigned int ext[8] = { 0x6ea8e92d, 0x7b8556c6, 0x32cb2531, 0xc2e70b82, 0x3bf6bff0, 0xf0e03f93, 0xf3b1dcc7, 0xf943fdfb };
    int i = 0;
	uint32_t modular_p[8];	
    memcpy(modular_p, ECDSA_MODULAR_P, 32);
	
	//pkcc_memory_setting();
         
    ecc_MD(2, modular_p, arr1, arr2, output);
    
    printf("\r\necc_MD_256_test");
    
    printf("\r\n output.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", output[i]);
    }
    
    printf("\r\n expect.x\r\n");
    for(i = 0; i < 8; i++){
        printf("%8x ", ext[i]);
    }
    
    for(i = 0; i < 8; i++){
        if(output[i] != ext[i]){
           printf("\r\n value unmatching\r\n");
           break;
        }
    }
    printf("\r\n");
    
    return;
}

void pkcc_find_curve_test(){
	unsigned int mod[32];
	unsigned int a_param[32];
	unsigned int b_param[32];
	unsigned int msg[32];
	int mode = 2;
	int result = 1;
	int i = 0, count = 100;
	
	Point test;
	
	memcpy(mod, ECDSA_MODULAR_P, 32);
	
	a_param[7] = 0xFFFFFFFF;
	a_param[6] = 0x00000001;
	a_param[5] = 0x00000000;
	a_param[4] = 0x00000000;
	a_param[3] = 0x00000000;
	a_param[2] = 0xFFFFFFFF;
	a_param[1] = 0xFFFFFFFF;
	a_param[0] = 0xFFFFFFFC;
	
	b_param[7] = 0x5AC635D8; 
	b_param[6] = 0xAA3A93E7; 
	b_param[5] = 0xB3EBBD55; 
	b_param[4] = 0x769886BC; 
	b_param[3] = 0x651D06B0; 
	b_param[2] = 0xCC53B0F6; 
	b_param[1] = 0x3BCE3C3E; 
	b_param[0] = 0x27D2604B;

#if 0
	msg[0] = 0x00002663;
	msg[1] = 0x9DE2D740;
	msg[2] = 0xF1700DB3;
	msg[3] = 0x53455C33;
	msg[4] = 0xAC2BAF9C;
	msg[5] = 0xD3856E97;
	msg[6] = 0x21EBD76A;
	msg[7] = 0x66310A06;
#elif 0
	msg[7] = 0xcaea6c53;
	msg[6] = 0xf9d400f9;
	msg[5] = 0x9a48f0d6;
	msg[4] = 0x4f5975d7;
	msg[3] = 0x3745df7e;
	msg[2] = 0xd97a1df0;
	msg[1] = 0xb6c9262a;
	msg[0] = 0xc7991f5d;
#elif 1
	msg[7] = 0x6B17D1F2;
	msg[6] = 0xE12C4247;
    msg[5] = 0xF8BCE6E5;
    msg[4] = 0x63A440F2;
    msg[3] = 0x77037D81;
    msg[2] = 0x2DEB33A0;
    msg[1] = 0xF4A13945;
    msg[0] = 0xD898C296;
#endif

//	result = pkcc_MSR(mode, mod_N, a_param, b_param, msg, test);
//		
//	if(result == 0){
//		printf("value in curve \r\n value : ");
//			
//		for(i = 0; i < 8; i++){
//			printf("0x%08x ", msg[i]);
//		}
//		printf("\r\n");
//		return ;
//	}else{
//		printf("\r\nvalue not in curve\r\n");
//		
//		for(i = 0; i < 8; i++){
//			printf("0x%08x ", msg[i]);
//		}
//	}
	
#if 1
	printf("\r\nPKCC FIND CURVE(MSI) TEST\r\n");
	while(count--){
		if(count % 10 == 0){
			printf("count : %d", count);
		}
		for(i = 0; i < 8; i++){
			msg[i] = (rand() % 0xFFFF);
			printf("0x%08x ", msg[i]);
			delay_us(10);
		}
		
		result = pkcc_MSR(mode, mod, a_param, b_param, msg, &test);
		
		if(result == 0){
			printf("\r\nvalue in curve \r\n value : ");
			
			for(i = 0; i < 8; i++){
				printf("0x%08x ", msg[i]);
			}
			printf("\r\ntest x");
			for(i = 0; i < 8; i++){
				printf("0x%08x ", test.x[i]);
			}
			printf("\r\ntest y");
			for(i = 0; i < 8; i++){
				printf("0x%08x ", test.y[i]);
			}
			printf("\r\n");
			return ;
		}else{
			printf("\r\nvalue not in curve\r\n");
		}
		delay_us(100);
	}
	printf("value not in curve");
#endif
	return ;
}