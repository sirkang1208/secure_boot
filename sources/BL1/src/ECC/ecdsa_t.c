#include <ecdsa_t.h>
#include <sha256_t.h>


void pkcc_ecc_ecdsa_memory_set(PKCC_ECDSA_CTX* ecdsa_ctx){
	static const uint32_t ecdsa_moular_p[8]	= {0xFFFFFFFF, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	static const uint32_t ecdsa_modular_n[8]	= {0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xBCE6FAAD, 0xA7179E84, 0xF3B9CAC2, 0xFC632551};
	static const uint32_t ecdsa_p256_x[8]		= {0x6B17D1F2, 0xE12C4247, 0xF8BCE6E5, 0x63A440F2, 0x77037D81, 0x2DEB33A0, 0xF4A13945, 0xD898C296};
	static const uint32_t ecdsa_p256_y[8]		= {0x4FE342E2, 0xFE1A7F9B, 0x8EE7EB4A, 0x7C0F9E16, 0x2BCE3357, 0x6B315ECE, 0xCBB64068, 0x37BF51F5};

	ecdsa_ctx->pkcc_mode = PKCC_P256;
	
	memcpy(ecdsa_ctx->modular_p, ecdsa_moular_p, 32);
	memcpy(ecdsa_ctx->order_n, ecdsa_modular_n, 32);
	memcpy(ecdsa_ctx->curve_point.x, ecdsa_p256_x, 32);
	memcpy(ecdsa_ctx->curve_point.y, ecdsa_p256_y, 32);
	
	//pkcc_memory_setting();
}

//void dalpu_ecdsa_sign(PKCC_ECDSA_CTX* ecdsa_ctx, uint8_t* message, Point* sign_rs){
void dalpu_ecdsa_sign(PKCC_ECDSA_CTX* ecdsa_ctx, Point* sign_rs){
	Point x1;
	uint32_t zero_init[8] = {0,};
	uint8_t buf[32];
	uint32_t dr[8]={0,};
	uint32_t k_inv[8] = { 0,};
	uint32_t hdr[8] = { 0,};	
	SHA256_CTX sha_ctx; 
	
	//1.Q(x,y) = d *G mode p(scalar multiplication)
	pkcc_ECPSM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->modular_p, ecdsa_ctx->pri_key, &ecdsa_ctx->curve_point, ecdsa_ctx->pub_key);
	//2.(x1, y1) = k * G(x,y) (scalar multiplication)
	pkcc_ECPSM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->modular_p, ecdsa_ctx->secret_rand, &ecdsa_ctx->curve_point, &x1);
	
	//3.r = x1 + 0 mod n (modular addition)
	memset(zero_init, 0, 32);
	pkcc_MA(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, zero_init, x1.x, sign_rs->x);
	
	//make sha hash data
	//sha operation..........
	sha256_init(&sha_ctx);  
	sha256_update(&sha_ctx, ecdsa_ctx->message, ecdsa_ctx->msg_len);
	sha256_final(&sha_ctx, buf);
	//.................sha end
	//h(m)
	for( int i=0, j=0; i<8; i++) {
		ecdsa_ctx->hash[i] = ((((buf[j+0]) & 0xff)<<24)
							|(((buf[j+1]) & 0xff)<<16)
							|(((buf[j+2]) & 0xff)<<8)
							|((buf[j+3]) & 0xff) );		
		j+=4;
	}
	
	//Create signature data
	//d*r mod n
	pkcc_MM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, sign_rs->x, ecdsa_ctx->pri_key, dr);

	//h(m)+dr
	pkcc_MA(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, ecdsa_ctx->hash, dr, hdr);
		
	//k^-1 mod n (k: secret_rand)
	pkcc_MI(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, ecdsa_ctx->secret_rand, k_inv);
	pkcc_MM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, hdr, k_inv, sign_rs->y);

}

