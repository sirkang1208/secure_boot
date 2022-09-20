#include <sha256_t.h>


void sha256_org(unsigned int* msg, unsigned int* hash_msg){
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void sha256_init(SHA256_CTX* ctx)
{
    int i;  
    for (i = 0; i < 64; ++i) ctx->data[i] = 0; 
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->mac_flag = 0;
	
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

void sha256_transform(SHA256_CTX* ctx, const uint8_t* data){
	int i,j;
	uint32_t msg[16];
	
	for( i=0; i<16; i++){
		msg[i] = ((uint32_t) data[i*4+0] << 24) |
		         ((uint32_t) data[i*4+1] << 16) |
		         ((uint32_t) data[i*4+2] <<  8) |
		         ((uint32_t) data[i*4+3] <<  0) ;
	}
	
	*SHA256_TEXT_EN      = 0x00000001;   //TEXT_EN
	
	for(i = 0; i < 16; i++){
		*SHA256_TEXT_DAT = msg[i];
	}

	*SHA256_TEXT_EN      = 0x00000000;   //TEXT_EN
	*SHA256_CMD_W_I      = 0x00000001;
	*SHA256_CMD_W_I      = 0x00000000;	 //SHA RUN Tic
	
//	while((*SHA256_STATUS) != 1){
//	
//		printf("\r\n SHA256_STATUS = %x", *SHA256_STATUS);
//	}
//	*SHA256_IRQ_CLR = 0x01;
	
}

void sha256_update(SHA256_CTX* ctx, const uint8_t* data, size_t len)
{
	uint32_t i, round = 0;
	
//	printf("\r\n len: %d", len);
//	printf("\r\n datalen: %d", ctx->datalen);
//	printf("\r\n data: ");
//	for (i = 0; i < len; ++i){ if((i>0) && (i%16==0)) printf("\r\n\t");  printf(" 0x%02x", data[i]);}
		
	for (i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		
		if (ctx->datalen == 64) {
//			printf("\r\n ctx->data: ");
//			for (int j = 0; j < ctx->datalen; ++j){ if((j>0) && (j%16==0)) printf("\r\n\t");  printf(" 0x%02x", ctx->data[j]);}
			sha256_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
			if(len >= 56)round++;
		}else{
			if(round != 0) 	{
				//printf("\r\n round : %d", round);
				*SHA256_CMD_I      = 0x00000006;
			}
		}
		
	}
}

void sha256_final(SHA256_CTX* ctx, uint8_t hash[])
{
	uint32_t i;

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
		memset(ctx->data, 0, 64);
		*SHA256_CMD_I      = 0x00000006;
	}
	if(ctx->mac_flag != 0) *SHA256_CMD_I      = 0x00000006;

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

int sha256_test(){
	#if 1
	uint8_t message[208] = {	0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21,
								0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21
							};
	uint32_t datasize = 208;
//	uint8_t message[56] = { 0x61, 0x62, 0x63, 0x64, 0x62, 0x63, 0x64, 0x65, 0x63, 0x64, 0x65, 0x66, 0x64, 0x65, 0x66, 0x67, 0x65, 0x66, 0x67, 0x68, 0x66, 0x67, 0x68, 0x69, 0x67, 0x68, 0x69, 0x6A, 
//                            0x68, 0x69, 0x6A, 0x6B, 0x69, 0x6A, 0x6B, 0x6C, 0x6A, 0x6B, 0x6C, 0x6D, 0x6B, 0x6C, 0x6D, 0x6E, 0x6C, 0x6D, 0x6E, 0x6F, 0x6D, 0x6E, 0x6F, 0x70, 0x6E, 0x6F, 0x70, 0x71 };
//	uint32_t datasize = 56;
	#elif 0
	//uint8_t message[68] ={ 0x33 ,0x38 ,0xa0 ,0x1c ,0x97 ,0x15 ,0x3c ,0x41 ,0x19 ,0x18 ,0xcd ,0xc2 ,0x04 ,0x08 ,0x32 ,0x12 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x64 ,0x61 ,0x74 ,0x61};
	uint8_t message[68] = { 0xe2 ,0x38 ,0xa0 ,0x1c ,0x97 ,0x15 ,0x3c ,0x41 ,0x19 ,0x18 ,0xcd ,0xc2 ,0x04 ,0x08 ,0x32 ,0x12 
						   ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 
		                   ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 
		                   ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 ,0x36 
						   ,0x64 ,0x61 ,0x74 ,0x61};
	uint32_t datasize = 57;
	#elif 0
	uint8_t message[8] = { 0x61, 0x73, 0x64, 0x66, 0x61, 0x73, 0x64, 0x66 }; //asdfasdf
	uint32_t datasize = 8;
	#endif
						   
//	uint8_t message[8] = { 0x61, 0x73, 0x64, 0x66, 0x61, 0x73, 0x64, 0x66 }; //asdfasdf						   
	int i,j = 0;
	uint8_t hw_buf[32];
	uint8_t sw_buf[32];
	SHA256_CTX ctx;  
		
//	sha256_init(&ctx);  
//	sha256_update(&ctx, message, 3);
//	sha256_final(&ctx, buf);

//	printf("\r\nSHA256 RESULT Single Frame: ");
//	for(i = 0; i < 32; i++){
//		printf("%02x ", buf[i]);
//	}
//	printf("\r\n");
						   
	sha256_init(&ctx);  
	sha256_update(&ctx, message, datasize);
	sha256_final(&ctx, hw_buf);
		
	printf("\r\nSHA256 RESULT Multie Frame: ");
	for(i = 0; i < 32; i++){
		printf("%02x ", hw_buf[i]);
	}
	printf("\r\n");
	
//	sha256_initialize(&ctx);  
//	sha256_update_sw(&ctx, message, datasize);
//	sha256_finalize(&ctx, buf);
	
	sha256_get(sw_buf, message, datasize);
		
	printf("\r\nSHA256 SW Multie Frame: ");
	for(i = 0; i < 32; i++){
		printf("%02x ", sw_buf[i]);
	}
	printf("\r\n");
	
	if(memcmp(hw_buf, sw_buf, 32)){ printf("\r\n SHA TEST FAIL!!!"); return 1;}
	else printf("\r\n SHA TEST PASS");
	
	printf("\r\n=======================================================================\n");
	
	return 0;
}



void sha256_hmac(unsigned char key[], int length, char message[],int dlength, uint8_t* output){ 
	//int msgLen = strlen(message); //get the length of the message to be encrypted 
	int msgLen = dlength; //get the length of the message to be encrypted 
	char keyFinal[BLOCK_SIZE] = {0}; //setup array for the data to go into 
	int i;
	char oKeyPad[BLOCK_SIZE] = {0}; //setup the oKeyPad 
	char iKeyPad[BLOCK_SIZE] = {0}; //setup the ikeypad 
	char iandmesg[BLOCK_SIZE+MAX_SHA]; //setup the inner hash ikeypad concat with message 
	unsigned char hash_iandmesg[HASH_LEN] = {0};  //get ready to get bytes back from the hashing function 
	unsigned char oandihash[(BLOCK_SIZE + HASH_LEN)]; //setup the outter hash, okeypad + (hash of ikeypad + message) 

	SHA256_CTX sha;
	uint8_t *msg;
	unsigned long msg_len;
	uint8_t digest[140] = {0x0};
	uint8_t  one_block_message[140];
	unsigned long  one_block_message_len = 0;


	if(length > BLOCK_SIZE){   //if the given data is too long, hash it 
		for( i=0; i<length; i++) one_block_message[i] = key[i];

		//one_block_message[140] = {0x61,0x62,0x63};
		one_block_message_len = length;

		msg = (uint8_t*)one_block_message;
		msg_len = one_block_message_len;

		sha256_init(&sha);
		sha.mac_flag = 1;
		sha256_update(&sha, msg, msg_len);
		sha256_final(&sha, digest);		
		for( i=0; i<32; i++) key[i] = digest[i];
		length = HASH_LEN;
	} 
    if(length < BLOCK_SIZE){   //if the given data is too short, pad it with 0x00 
     for(i = 0; i < BLOCK_SIZE; i++){ 
      if(i < length){    //read in the data 
       keyFinal[i] = key[i]; 
      }else{      //if there is no more data to read, read in zeros 
       keyFinal[i] = 0x00; 
      } 
     } 
    } 
    if(length == BLOCK_SIZE){  //if the given data is the right size, transfer it to keyFinal 
     for(i = 0; i < BLOCK_SIZE; i++){ 
      keyFinal[i] = key[i]; 
     } 
    } 

   
    for(i = 0; i < BLOCK_SIZE; i++){ //for each item in key final, xor it with O_KEY_PAD and I_KEY_PAD 
     oKeyPad[i] = keyFinal[i]^O_KEY_PAD; 
     iKeyPad[i] = keyFinal[i]^I_KEY_PAD; 
    } 

    //make the message to be hashed, ikeypad concatinated with message 
    for(i = 0; i < BLOCK_SIZE; i++){ //read in ikeypad 
     iandmesg[i] = iKeyPad[i]; 
    } 
    for(i = BLOCK_SIZE; i < (msgLen + BLOCK_SIZE); i++){ //read in message 
     iandmesg[i] = message[i-BLOCK_SIZE]; 
    } 


	for( i=0; i<msgLen+BLOCK_SIZE; i++) one_block_message[i] = iandmesg[i];

	//one_block_message[140] = {0x61,0x62,0x63};
	one_block_message_len = (msgLen+BLOCK_SIZE);

	msg = (uint8_t*)one_block_message;
	
	msg_len = one_block_message_len;

	//printf("\r\n msg_len : %d \r\n msg : ", msg_len);  for(i=0; i<msg_len; i++) printf(" ,0x%02x", msg[i]);
	
	sha256_init(&sha);
	sha.mac_flag = 1;
	sha256_update(&sha, msg, msg_len);
	sha256_final(&sha, hash_iandmesg);			

    //make the message to be hashed, okeypad concatinated with the hash of (ikeypad + message) 
    for(i = 0; i < BLOCK_SIZE; i++){ //read in okeypad 
     oandihash[i] = oKeyPad[i]; 
    } 
    for(i = BLOCK_SIZE; i < (BLOCK_SIZE + HASH_LEN); i++){ //read in hash of ikeypad + message 
     oandihash[i] = hash_iandmesg[i-BLOCK_SIZE]; 
    } 

    //return the result of the hash of (okeypad + hash(ikeypad + message)) 

	for( i=0; i<(BLOCK_SIZE + HASH_LEN); i++) one_block_message[i] = oandihash[i];
	one_block_message_len = (BLOCK_SIZE + HASH_LEN);

	msg = (uint8_t*)one_block_message;
	msg_len = one_block_message_len;

	sha256_init(&sha);
	sha.mac_flag = 1;
	sha256_update(&sha, msg, msg_len);
	sha256_final(&sha, digest);		
	//		for( i=0; i<32; i++) SHA_HASH_DATA[i] = digest[i];
	memcpy(output, digest, 32);
	printf("\r\n hmac : "); for( i=0; i<32; i++) printf(" %02x", output[i]);
	
}

void sha256_hmac_test(uint8_t* result){
	uint8_t key[3] ={ 0x6b, 0x65, 0x079};  //Str print : key
//	uint8_t key[16] = { 0x6d, 0x7d, 0xc4, 0xf8, 0x3d, 0x76, 0xc7, 0x0a, 
//					   0x46, 0x74, 0x72, 0x52, 0xd5, 0x25, 0x1c, 0x99};
	int keylength = 3;	
	char message[4] = { 0x64, 0x61, 0x74, 0x61, }; //Str print : data
	int datalength = 4;
	int i,j;
	
	uint8_t output[32];
	
	sha256_hmac(key, keylength, message, datalength, output);
	
	memcpy(result, output, 32);
}
