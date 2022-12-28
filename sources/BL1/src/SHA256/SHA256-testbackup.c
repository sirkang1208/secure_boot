#include "cm0ik_reg.h"

void sha256_org(unsigned int *msg, unsigned int *hash_msg){
	int i = 0;
	uint32_t temp[16] = { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0x000001C0 };
	
	printf("\r\nSHA256 MODE MSG INPUT TEST\r\n");

	*SHA256_WR_ADDR      = 0x7B80;   //0x2001EE00
	
	
	*SHA256_CMD_I         = 0x00000002; // sha256 1fram State 512bit init(sha256_initialize function)

	
	*SHA256_TEXT_EN      = 0x00000001;   //TEXT_EN
	
	for(i = 0; i < 16; i++){
		*SHA256_TEXT_DAT = msg[i];
	}
	
	 *SHA256_TEXT_EN      = 0x00000000;   //TEXT_EN
	
	//*SHA256_CMD_I         = 0x00000002;
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
	
	#if 0	
	*SHA256_CMD_I      = 0x00000006;
	*SHA256_TEXT_EN      = 0x00000001;   //TEXT_EN
	for(i = 0; i < 16; i++){
		*SHA256_TEXT_DAT = temp[i];
	}


    *SHA256_TEXT_EN      = 0x00000000;   //TEXT_EN
   
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
 	#endif  
	
    *SHA256_CMD_I         = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
	
	while(*SHA256_STATUS != 0x1){ };

	*SHA256_IRQ_CLR      = 0x00000001;
	
	for(i = 0; i < 8; i++){
		hash_msg[7-i] = *(SHA256_DATA+i);
	}
	
	#if 1 //def DEBUG
	
	printf("\r\nSHA256 RESULT : ");
	for(i = 0; i < 8; i++){
		printf("%08x ", hash_msg[i]);
	}
	printf("\r\n");
	
	printf("SHA256_TEST_EXPECT_RESULT : 256'h248d6a61_d20638b8_e5c02693_0c3e6039_a33ce459_64ff2167_f6ecedd4_19db06c1 \r\n");
	
	return ;
	
	#endif
}