uint8_t dalpu_ecdsa_verify(PKCC_ECDSA_CTX* ecdsa_ctx, Point* sign_rs, Point* verify_rs){
	uint8_t ret = 0xff;
	Point verify1, verify2;
	Point verify_data;
	
	uint32_t w[8];
	uint32_t u1[8];
	uint32_t u2[8];
	
	//w = s^-1 mod n(modular inversion)
	pkcc_MI(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, sign_rs->y, w);
	
	//u1 = (h(m)*w) mod n (modular multiplication)
	pkcc_MM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, ecdsa_ctx->hash, w, u1);
	
	//u2 = (r*w) mod n (modular multiplication)
	pkcc_MM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->order_n, sign_rs->x, w, u2);
	
	//(x2, y2) = (u1 * G(x,y) + u2 * Q(x,y)) mod p 
	pkcc_ECPSM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->modular_p, u1, &ecdsa_ctx->curve_point, &verify1);
	pkcc_ECPSM(ecdsa_ctx->pkcc_mode, ecdsa_ctx->modular_p, u2, ecdsa_ctx->pub_key, &verify2);
	pkcc_ECPA(ecdsa_ctx->pkcc_mode, ecdsa_ctx->modular_p, &verify1, &verify2, &verify_data);

	memcpy(verify_rs->x, verify_data.x, 32);
	memcpy(verify_rs->y, verify_data.y, 32);
	
	if(memcmp(sign_rs->x, verify_data.x, 32)) { 
		//printf("\r\n PKCC ECDSA Verify test FAIL"); 
		ret = 1;
	}else { 
		//printf("\r\n PKCC ECDSA Verify test Pass!!"); 
		ret = 0;
	}

	return ret;
}

void ecdsa_mem_setting(PKCC_ECDSA_CTX* ecdsa_ctx, uint32_t len){
	ecdsa_ctx->modular_p = malloc(32);
	ecdsa_ctx->order_n = malloc(32);
	ecdsa_ctx->secret_rand = malloc(32);
	ecdsa_ctx->pri_key = malloc(32);
	ecdsa_ctx->pub_key = malloc(sizeof(Point));
	ecdsa_ctx->message = malloc(len);
	ecdsa_ctx->msg_len = len;
	ecdsa_ctx->hash = malloc(32);
	
	pkcc_ecc_ecdsa_memory_set(ecdsa_ctx);
}

void ecdsa_mem_destroy(PKCC_ECDSA_CTX* ecdsa_ctx){
	free(ecdsa_ctx->modular_p);
	free(ecdsa_ctx->order_n);
	free(ecdsa_ctx->secret_rand);
	free(ecdsa_ctx->pri_key);
	free(ecdsa_ctx->pub_key);
	free(ecdsa_ctx->message);
	free(ecdsa_ctx->hash);
	free(ecdsa_ctx);
}

uint32_t r_data[8] ;//= { 0x49a6f01e, 0x99da021d, 0x33a366c2, 0x5dc8f4d2, 0xcc219025, 0x3269bdb1, 0x4c8cc8ec, 0xa0689ae3 };
uint32_t s_data[8] ;//= { 0xf813e5da, 0x8398eeee, 0x8833f82a, 0x0c652e1e, 0x0ffac776, 0x8b953014, 0x54d8bc8e, 0x605f662a };
uint32_t h_data[8] ;//= 
Point opponent_public_test_key;

void ecdsa_verify_function_test(){
	Point sign_rs; 
	Point verify_rs; 
	PKCC_ECDSA_CTX bob;
	uint8_t ret = 0xff;
	
	printf("\r\n\n ecdsa_verify_function_test start");
	
	ecdsa_mem_setting(&bob, 0);
	
	memcpy(sign_rs.x, r_data, 32);
	memcpy(sign_rs.y, s_data, 32);
	memcpy(bob.hash, h_data, 32);
	memcpy(bob.pub_key->x, opponent_public_test_key.x, 32);
	memcpy(bob.pub_key->y, opponent_public_test_key.y, 32);
	
//	printf("\r\n r  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", sign_rs.x[i]); }	
//	printf("\r\n s  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", sign_rs.y[i]); }	
//	printf("\r\n h  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.hash[i]); }	
//	printf("\r\n pub_key.x  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pub_key->x[i]); }	
//	printf("\r\n pub_key.y  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pub_key->y[i]); }	
//	
//	printf("\r\n");
	
	ret = dalpu_ecdsa_verify(&bob, &sign_rs, &verify_rs);
	
	printf("\r\n r\t\t:\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", sign_rs.x[i]); }	
	printf("\r\n verify_rs.x\t:\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", verify_rs.x[i]); }	
	printf("\r\n");
	
	if(ret == 0){		
		printf("\r\n ecdsa verify test PASS ret = %x\n", ret);
	}else{
		printf("\r\n ecdsa verify test FAIL ret = %x\n", ret);
	}
		
	ecdsa_mem_destroy(&bob);
	printf("\r\n ecdsa_verify_function_test end\n");
}

