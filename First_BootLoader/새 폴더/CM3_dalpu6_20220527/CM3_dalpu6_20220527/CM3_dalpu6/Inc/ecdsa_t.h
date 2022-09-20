#include <pkcc_module.h>
#include <ecc.h>

typedef struct  {
	Point curve_point;
	PKCC_MODE pkcc_mode;
	uint32_t* modular_p;
	uint32_t* order_n;
	uint32_t* secret_rand;
	uint32_t* pri_key;
	Point* pub_key;
	uint8_t* message;
	uint32_t msg_len;
	uint32_t* hash;
}PKCC_ECDSA_CTX;


extern void ecc_memory_setting();


void pkcc_ecc_ecdsa_memory_set(PKCC_ECDSA_CTX* ecdsa_ctx);
//void dalpu_ecdsa_sign(PKCC_ECDSA_CTX* ecdsa_ctx, uint8_t* message, Point* sign_rs);
void dalpu_ecdsa_sign(PKCC_ECDSA_CTX* ecdsa_ctx, Point* sign_rs);
uint8_t dalpu_ecdsa_verify(PKCC_ECDSA_CTX* ecdsa_ctx, Point* sign_rs, Point* verify_rs);
//void ecdsa_mem_setting(PKCC_ECDSA_CTX* ecdsa_ctx);
void ecdsa_mem_setting(PKCC_ECDSA_CTX* ecdsa_ctx, uint32_t len);
void ecdsa_mem_destroy(PKCC_ECDSA_CTX* ecdsa_ctx);