void sha256_test_org(){
	//unsigned int msg[16] = { 0x61626364, 0x62636465, 0x63646566, 0x64656667, 0x65666768, 0x66676869, 0x6768696A,    0x68696A6B, 0x696A6B6C, 0x6A6B6C6D, 0x6B6C6D6E, 0x6C6D6E6F, 0x6D6E6F70, 0x6E6F7071, 0x80000000, 0x00000000};
	unsigned int sha_result[8] = { 0, };
	unsigned int msg1[16] = { 0x61626380, 0x0, 0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x00000018};
	uint32_t i=0;	
	
	printf("\r\nSHA_TEST");
	sha256_org(msg1, sha_result);
	
	printf("\r\nSHA256 RESULT Re : ");
	for(i = 0; i < 8; i++){
		printf("%08x ", sha_result[i]);
	}
	printf("\r\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0 // TEST 1
typedef struct{
	uint8_t		hash[32];	// Changed by RKW, unsigned char becomes uint8_t  
	uint32_t	buffer[16];	// Changed by RKW, unsigned long becomes uint32_t 
	uint32_t	state[8];	// Changed by RKW, unsigned long becomes uint32_t 
	uint8_t		length[8];	// Changed by RKW, unsigned char becomes uint8_t  
} sha256;

void sha256_initialize(sha256 *sha) {  
    int i;  
    for (i = 0; i < 17; ++i) sha->buffer[i] = 0; 
	
	*SHA256_WR_ADDR      = 0x7B80;   //0x2001EE00

	*SHA256_CMD_I         = 0x00000002; // sha->state 512bit init(sha256_initialize function)
//    sha->state[0] = 0x6a09e667;  // SHA256_CMD_I init value
//    sha->state[1] = 0xbb67ae85;  // SHA256_CMD_I init value
//    sha->state[2] = 0x3c6ef372;  // SHA256_CMD_I init value
//    sha->state[3] = 0xa54ff53a;  // SHA256_CMD_I init value
//    sha->state[4] = 0x510e527f;  // SHA256_CMD_I init value
//    sha->state[5] = 0x9b05688c;  // SHA256_CMD_I init value
//    sha->state[6] = 0x1f83d9ab;  // SHA256_CMD_I init value
//    sha->state[7] = 0x5be0cd19;  // SHA256_CMD_I init value
	
    for (i = 0; i < 8; ++i) sha->length[i] = 0;  
}  

//  Changed by RKW, formal args are now const uint8_t, uint_32  
//    from const unsigned char, unsigned long respectively  
void sha256_update(sha256 *sha, const uint8_t *message, uint32_t length){
	int i, j;  
    /* Add the length of the received message, counted in 
     * bytes, to the total length of the messages hashed to 
     * date, counted in bits and stored in 8 separate bytes. */ 
	   /* Add the length of the received message, counted in 
     * bytes, to the total length of the messages hashed to 
     * date, counted in bits and stored in 8 separate bytes. */  
	uint32_t msg[16];
	
	for( i=0; i<16; i++){
		msg[i] = ((unsigned int) message[i*4+0] << 24) |
		         ((unsigned int) message[i*4+1] << 16) |
		         ((unsigned int) message[i*4+2] <<  8) |
		         ((unsigned int) message[i*4+3] <<  0) ;
	}
	
	*SHA256_TEXT_EN      = 0x00000001;   //TEXT_EN

	
	for(i = 0; i < 16; i++){
		*SHA256_TEXT_DAT = msg[i];
	}

	*SHA256_TEXT_EN      = 0x00000000;   //TEXT_EN
	*SHA256_CMD_W_I      = 0x00000001;
	*SHA256_CMD_W_I      = 0x00000000;	 //SHA RUN Tic


}

//  Changed by RKW, formal args are now const uint8_t, uint_32  
//    from const unsigned char, unsigned long respectively  
#if 1
void sha256_finalize(sha256 *sha, const uint8_t *message, uint32_t length) {  
    int i,j;  
	uint32_t result[64];
	uint8_t msg_temp[64];
	
    // Changed by RKW, unsigned char becomes uint8_t  
    //uint8_t terminator[64 + 8] = { 0x80 };  
	uint8_t terminator[64 + 8] = { 0x00, };  
	
    for (i = 7; i >= 0; --i) {  
        int bits;  
        if (i == 7)  
            bits = length << 3;  
        else if (i == 0 || i == 1 || i == 2)  
            bits = 0;  
        else  
            bits = length >> (53 - 8 * i);  
        bits &= 0xff;  
        if (sha->length[i] + bits > 0xff) {  
            for (j = i - 1; j >= 0 && sha->length[j]++ == 0xff; --j);  
        }  
        sha->length[i] += bits;  
    }  	
  
	memset(msg_temp, 0, 64);
	for( i=0; i<length; i++) msg_temp[i] = message[i];
	msg_temp[length] = 0x80;
	
	sha256_update(sha, msg_temp, 64);  
	
	length = 64 - sha->length[6] % 2 * 32 - sha->length[7] / 8;  
	//if (length < 9) length += 64;  
	//for (i = 0; i < 8; ++i) terminator[length - 8 + i] = sha->length[i];  
	if (length < 9) length += (64-length);  
	for (i = 0; i < 8; i++) terminator[length - 8 + i] = sha->length[i];  
	/* Hash the terminator to finalize the message digest. */ 
	*SHA256_CMD_I      = 0x00000006;	
	sha256_update(sha, terminator, length);  
	
	// HASH result Read start
    *SHA256_CMD_I        = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
	
	while(*SHA256_STATUS != 0x1){ };

	*SHA256_IRQ_CLR      = 0x00000001;
	
	for(i = 0; i < 8; i++){
		result[7-i] = *(SHA256_DATA+i);
	}
		printf("\r\nSHA256 RESULT : ");
	for(i = 0; i < 8; i++){
		printf("%08x ", result[i]);
	}
	printf("\r\n");
	
	printf("SHA256_TEST_EXPECT_RESULT : 256'h248d6a61_d20638b8_e5c02693_0c3e6039_a33ce459_64ff2167_f6ecedd4_19db06c1 \r\n");
		
}  
#else
void sha256_finalize(sha256 *sha, const uint8_t *message, uint32_t length) {  
    int i,j;  
	uint32_t result[64];
	uint8_t msg_temp[64];
	
    // Changed by RKW, unsigned char becomes uint8_t  
    //uint8_t terminator[64 + 8] = { 0x80 };  
	uint8_t terminator[64 + 8] = { 0x00, };  
	
    for (i = 7; i >= 0; --i) {  
        int bits;  
        if (i == 7)  
            bits = length << 3;  
        else if (i == 0 || i == 1 || i == 2)  
            bits = 0;  
        else  
            bits = length >> (53 - 8 * i);  
        bits &= 0xff;  
        if (sha->length[i] + bits > 0xff) {  
            for (j = i - 1; j >= 0 && sha->length[j]++ == 0xff; --j);  
        }  
        sha->length[i] += bits;  
    }  	
    /* Hash the final message bytes if necessary. */  
    if (length > 0){ 
		if( length < 64 ){
			memset(msg_temp, 0, 64);
			for( i=0; i<length; i++) msg_temp[i] = message[i];
			msg_temp[length] = 0x80;
			if( length <= 55){
				msg_temp[62] = (length * 8) >> 8;
				msg_temp[63] = length * 8 & 0xf8;
			}
			sha256_update(sha, msg_temp, 64);  
		}
//		else{
//			sha256_update(sha, message, length);  
//		}
	}
		#if 0
		/* Create a terminator that includes a stop bit, padding, and 
		 * the the total message length. See FIPS 180-2 for details. */  
		length = 64 - sha->length[6] % 2 * 32 - sha->length[7] / 8;  
		//if (length < 9) length += 64;  
		//for (i = 0; i < 8; ++i) terminator[length - 8 + i] = sha->length[i];  
		if (length < 9) length += (64-length);  
		for (i = 0; i < 8; i++) terminator[length - 8 + i] = sha->length[i];  
		/* Hash the terminator to finalize the message digest. */ 
		*SHA256_CMD_I      = 0x00000006;	
		sha256_update(sha, terminator, length);  
		/* Extract the message digest. */  
		//    for (i = 0; i < 32; ++i) {  
		//        sha->hash[i] = (sha->state[i / 4] >> (24 - 8 * (i % 4))) & 0xff;  
		//    }  
		#endif
    *SHA256_CMD_I        = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
	
	while(*SHA256_STATUS != 0x1){ };

	*SHA256_IRQ_CLR      = 0x00000001;
	
	for(i = 0; i < 8; i++){
		result[7-i] = *(SHA256_DATA+i);
	}
		printf("\r\nSHA256 RESULT : ");
	for(i = 0; i < 8; i++){
		printf("%08x ", result[i]);
	}
	printf("\r\n");
	
	printf("SHA256_TEST_EXPECT_RESULT : 256'h248d6a61_d20638b8_e5c02693_0c3e6039_a33ce459_64ff2167_f6ecedd4_19db06c1 \r\n");
		
}  
#endif

void sha256_test(){
	uint32_t msg1[16] = { 0x61626364, 0x62636465, 0x63646566, 0x64656667, 0x65666768, 0x66676869, 0x6768696A, 0x68696A6B, 0x696A6B6C, 0x6A6B6C6D, 0x6B6C6D6E, 0x6C6D6E6F, 0x6D6E6F70, 0x6E6F7071, 0x80000000, 0x00000000};
	uint32_t msg2[16] = { 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0x000001C0 };

	uint32_t sha_result[8] = { 0, };
	uint8_t message[56] = { 0x61, 0x62, 0x63, 0x64, 0x62, 0x63, 0x64, 0x65, 0x63, 0x64, 0x65, 0x66, 0x64, 0x65, 0x66, 0x67, 0x65, 0x66, 0x67, 0x68, 0x66, 0x67, 0x68, 0x69, 0x67, 0x68, 0x69, 0x6A, 
                            0x68, 0x69, 0x6A, 0x6B, 0x69, 0x6A, 0x6B, 0x6C, 0x6A, 0x6B, 0x6C, 0x6D, 0x6B, 0x6C, 0x6D, 0x6E, 0x6C, 0x6D, 0x6E, 0x6F, 0x6D, 0x6E, 0x6F, 0x70, 0x6E, 0x6F, 0x70, 0x71 };
	uint32_t datasize = 56;
	int i,j = 0;
	
	sha256 sha;  
	sha256_initialize(&sha);  
	sha256_finalize(&sha, message, datasize);
	
}
#endif //eof TEST 1 


#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef uint8_t BYTE;             // 8-bit byte
typedef uint32_t  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx)
{
    int i;  
    for (i = 0; i < 64; ++i) ctx->data[i] = 0; 
	ctx->datalen = 0;
	ctx->bitlen = 0;
	
	*SHA256_WR_ADDR      = 0x7B80;   //0x2001EE00

	*SHA256_CMD_I         = 0x00000002; // sha->state 512bit init(sha256_initialize function)
//    sha->state[0] = 0x6a09e667;  // SHA256_CMD_I init value
//    sha->state[1] = 0xbb67ae85;  // SHA256_CMD_I init value
//    sha->state[2] = 0x3c6ef372;  // SHA256_CMD_I init value
//    sha->state[3] = 0xa54ff53a;  // SHA256_CMD_I init value
//    sha->state[4] = 0x510e527f;  // SHA256_CMD_I init value
//    sha->state[5] = 0x9b05688c;  // SHA256_CMD_I init value
//    sha->state[6] = 0x1f83d9ab;  // SHA256_CMD_I init value
//    sha->state[7] = 0x5be0cd19;  // SHA256_CMD_I init value
}

void sha256_transform(SHA256_CTX *ctx, const BYTE *data){
	int i,j;
	uint32_t msg[16];
	
	for( i=0; i<16; i++){
		msg[i] = ((unsigned int) data[i*4+0] << 24) |
		         ((unsigned int) data[i*4+1] << 16) |
		         ((unsigned int) data[i*4+2] <<  8) |
		         ((unsigned int) data[i*4+3] <<  0) ;
	}
	
	*SHA256_TEXT_EN      = 0x00000001;   //TEXT_EN

	
	for(i = 0; i < 16; i++){
		*SHA256_TEXT_DAT = msg[i];
	}

	*SHA256_TEXT_EN      = 0x00000000;   //TEXT_EN
	*SHA256_CMD_W_I      = 0x00000001;
	*SHA256_CMD_W_I      = 0x00000000;	 //SHA RUN Tic
}

void sha256_update(SHA256_CTX *ctx, const BYTE *data, size_t len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		
		if (ctx->datalen == 64) {
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	WORD i;

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;

		sha256_transform(ctx, ctx->data);
		*SHA256_CMD_I      = 0x00000006;
		memset(ctx->data, 0, 56);
	}
	
	// Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha256_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	*SHA256_CMD_I        = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000001;
    *SHA256_CMD_W_I      = 0x00000000;
	
	while(*SHA256_STATUS != 0x1){ };

	*SHA256_IRQ_CLR      = 0x00000001;
	
	for(i = 0; i < 8; i++){
		ctx->state[7-i] = *(SHA256_DATA+i);
	}
		
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void sha256_test(){
	uint8_t message[56] = { 0x61, 0x62, 0x63, 0x64, 0x62, 0x63, 0x64, 0x65, 0x63, 0x64, 0x65, 0x66, 0x64, 0x65, 0x66, 0x67, 0x65, 0x66, 0x67, 0x68, 0x66, 0x67, 0x68, 0x69, 0x67, 0x68, 0x69, 0x6A, 
                            0x68, 0x69, 0x6A, 0x6B, 0x69, 0x6A, 0x6B, 0x6C, 0x6A, 0x6B, 0x6C, 0x6D, 0x6B, 0x6C, 0x6D, 0x6E, 0x6C, 0x6D, 0x6E, 0x6F, 0x6D, 0x6E, 0x6F, 0x70, 0x6E, 0x6F, 0x70, 0x71 };
	uint32_t datasize = 56;
	int i,j = 0;
	BYTE buf[32];
	SHA256_CTX ctx;  
		
	sha256_init(&ctx);  
	sha256_update(&ctx, message, datasize);
	sha256_final(&ctx, buf);
		
	printf("\r\nSHA256 RESULT : ");
	for(i = 0; i < 32; i++){
		printf("%02x ", buf[i]);
	}
	printf("\r\n");
	
}


void test(){
	
}
	