void ecdsa_sign_function_test(){
	uint32_t d[8] 	= { 0xab9ca4f8, 0x5b6d039a, 0xc6f9698d, 0x135ae163, 0x76310be1, 0x8436ef3e, 0x3105f4d7, 0x16a69420 };
	uint32_t k[8] 	= { 0x9c41ae96, 0x9ac6f8a1, 0xd20b26cc, 0xc9c28243, 0xdc773b3f, 0x46caa105, 0x6c404e6a, 0x3d8e0e7b };
	uint8_t buf[32];
	
	uint8_t message[8] = { 0x61, 0x73, 0x64, 0x66, 0x61, 0x73, 0x64, 0x66 }; //asdfasdf
	uint32_t msg_len = 8;
	
	SHA256_CTX sha_ctx;  			
	PKCC_ECDSA_CTX alice;
	Point sign_rs; // sign_rs.x == r, sign_rs.y == s
	
	printf("\r\n\n ecdsa_sign_function_test start");

	ecdsa_mem_setting(&alice, msg_len);
	
	memcpy(alice.pri_key, d, 32);
	memcpy(alice.secret_rand, k, 32);
	memcpy(alice.message, message, msg_len);
	
	printf("\r\n alice.modular_p  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.modular_p[i]); }	
	printf("\r\n alice.order_n  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.order_n[i]); }	
	printf("\r\n alice.curve_point.x  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.x[i]); }	
	printf("\r\n alice.curve_point.y  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.y[i]); }	
	printf("\r\n");
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice.secret_rand  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.secret_rand[i]); }	
	printf("\r\n");
	
	//dalpu_ecdsa_sign(&alice, message, &sign_rs);
	dalpu_ecdsa_sign(&alice, &sign_rs);
	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pub_key->y[i]); }
	printf("\r\n");
	
	printf("\r\n r\t:\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", sign_rs.x[i]); }	
	printf("\r\n s\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", sign_rs.y[i]); }	
	printf("\r\n");
	
	
	memcpy(r_data, sign_rs.x, 32);
	memcpy(s_data, sign_rs.y, 32);
	memcpy(h_data, alice.hash, 32);
	memcpy(opponent_public_test_key.x, alice.pub_key->x, 32);
	memcpy(opponent_public_test_key.y, alice.pub_key->y, 32);
	
	ecdsa_mem_destroy(&alice);
	
	printf("\r\n ecdsa_sign_function_test end\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ecdsa_test(){	
	uint32_t d[8] 	= { 0xab9ca4f8, 0x5b6d039a, 0xc6f9698d, 0x135ae163, 0x76310be1, 0x8436ef3e, 0x3105f4d7, 0x16a69420 };
	uint32_t k[8] 	= { 0x9c41ae96, 0x9ac6f8a1, 0xd20b26cc, 0xc9c28243, 0xdc773b3f, 0x46caa105, 0x6c404e6a, 0x3d8e0e7b };
	uint8_t message[8] = { 0x61, 0x73, 0x64, 0x66, 0x61, 0x73, 0x64, 0x66 }; //asdfasdf
	uint8_t buf[32];
	SHA256_CTX ctx;  			
	PKCC_ECDSA_CTX alice;
	
	printf("\r\n ecdsa_test start");
	
	alice.modular_p = malloc(32);
	alice.order_n = malloc(32);
	alice.secret_rand = malloc(32);
	alice.pri_key = malloc(32);
	alice.pub_key = malloc(sizeof(Point));
	alice.hash = malloc(32);
	
	memcpy(alice.pri_key, d, 32);
	memcpy(alice.secret_rand, k, 32);
	
	pkcc_ecc_ecdsa_memory_set(&alice);


#if 1	// ecdsa_p256_sign function	
	//Q(x,y) = d *G mode p(scalar multiplication)
	pkcc_ECPSM(alice.pkcc_mode, alice.modular_p, alice.pri_key, &alice.curve_point, alice.pub_key);
	delay_us(100);
	
	printf("\r\n alice.modular_p  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.modular_p[i]); }	
	printf("\r\n alice.order_n  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.order_n[i]); }	
	printf("\r\n alice.curve_point.x  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.x[i]); }	
	printf("\r\n alice.curve_point.y  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.y[i]); }	
	printf("\r\n");
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pub_key->y[i]); }
	printf("\r\n");
	
	
	Point x1;
	uint32_t zero_init[8] = {0,};
	uint32_t r[8] = {0,};
	//(x1, y1) = k * G(x,y) (scalar multiplication)
	pkcc_ECPSM(alice.pkcc_mode, alice.modular_p, alice.secret_rand, &alice.curve_point, &x1);
	delay_us(100);
	
	printf("\r\n alice.secret_rand  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.secret_rand[i]); }	
	printf("\r\n x1.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", x1.x[i]); }
	printf("\r\n x1.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", x1.y[i]); }
	printf("\r\n");
	
	//r = x1 + 0 mod n (modular addition)
	memset(zero_init, 0, 32);
	pkcc_MA(alice.pkcc_mode, alice.order_n, zero_init, x1.x, r);
	
	printf("\r\n alice.order_n\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.order_n[i]); }	
	printf("\r\n zero_init\t:\t");		for(int i = 0; i < 8; i++){ printf("0x%08x ", zero_init[i]); }	
	printf("\r\n r\t\t:\t");			for(int i = 0; i < 8; i++){ printf("0x%08x ", r[i]); }	
	printf("\r\n");
	
	//make sha hash data
	//sha operation..........
	sha256_init(&ctx);  
	sha256_update(&ctx, message, 8);
	sha256_final(&ctx, buf);
	printf("\r\n sha data\t:\t");	for(int i = 0; i < 32; i++){ printf("0x%02x ", buf[i]); }	
	
	//.................sha end
	
	//h(m)
	for( int i=0, j=0; i<8; i++) {
		alice.hash[i] = ((((buf[j+0]) & 0xff)<<24)
						|(((buf[j+1]) & 0xff)<<16)
						|(((buf[j+2]) & 0xff)<<8)
						|((buf[j+3]) & 0xff) );		
		j+=4;
	}
	printf("\r\n alice.hash\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.hash[i]); }	
	printf("\r\n");
	
	//Create signature data
	uint32_t dr[8]={0,};

	//d*r mod n
	pkcc_MM(alice.pkcc_mode, alice.order_n, r, alice.pri_key, dr);
	printf("\r\n dr\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", dr[i]); }	
	printf("\r\n");
	
	
	//h(m)+dr
	uint32_t hdr[8] = { 0,};
	
	pkcc_MA(alice.pkcc_mode, alice.order_n, alice.hash, dr, hdr);
	printf("\r\n hdr\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", hdr[i]); }	
	printf("\r\n");
	
	//k^-1 mod n
	uint32_t k_inv[8] = { 0,};
	pkcc_MI(alice.pkcc_mode, alice.order_n, k, k_inv);
	printf("\r\n k\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", k[i]); }	
	printf("\r\n k_inv\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", k_inv[i]); }	
	printf("\r\n");
	
	uint32_t s[8] = { 0, };
	pkcc_MM(alice.pkcc_mode, alice.order_n, hdr, k_inv, s);
		
	printf("\r\n r\t:\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", r[i]); }	
	printf("\r\n s\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", s[i]); }	
	printf("\r\n");
	printf("\r\n");
		
