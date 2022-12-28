#ifndef _ECC_H_
#define _ECC_H_

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



#endif //eof #ifndef _ECC_H_
