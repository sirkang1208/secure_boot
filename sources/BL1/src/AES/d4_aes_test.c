#include <string.h>
#include <stdio.h>

#include <d6_config.h>

#define AES_DEBUG 0

typedef enum
{
	MODE_ECB = 0,
	MODE_CBC,
	MODE_OFB,
	MODE_CTR,
	MODE_CFB
}RG_AES_OPMODE;
	
typedef enum
{
	RG_256 = 0,
	RG_128
}RG_MODE;
typedef enum
{
	RG_ARIA = 0,
	RG_AES
}RG_ALGO;
typedef enum
{
	RG_ENC = 0,
	RG_DEC
}RG_ENCDEC;

#define LAST 1

#define NULL 0

void printbyte(unsigned char *pData,int size )
{
	int i = 0;
	printf("\r\n");
	for(i = 0; i < size; i++)
	{
		printf("%02x",pData[i]);
	}

}

//#define DEBUG_API
void SET_IV(unsigned char *IV,int AES_OPMODE,int RG_128_256,int AES_ARIA,unsigned char *AES_ARIA_KEY, int RG_TWO_FRAME, int RG_ENC_DEC)
{
	int i;
	//int j;
	//unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	//unsigned char rx_data[64];	   
	//int success = 1;
	//unsigned char KEY_BUFFER[32];
	volatile unsigned char *data;
	int key_length = 0;
	if(RG_256 == RG_128_256)
		key_length = 32;
	else
		key_length = 16;

	if(NULL == AES_ARIA_KEY){
		printf("\r\nKEY IS NULL");
		return;
	}
	*RCM3_BUF_CTRL = 1;
	#if AES_DEBUG
	printf("\r\n RCM3_BUF_CTRL = 0x%02x", *RCM3_BUF_CTRL);
	#endif

	//*RCM3_EE_KEY_AES_CTRL = 0;

	#if AES_DEBUG
	printf("\r\n RCM3_EE_KEY_AES_CTRL = 0x%02x", *RCM3_EE_KEY_AES_CTRL);
	#endif
	tx_data[0] = 
//		(AES_OPMODE<<4)|
//		(RG_TWO_FRAME<<3)|
		(AES_OPMODE<<3)|
		(RG_128_256<<1)|
		AES_ARIA;
	*RCM3_AES_CTRL	= tx_data[0];	
	#if AES_DEBUG
	printf("\r\n RCM3_AES_CTRL = 0x%02x", *RCM3_AES_CTRL);
	#endif
	 tx_data[0] = 0x9;
	*RCM3_ST0_OPMODE = tx_data[0];
	#if AES_DEBUG
	printf("\r\n RCM3_ST0_OPMODE = 0x%02x", *RCM3_ST0_OPMODE);
	#endif
	if(RG_ENC == RG_ENC_DEC)	
		tx_data[0] = 0x2;	//ENC
	else	
		tx_data[0] = 0x3;	//DEC
	*RCM3_ST1_SYMCIP_OPMODE = tx_data[0];
	#if AES_DEBUG
	printf("\r\n RCM3_ST1_SYMCIP_OPMODE = 0x%02x", *RCM3_ST1_SYMCIP_OPMODE);
	#endif

#if 0	
	printf("\r\n RCM3_EE_KEY_AES_CTRL 0x%08x ", *RCM3_EE_KEY_AES_CTRL);
	printf("\r\n RCM3_AES_CTRL0x%08x ", *RCM3_AES_CTRL);
	printf("\r\n RCM3_ST0_OPMODE 0x%08x ", *RCM3_ST0_OPMODE);
	printf("\r\n RCM3_ST1_SYMCIP_OPMODE 0x%08x ", *RCM3_ST1_SYMCIP_OPMODE);
#endif
	if(NULL != IV) {
		tx_data[0] = 0x2;	
		*RCM3_ST2_SYMCIP_OPMODE	=	0x02;
		#if AES_DEBUG
		printf("\r\n RCM3_ST2_SYMCIP_OPMODE = 0x%02x", *RCM3_ST2_SYMCIP_OPMODE);
		#endif
		data = (volatile unsigned char *)0x00100320;
		*RCM3_RG_EEBUF_EN		=	0x01;	
		#if AES_DEBUG
		printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
		#endif
		for( i = 0; i < 16; i++)	
			data[i] = IV[i];
		*RCM3_RG_EEBUF_EN		=	0x0;		
		#if AES_DEBUG
		printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
		#endif
		//*RCM3_ST2_SYMCIP_OPMODE	=	0x01;	
		#if AES_DEBUG
		printf("\r\n RCM3_ST2_SYMCIP_OPMODE = 0x%02x", *RCM3_ST2_SYMCIP_OPMODE);
		#endif
	}

	if(NULL != AES_ARIA_KEY) {
		*RCM3_ST2_SYMCIP_OPMODE	=	0x03;
		#if AES_DEBUG
		printf("\r\n RCM3_ST2_SYMCIP_OPMODE = 0x%02x", *RCM3_ST2_SYMCIP_OPMODE);
		#endif
		data = (volatile unsigned char *)0x00100320;
		*RCM3_RG_EEBUF_EN		=	0x01;	
		#if AES_DEBUG
		printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
		#endif
		for( i = 0; i < key_length; i++)	
			data[i] = AES_ARIA_KEY[i];
		*RCM3_RG_EEBUF_EN		=	0x0;	
		#if AES_DEBUG
		printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
		#endif
		//*RCM3_ST2_SYMCIP_OPMODE	=	0x01;	
		#if AES_DEBUG
		printf("\r\n RCM3_ST2_SYMCIP_OPMODE = 0x%02x", *RCM3_ST2_SYMCIP_OPMODE);
		#endif
	}	
	*RCM3_ST2_SYMCIP_OPMODE	=	0x04;//RUN
	#if AES_DEBUG
	printf("\r\n RCM3_ST2_SYMCIP_OPMODE = 0x%02x", *RCM3_ST2_SYMCIP_OPMODE);
	#endif
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
#define MULTI 2

void AES_ARIA_Encrypt(unsigned char *pInput,unsigned char *pOutput, int len)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	volatile unsigned char *data;
	volatile unsigned int *data32;	
#ifdef DEBUG_API		
	printf("\r\n ENTER AES_ARIA_Encrypt");
	printf("\r\n AES_ARIA_ input");
	printbyte(pInput,16);
#endif
	//*RCM3_ST2_SYMCIP_OPMODE	=	0x04;//RUN
	//GPIO_START();
	*RCM3_RG_EEBUF_EN		=	0x01;
	#if AES_DEBUG
	printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
	#endif
	
	data = (volatile unsigned char *)0x00100300;
	for( i = 0; i < len; i++)
		data[i] = pInput[i];
	*RCM3_RG_EEBUF_EN		=	0x00;
	delay_us(10);
	#if AES_DEBUG
	printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
	#endif
	*RCM3_RG_EEBUF_EN		=	0x01;
	#if AES_DEBUG
	printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
	#endif
	
	data32 = (volatile unsigned int *)0x00100300;
	for( i = 0, j = 0; i < len/4; i++) 
	{
		pOutput[j+0] = (data32[i] >> 24) & 0xFF;
		pOutput[j+1] = (data32[i] >> 16) & 0xFF;
		pOutput[j+2] = (data32[i] >> 8) & 0xFF;
		pOutput[j+3] = (data32[i]) & 0xFF;
		//printf("\r\n data 0x%08x",data32[i]);
		j += 4;
	}
	*RCM3_RG_EEBUF_EN		=	0x00;	
	#if AES_DEBUG
	printf("\r\n RCM3_RG_EEBUF_EN = 0x%02x", *RCM3_RG_EEBUF_EN);
	#endif
	//GPIO_END();
#ifdef DEBUG_API		
	printf("\r\n AES_ARIA_ pOutput");
	printbyte(pOutput,16);
#endif	

}
void AES_ARIA_Decrypt(unsigned char *pInput,unsigned  char *pOutput, int len)
{
	int i;
	int j;
	unsigned int inst = 0;
	//unsigned char addr[2];
	unsigned char tx_data[64];
	unsigned char rx_data[64];
	volatile unsigned char *data;
	volatile unsigned int *data32;	
#ifdef DEBUG_API		
	printf("\r\n ENTER AES_ARIA_Decrypt");
	printf("\r\n AES_ARIA_ input");
	printbyte(pInput,16);
#endif
	//*RCM3_ST2_SYMCIP_OPMODE	=	0x04;//RUN
	*RCM3_RG_EEBUF_EN		=	0x01;
	data = (volatile unsigned char *)0x00100400;
	for( i = 0; i < len; i++)
		data[i] = pInput[i];
	*RCM3_RG_EEBUF_EN		=	0x00;
	delay_us(10);

	*RCM3_RG_EEBUF_EN		=	0x01;
	data32 = (volatile unsigned int *)0x00100400;
	for( i = 0, j = 0; i < len/4; i++) 
	{
		pOutput[j+0] = (data32[i] >> 24) & 0xFF;
		pOutput[j+1] = (data32[i] >> 16) & 0xFF;
		pOutput[j+2] = (data32[i] >> 8) & 0xFF;
		pOutput[j+3] = (data32[i]) & 0xFF;
		j += 4;
	}
	*RCM3_RG_EEBUF_EN		=	0x00;	
	
#ifdef DEBUG_API		
	printf("\r\n AES_ARIA_ pOutput");
	printbyte(pOutput,16);
#endif	
}