#endif		// eof ecdsa_p256_sign function 
	
#if 1 // ecdsa_p256_verify
	PKCC_ECDSA_CTX bob;
	
	bob.modular_p = malloc(32);
	bob.order_n = malloc(32);
	bob.secret_rand = malloc(32);
	bob.pri_key = malloc(32);
	bob.pub_key = malloc(sizeof(Point));
	bob.hash = malloc(32);
	pkcc_ecc_ecdsa_memory_set(&bob);
	
	//w = s^-1 mod n(modular inversion)
	uint32_t w[8];
	pkcc_MI(bob.pkcc_mode, bob.order_n, s, w);
	printf("\r\n s\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", s[i]); }	
	printf("\r\n w\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", w[i]); }	
	printf("\r\n");
	
	//make sha hash data
	//sha operation..........
//	sha256_init(&ctx);  
//	sha256_update(&ctx, message, 8);
//	sha256_final(&ctx, buf);
//	printf("\r\n sha data\t:\t");	for(int i = 0; i < 32; i++){ printf("0x%02x ", buf[i]); }	
//	//.................sha end
	//h(m)
	for( int i=0, j=0; i<8; i++) {
		bob.hash[i] = ((((buf[j+0]) & 0xff)<<24)
						|(((buf[j+1]) & 0xff)<<16)
						|(((buf[j+2]) & 0xff)<<8)
						|((buf[j+3]) & 0xff) );		
		j+=4;
	}
	printf("\r\n bob.hash\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.hash[i]); }	
	printf("\r\n");
	
	
	//u1 = (h(m)*w) mod n (modular multiplication)
	uint32_t u1[8];
	pkcc_MM(bob.pkcc_mode, bob.order_n, bob.hash, w, u1);
	printf("\r\n u1\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", u1[i]); }	
	printf("\r\n");
	
	//u2 = (r*w) mod n (modular multiplication)
	uint32_t u2[8];
	pkcc_MM(bob.pkcc_mode, bob.order_n, r, w, u2);
	printf("\r\n u2\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", u2[i]); }	
	printf("\r\n");
	
	//(x2, y2) = (u1 * G(x,y) + u2 * Q(x,y)) mod p 
	Point verify1, verify2;
	
	pkcc_ECPSM(bob.pkcc_mode, bob.modular_p, u1, &bob.curve_point, &verify1);
	delay_us(100);
	pkcc_ECPSM(bob.pkcc_mode, bob.modular_p, u2, alice.pub_key, &verify2);
	delay_us(100);
	printf("\r\n verify1.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify1.x[i]); }	
	printf("\r\n verify1.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify1.y[i]); }	
	printf("\r\n verify2.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify2.x[i]); }	
	printf("\r\n verify2.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify2.y[i]); }	
	printf("\r\n");
	
	Point verify_data;
	
	printf("\r\n bob.pkcc_mode\t:\t%d", bob.pkcc_mode);	
	printf("\r\n bob.modular_p\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.modular_p[i]); }	
	pkcc_ECPA(bob.pkcc_mode, bob.modular_p, &verify1, &verify2, &verify_data);
	//pkcc_ECPA(bob.pkcc_mode, bob.modular_p, &verify2, &verify1, &verify_data);
	delay_us(100);
	printf("\r\n verify_data.x\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify_data.x[i]); }	
	printf("\r\n verify_data.y\t:\t");	for(int i = 0; i < 8; i++){ printf("0x%08x ", verify_data.y[i]); }	
	printf("\r\n");
	
	if(memcmp(r, verify_data.x, 32)) { printf("\r\n PKCC ECDSA Verify test FAIL"); }
	else { printf("\r\n PKCC ECDSA Verify test Pass!!"); }

	
#endif	//eof ecdsa_p256_verify
	
	free(alice.modular_p);
	free(alice.order_n);
	free(alice.secret_rand);
	free(alice.pri_key);
	free(alice.pub_key);
	free(alice.hash);

	
	free(bob.modular_p);
	free(bob.order_n);
	free(bob.secret_rand);
	free(bob.pri_key);
	free(bob.pub_key);
	free(bob.hash);
	
	printf("\r\n ecdsa_test end");
}


