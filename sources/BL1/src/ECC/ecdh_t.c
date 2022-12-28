#include <ecdh_t.h>

void pkcc_ecc_ecdh_memory_set(PKCC_ECC_CTX* ctx){
	static const uint32_t modular_n[8]	= {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	static const uint32_t p256g_x[8]	= {0x6B17D1F2, 0xE12C4247, 0xF8BCE6E5, 0x63A440F2, 0x77037D81, 0x2DEB33A0, 0xF4A13945, 0xD898C296};
	static const uint32_t p256g_y[8]	= {0x4FE342E2, 0xFE1A7F9B, 0x8EE7EB4A, 0x7C0F9E16, 0x2BCE3357, 0x6B315ECE, 0xCBB64068, 0x37BF51F5};

	
	ctx->pkcc_mode = PKCC_P256;
	
	memcpy(ctx->modular_n, modular_n, 32);
	memcpy(ctx->curve_point.x, p256g_x, 32);
	memcpy(ctx->curve_point.y, p256g_y, 32);
	
	//pkcc_memory_setting();
}

void pkcc_ecc_key_pair_gen(PKCC_ECC_CTX* ctx){
	
	pkcc_ECPSM(ctx->pkcc_mode, ctx->modular_n, ctx->pri_key, &ctx->curve_point, ctx->ecdh_pub_key);
	delay_us(50);
	
}

void pkcc_shared_key_gen(PKCC_ECC_CTX* ctx, Point* opponent_pub_key){
	
	pkcc_ECPSM(ctx->pkcc_mode, ctx->modular_n, ctx->pri_key, opponent_pub_key, ctx->shared_key);
	delay_us(50);
}


int ecdh_pkcc_function_call_test(){
	#if 0
	uint32_t alice_pri_key[8] =	{0xff548f30, 0x94a0070b, 0x51a75b87, 0x6a4dc6a0, 0x396797b5, 0x7fe9ddc7, 0xbcd6a5ff, 0xf0f5eeab};
	uint32_t bob_pri_key[8]	=	{0xdc3a837b, 0x7522988c, 0x66bcbd64, 0x86b0a825, 0x6a29e693, 0x82e54c81, 0xe9414829, 0x9a75f08a};
	#else
	//uint32_t alice_pri_key[8] =	{0xE3B6DF8F, 0x3C6066F6, 0x28498D23, 0x2EA233D6, 0xFA2FCA46, 0xBB4DF532, 0x64F6F45C, 0x03B47F5A};
	//uint32_t bob_pri_key[8]	=	{0x82F5C426, 0xAD358BB9, 0xD878A13D, 0x51990BB5, 0x9521DBB1, 0x8DE08E0D, 0xCC637F7C, 0xBC9710E9};
	
	uint32_t alice_pri_key[8] =	{0x03B47F5A, 0x64F6F45C, 0xBB4DF532, 0xFA2FCA46, 0x2EA233D6, 0x28498D23, 0x3C6066F6, 0xE3B6DF8F};
	uint32_t bob_pri_key[8]	=	{0xBC9710E9, 0xCC637F7C, 0x8DE08E0D, 0x9521DBB1, 0x51990BB5, 0xD878A13D, 0xAD358BB9, 0x82F5C426};
	#endif
	uint32_t bob_pub_key_x[8]=	{0x2a0ca7a5, 0x180490d3, 0x265ff25a, 0x21446b97, 0x0e1ce1c2, 0x6006f8cc, 0x3fd6a2af, 0x8083e4b2};
	uint32_t bob_pub_key_y[8]=	{0x07aad6d1, 0x0411b1b2, 0x534e5fc6, 0xf7a39c06, 0xfd7948b7, 0x3b17ecdd, 0x5b9b2bed, 0x5da203ec};
	
	PKCC_ECC_CTX alice;
	PKCC_ECC_CTX bob;
	
	printf("\r\n ecdh_pkcc_function_call_test start");
	
	alice.pri_key = malloc(32);
	alice.ecdh_pub_key = malloc(sizeof(Point));
	alice.shared_key = malloc(sizeof(Point));
	
	 
	
	memcpy(alice.pri_key, alice_pri_key, 32);
	
	bob.pri_key = malloc(32);
	bob.ecdh_pub_key = malloc(sizeof(Point));
	bob.shared_key = malloc(sizeof(Point));
	memcpy(bob.pri_key, bob_pri_key, 32);
	
	pkcc_ecc_ecdh_memory_set(&alice);
	pkcc_ecc_ecdh_memory_set(&bob);
	
	pkcc_ecc_key_pair_gen(&alice);
	pkcc_ecc_key_pair_gen(&bob);
	
	printf("\r\n g.x  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.x[i]); }	
	printf("\r\n g.y  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.y[i]); }	
	printf("\r\n mod.n  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.modular_n[i]); }	
	
	pkcc_shared_key_gen(&alice, bob.ecdh_pub_key);
	pkcc_shared_key_gen(&bob, alice.ecdh_pub_key);
	
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->y[i]); }
	
	printf("\r\n bob.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pri_key[i]); }	
	printf("\r\n bob_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->x[i]); }
	printf("\r\n bob_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->y[i]); }
	printf("\r\n");
	
	printf("\r\n alice_shared_key.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.shared_key->x[i]); }
	printf("\r\n alice_shared_key.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.shared_key->y[i]); }
	printf("\r\n bob_shared_key.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.shared_key->x[i]); }
	printf("\r\n bob_shared_key.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.shared_key->y[i]); }
	
	if(memcmp(alice.shared_key, bob.shared_key, 32)) { 
		printf("\r\n shared_key not matched \r\n ECDH TEST FAIL!!"); 
		free(alice.pri_key);
		free(alice.ecdh_pub_key);
		free(alice.shared_key);
		
		free(bob.pri_key);
		free(bob.ecdh_pub_key);
		free(bob.shared_key);
		return 1;
	} else {
		printf("\r\n shared_key matched \r\n ECDH TEST PASS");
	}

	
	free(alice.pri_key);
	free(alice.ecdh_pub_key);
	free(alice.shared_key);
	
	free(bob.pri_key);
	free(bob.ecdh_pub_key);
	free(bob.shared_key);

	printf("\r\n ecdh_pkcc_function_call_test end");
	
	return 0;
}