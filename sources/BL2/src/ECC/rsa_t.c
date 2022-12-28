#include <pkcc_module.h>

#if 1
void rsa_test_encrypt(uint32_t *msg,uint32_t *res){
	//n,d -> private key
	uint32_t n[64] = {0x69a5b76f, 0x3d3640e4, 0xe5bd5de4, 0x2899a319, 0x85acf4d9, 0x26751b12, 0x1edf1a85, 0x42aed462, 0x43a0eef5, 0xa6b3b894, 0x02ab387b, 0x5d0e9c31, 0xd5d9f05f, 0x65a199a9, 0x23d5958d, 0xa0215593, 0x7fea34e1, 0x28db2ee8, 0x7a9d07f7, 0xae2ebe72, 0x96ac2cbb, 0x119adb67, 0x86e38bbb, 0xbf6ea756, 0x0077ff3a, 0xeb3b2d14, 0x9ea7e609, 0x7dec7cf2, 0x43d2e0b3, 0xc9d0a933, 0x162fda8f, 0x34d9a60b, 0x8ab62a11, 0x85cedf76, 0x13d680ff, 0xe97b039c, 0x35fd0591, 0x5dab459b, 0xd59efd0e, 0xa8709244, 0x88941cdc, 0xa181fe58, 0x758a9a60, 0x93d27e93, 0x4cddfa44, 0xa5b31b32, 0xc7504fed, 0x85156a85, 0xf31f539f, 0x37ff51e6, 0xae33d829, 0xe1cef544, 0x0fb3a225, 0x619a0f2c, 0x14d41133, 0x56cd427c, 0x7f6a4ba4, 0x0d474135, 0x1a8a24e5, 0x24406f78, 0x9c9d1c39, 0x9f4f7b5d, 0x8ff277f6, 0xb3081dcd	};
	uint32_t d[64] = {0x135d185d, 0xff2e2e7e, 0xc5cd0ba4, 0x81fc86f4, 0x6a97e4d8, 0xd83fa692, 0x8fadfcc2, 0x68127b42, 0xf77a4bd2, 0x2297d640, 0xeaac4f51, 0xed207c68, 0x75f5d824, 0xe40fd686, 0x35d733ee, 0x1d677eb3, 0xe6421b27, 0x19c1f2bb, 0x119e2e93, 0xab0d4e42, 0xda72fc4f, 0xdd4e30e0, 0xaccc9013, 0xfd665ead, 0x58b0a5cb, 0x3aca41dd, 0x01a76c76, 0x58ea3b8c, 0x7bf9eecd, 0x8534058f, 0x2385e9dd, 0x4da686d4, 0x62055cf0, 0x4b198874, 0xbb510774, 0xeb312ac1, 0x5001c8f6, 0xbf9ef0a1, 0x286dcdad, 0xad8ae3af, 0x04194c59, 0x8a909958, 0x6aadab33, 0x3fd9c425, 0x345bf334, 0xa0f247aa, 0x100185ca, 0x47e839f1, 0x875ebe87, 0x3df1ce0a, 0x6a55b142, 0xeca3fe6f, 0x59c822a5, 0x1a711299, 0x181f42a6, 0xc057f6bf, 0xb7b40a8e, 0xf79f17b1, 0xb98448a8, 0xcf84ad61, 0x2221c7c3, 0x878951ed, 0x07750b30, 0x2d7a6581};
	uint32_t len = 0;

	len = pkcc_RSA(1, n, d, msg, res);
	printf("\r\n"); 
	printf("\r\n ms\t:\t");	for(int i = 0; i < len+1; i++){if( (i%8 == 0) && (i!= 0) ) printf("\r\n\t\t"); printf("0x%08x ", msg[i]); }	
	printf("\r\n"); 
	printf("\r\n result\t:\t");	for(int i = 0; i < len+1; i++){if( (i%8 == 0) && (i!= 0) ) printf("\r\n\t\t");  printf("0x%08x ", res[i]); }	
	
}




void rsa_test_decrypt(uint32_t *msg, uint32_t *res, uint32_t *n, uint32_t *e){
	//n,e -> public key
	uint32_t len = 0;

	len = pkcc_RSA(1, n, e, msg, res);
  

}
#endif