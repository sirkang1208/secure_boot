//#include <iostream>
#include <string.h>
#include "aes128e.h"
#include "aes128gcm.h"


//암호 키
  const unsigned char aes_key[32] = { 0x21, 0x92, 0xB3, 0x94, 0x25, 0xBB, 0xD0, 0x8B, 0x6E, 0x8E, 0x61, 0xC5, 0xD1, 0xF1, 0xBC, 0x9F, 0x42, 0x8F, 0xC5, 0x69, 0xFB, 0xC6, 0xF7, 0x8C, 0x0B, 0xC4, 0x8F, 0xCC, 0xDB, 0x0F, 0x42, 0xAE };
//const unsigned char aes_key[16] = { 0x98, 0xff, 0xf6, 0x7e, 0x64, 0xe4, 0x6b, 0xe5, 0xee, 0x2e, 0x05, 0xcc, 0x9a, 0xf6, 0xd0, 0x12 };
//1회용 초기화 키 Nonce
//  const unsigned char IV[16] = { 0xE1, 0xE5, 0x92, 0xE8, 0x72, 0x25, 0x84, 0x7C, 0x11, 0xD9, 0x48, 0x68, 0x4F, 0x3B, 0x07, 0x0D };
const unsigned char aes_IV[12] = { 0x2d, 0xfb, 0x42, 0x9a, 0x48, 0x69, 0x7c, 0x34, 0x00, 0x6d, 0xa8, 0x86 };


//평문 데이터
 const unsigned char aes_plaintext[32] =  "Test encryption and decryption";
//const unsigned char plaintext[16] = "Hello world";

//추가 인증 데이터 Additional authenticated data
const unsigned char aes_add_data[16] = { 0xa0, 0xca, 0x58, 0x61, 0xc0, 0x22, 0x6c, 0x5b, 0x5a, 0x65, 0x14, 0xc8, 0x2b, 0x77, 0x81, 0x5a};
									// 0xa0, 0xca, 0x58, 0x61, 0xc0, 0x22, 0x6c, 0x5b, 0x5a, 0x65, 0x14, 0xc8, 0x2b, 0x77, 0x81, 0x5a};

//tag 검증용 데이터 GMAC
const unsigned char aes_tag_ref[16] = { 0xE9, 0x11, 0x99, 0x69, 0x98, 0xAD, 0xFA, 0x61, 0x2E, 0xFF, 0x09, 0x71, 0x01, 0x33, 0x6C, 0x1B };

//암호화 검증용 데이터
const unsigned char aes_ciphertext_ref[16] = { 0xA3, 0xF3, 0x99, 0x95, 0xE5, 0xF4, 0x2B, 0xD1, 0x25, 0x35, 0x12, 0x09, 0xC8, 0x94, 0xDD, 0x1D };


void aes_gcm_test()
{
	//암호화 데이터 저장용
	unsigned char ciphertext[1024];
	//복호화 데이터 저장용
	unsigned char decrypted_plaintext[1024];
	//암호화시 만들어지는 TAG(GMAC)
	unsigned char tag[16];
	//복호화시 만들어지는 TAG(GMAC)
	unsigned char decrypted_tag[1024];
	//암호화 블럭 크기 1 = 16바이트
	unsigned int block_len = 2;
	//추가 인증 데이터 크기 블럭단위이며 1블럭은 16바이트
	unsigned int add_len = 1;

	printf("\r\n========================================\n");
	printf("\r\nEncrpytion :");
	printf("\r\n----------------------------------------\n");

	//암호화 시작!!
	aes_gcm_enc(ciphertext, tag, aes_key, aes_IV, aes_plaintext, block_len, aes_add_data, add_len);
	//aes128gcm_enc(ciphertext, tag, key, IV, plaintext, block_len, add_data, add_len);
	printf("\r\n pt : ");
	for (int i = 0; i < 16 * block_len; i++) {
		printf("0x%02X ", aes_plaintext[i]);
	}
	printf("\n");
	//암호화 데이터 출력
	//printf("\r\nciphertext %s ", !compare(ciphertext, ciphertext_ref, (block_len) * 16) ? "PASS" : "FAIL");
	printf("\r\nciphertext : ");
	for (int i = 0; i < 16 * block_len; i++) {
		printf("0x%02X ", ciphertext[i]);
	}
	printf("\n");
	
	printf("\r\ntag : ");
	for (int i = 0; i < 16; i++) {
		printf("0x%02X ", tag[i]);
	}
	printf("\n");
	
#if 1
	printf("\r\n========================================");
	printf("\r\nDecrpytion :");
	printf("\r\n----------------------------------------\n");

	//복호화 시작!!
	aes_gcm_dec(decrypted_plaintext, decrypted_tag, aes_key, aes_IV, ciphertext, block_len, aes_add_data, add_len);
	printf("\r\nciphertext 01: ");
	for (int i = 0; i < 16 * block_len; i++) {
		printf("0x%02X ", ciphertext[i]);
	}
	//복호화 데이터 출력	
	//printf("\r\ndecrypted_plaintext result : %s\n", compare(decrypted_plaintext, plaintext, (block_len) * 16) == 0 ? "PASS" : "FAIL");
	printf("\r\nSTRING : %s \n", decrypted_plaintext);
	printf("\r\ndecrypted_plaintext : ");
	for (int i = 0; i < 16 * block_len; i++) {
		printf("0x%02X ", decrypted_plaintext[i]);
	}
	printf("\n\n");

	// 복호화시 생성된 TAG와 위에서 암호화시 생성된 TAG를 비교 
	// 만약 여기서 TAG가 다르다면 변조된 데이터!!!
	//printf("\r\ndecrypted_tag result : %s\n", compare(tag, decrypted_tag, 16) == 0 ? "PASS" : "FAIL");
	// 복호화시 생성된 TAG 출력
	if(compare(tag, decrypted_tag, 16) == 0 ) printf("\r\n PASS");
	else printf("\r\n FAIL");
	printf("\r\ndecrypted_tag : ");
	for (int i = 0; i < 16; i++) {
		printf("0x%02X ", decrypted_tag[i]);
	}
#endif
	printf("\n");

}