#include <pkcc_module.h>
#include <ecc.h>

#if 0
#define P256

typedef enum{
	PKCC_P192 = 0,
	PKCC_P224,
	PKCC_P256,
	PKCC_P384,
	PKCC_P512
}PKCC_MODE;

typedef struct  {
	uint32_t modular_n[8];
	Point curve_point;
	PKCC_MODE pkcc_mode;
    uint32_t* pri_key;
    Point* ecdh_pub_key;
	Point* shared_key;
}PKCC_ECC_CTX;

#ifdef P256
const uint32_t MODULAR_N[8]	= {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF};
const uint32_t P256G_X[8]	= {0xD898C296, 0xF4A13945, 0x2DEB33A0, 0x77037D81, 0x63A440F2, 0xF8BCE6E5, 0xE12C4247, 0x6B17D1F2};
const uint32_t P256G_Y[8]	= {0x37BF51F5, 0xCBB64068, 0x6B315ECE, 0x2BCE3357, 0x7C0F9E16, 0x8EE7EB4A, 0xFE1A7F9B, 0x4FE342E2};
#endif
#endif