void AES_ARIA_CLOSE()
{

	*RCM3_ST2_SYMCIP_OPMODE	=	0x01;
	*RCM3_ST0_OPMODE	= 0x01;
	*RCM3_ACCESS			= 0x00;
}


int dorca3_cipher_decipher(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, unsigned int len, int type,int last)
{
	int dorca3_opmode = 0;
	int dorca3_enc_dec = 0;
	int dorca3_keylength = 0;
	int dorca3_aes_aria = 0;
	int dorca3_two_frame = 0;
	if(32 == len)
	  dorca3_two_frame = 1;
	  
	dorca3_opmode = type;
	dorca3_enc_dec = mode;
	if( 1 == arg_type)
		dorca3_aes_aria = RG_AES;
	else
		dorca3_aes_aria = RG_ARIA;
	if(32 == key_length)
		dorca3_keylength = RG_256;
	else
		dorca3_keylength = RG_128;
#ifdef DEBUG_API	
	printf("\r\n ENTER dorca3_cipher_decipher");
#endif	
	#ifdef DEBUG_API
	//#if 1
		printf("\r\n mode %d",mode);
		printf("\r\n arg_type %d",arg_type);
		if(NULL != Key) {
			printf("\r\n Key");
			printbyte(Key,key_length);
		}
		if(NULL != iv){
			printf("\r\n iv");
			printbyte(iv,16);
		}
		printf("\r\n in");
		printbyte(in,len);
		printf("\r\n len %d", len);
		printf("\r\n type %d", type);
		printf("\r\n last %d", last);
		
    printf("\r\n init_hash_subkey  key00 :"); for (int i = 0; i < 32 ; i++) printf(" %02x", Key[i]);
	printf("\r\n init_hash_subkey  H00 :"); for (int i = 0; i < 16 ; i++) printf(" %02x", in[i]);			
	#endif
	
	if(NULL != Key)
		SET_IV(iv,dorca3_opmode,dorca3_keylength,dorca3_aes_aria,Key,dorca3_two_frame,mode);

	
	if( RG_ENC == dorca3_enc_dec)
		AES_ARIA_Encrypt(in,out,len);
	else
		AES_ARIA_Decrypt(in,out,len);	
	
	
	if(last){
		AES_ARIA_CLOSE();
	}
	
#ifdef DEBUG_API		
	printf("\r\n init_hash_subkey  H01 :"); for (int i = 0; i < 16 ; i++) printf(" %02x", out[i]);	
printf("\r\n LEAVE dorca3_cipher_decipher");
#endif

	return 0;
}

