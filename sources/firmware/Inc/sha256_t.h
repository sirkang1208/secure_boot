#include "cm0ik_reg.h"

#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

typedef struct {
	uint8_t data[64];
	uint32_t datalen;
	unsigned long long bitlen;
	uint32_t state[8];
	uint8_t mac_flag;
} SHA256_CTX;

//AMI HMAC
#define BLOCK_SIZE 64 
#define O_KEY_PAD  0x5C 
#define I_KEY_PAD  0x36 
#define MAX_SHA    256
#define HASH_LEN   32

void sha256_init(SHA256_CTX* ctx);
void sha256_update(SHA256_CTX* ctx, const uint8_t* data, size_t len);
void sha256_final(SHA256_CTX* ctx, uint8_t hash[]);
void sha256_transform(SHA256_CTX* ctx, const uint8_t* data);

void sha256_hmac(unsigned char key[], int length, char message[],int dlength, uint8_t* output);