int dorca3_cipher_decipher_big_size(int mode, int arg_type, unsigned char* Key, int key_length, unsigned char *iv, unsigned char *out, unsigned char *in, unsigned int len, int type,int last){
    int i = 0;
    int j = 0;
    int cycle = 0x10;
    int run =  len - cycle;
    unsigned char temp_out[128] = { 0, };
	
	//for(i = 0; i < len; i += cycle){
    for(i = 0; i < run; i += cycle){
        if(i == 0) dorca3_cipher_decipher(mode, arg_type /*AES*/, Key,key_length, iv, temp_out, in + i, cycle, type, 0);
        else dorca3_cipher_decipher(mode, arg_type /*AES*/, Key,key_length, NULL, temp_out, in + i, cycle, type, 0);
        
        for(j = 0; j < cycle; j++){
        	out[i + j] = temp_out[j];
        }
    }
	
    dorca3_cipher_decipher(mode, arg_type /*AES*/, Key,key_length, NULL, temp_out, in + i, cycle, type, 1);
	
    for(j = 0; j < cycle; j++){
		
       	out[i + j] = temp_out[j];
    }
    
    return 0;
}


void D4_AES_TEST(){
	unsigned char CT[256];
	unsigned char PT[256];	
	//unsigned char rnd_data[256];
	unsigned char IV[16] = { 0x2d, 0xfb, 0x42, 0x9a, 0x48, 0x69, 0x7c, 0x34, 0x00, 0x6d, 0xa8, 0x86, 0x01, 0x02, 0x03, 0x04 };
	unsigned char pt_org[256];
	unsigned char key_org[256] = { 0x21, 0x92, 0xB3, 0x94, 0x25, 0xBB, 0xD0, 0x8B, 0x6E, 0x8E, 0x61, 0xC5, 0xD1, 0xF1, 0xBC, 0x9F, 
								   0x42, 0x8F, 0xC5, 0x69, 0xFB, 0xC6, 0xF7, 0x8C, 0x0B, 0xC4, 0x8F, 0xCC, 0xDB, 0x0F, 0x42, 0xAE };
	unsigned char test_size = 16;
	unsigned char pt_temp[256];
	unsigned char key_temp[256];
	//const unsigned char aes_key[32] = { 0x21, 0x92, 0xB3, 0x94, 0x25, 0xBB, 0xD0, 0x8B, 0x6E, 0x8E, 0x61, 0xC5, 0xD1, 0xF1, 0xBC, 0x9F, 0x42, 0x8F, 0xC5, 0x69, 0xFB, 0xC6, 0xF7, 0x8C, 0x0B, 0xC4, 0x8F, 0xCC, 0xDB, 0x0F, 0x42, 0xAE };
	
	for(int i = 0 ; i < test_size ; i++) {
		pt_org[i] = i*16+i;
		//pt_org[i] = 0;
	}

	for(int i = 0 ; i<32; i++) {
		key_org[i] = i;
	}
//	for(int i = 0, j=3; i < test_size ; i++) {
//		pt_temp[j--] = pt_org[i];
//		j+=4;
//		//pt_org[i] = 0;
//	}	
//	for(int i = 0, j=3; i < 32 ; i++) {
//		key_temp[j--] = key_org[i];
//		j+=4;
//		//pt_org[i] = 0;
//	}
	printf("\r\n D4 AES TEST");
	printf("\r\n KEY:");printbyte(key_org,32);
	printf("\r\n PT:");printbyte(pt_org,test_size);
//	aes128e(CT, pt_org, key_org, 0, 16);
//	printf("\r\n CT_sw:");printbyte(CT,test_size);
//	
//	aes128e(pt_temp, CT, key_org, 1, 16);
//	printf("\r\n DT_sw:");printbyte(pt_temp,test_size);
		
	dorca3_cipher_decipher(RG_ENC,0 /*AES*/,key_org,32,NULL,CT,pt_org,16,MODE_ECB,LAST);		
	printf("\r\n D4 AES CT:");printbyte(CT,16);
#if 1	
	dorca3_cipher_decipher(RG_DEC,0 /*AES*/,key_org,32,NULL,PT,CT,16,MODE_ECB,LAST);		
	printf("\r\n D4 AES DT:");printbyte(PT,16);
	
	for(int i = 0; i < 16; i++){
		if(PT[i] != pt_org[i]){
			printf("\r\nMODE_ECB FAIL\r\n");
			return ;
		}
	}
	printf("\r\nMODE_ECB PASS\r\n");
	
	
	dorca3_cipher_decipher(RG_ENC, 0, key_org, 32, IV, CT, pt_org, 16, MODE_CBC, LAST);
	printf("\r\n D4 AES CT:");printbyte(CT,16);
	dorca3_cipher_decipher(RG_DEC, 0, key_org, 32, IV, PT, CT, 16, MODE_CBC, LAST);
	printf("\r\n D4 AES DT:");printbyte(PT,16);
	for(int i = 0; i < 16; i++){
		if(PT[i] != pt_org[i]){
			printf("\r\nMODE_CBC FAIL\r\n");
			return ;
		}
	}
	printf("\r\nMODE_CBC PASS\r\n");
	
	dorca3_cipher_decipher(RG_ENC, 0, key_org, 32, IV, CT, pt_org, 16, MODE_OFB, LAST);
	printf("\r\n D4 AES CT:");printbyte(CT,16);
	dorca3_cipher_decipher(RG_DEC, 0, key_org, 32, IV, PT, CT, 16, MODE_OFB, LAST);
	printf("\r\n D4 AES DT:");printbyte(PT,16);
	for(int i = 0; i < 16; i++){
		if(PT[i] != pt_org[i]){
			printf("\r\n MODE_OFB FAIL\r\n");
			return ;
		}
	}
	printf("\r\n MODE_OFB PASS\r\n");
	
	dorca3_cipher_decipher(RG_ENC, 0, key_org, 32, IV, CT, pt_org, 16, MODE_CTR, LAST);
	printf("\r\n D4 AES CT:");printbyte(CT,16);
	dorca3_cipher_decipher(RG_DEC, 0, key_org, 32, IV, PT, CT, 16, MODE_CTR, LAST);
	printf("\r\n D4 AES DT:");printbyte(PT,16);
	for(int i = 0; i < 16; i++){
		if(PT[i] != pt_org[i]){
			printf("\r\n MODE_CTR FAIL\r\n");
			return ;
		}
	}
	printf("\r\n MODE_CTR PASS\r\n");
	
	dorca3_cipher_decipher(RG_ENC, 0, key_org, 32, IV, CT, pt_org, 16, MODE_CFB, LAST);
	printf("\r\n D4 AES CT:");printbyte(CT,16);
	dorca3_cipher_decipher(RG_DEC, 0, key_org, 32, IV, PT, CT, 16, MODE_CFB, LAST);
	printf("\r\n D4 AES DT:");printbyte(PT,16);
	for(int i = 0; i < 16; i++){
		if(PT[i] != pt_org[i]){
			printf("\r\n MODE_CFB FAIL\r\n");
			return ;
		}
	}
	printf("\r\n MODE_CFB PASS\r\n");
#endif
//	if(memcmp(pt_org, PT,16) == 0){
//		printf("\r\n D4_AES_TEST TEST PASS!!\n");
//	}
//	else {
//		printf("\r\n D4_AES_TEST TEST FAIL!!\n");
//	}
}
