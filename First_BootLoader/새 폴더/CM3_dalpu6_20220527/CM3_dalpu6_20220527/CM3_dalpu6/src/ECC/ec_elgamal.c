#include <ec_elgamal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

extern void pkcc_ecc_ecdh_memory_set(PKCC_ECC_CTX* ctx);
extern void pkcc_ecc_key_pair_gen(PKCC_ECC_CTX* ctx);
extern void pkcc_shared_key_gen(PKCC_ECC_CTX* ctx, Point* opponent_pub_key);

static uint32_t PKCC_MSR_A_PRAM[8] = { 0x27D2604B, 0x3BCE3C3E, 0xCC53B0F6, 0x651D06B0, 0x769886BC, 0xB3EBBD55, 0xAA3A93E7, 0x5AC635D8 };
static uint32_t PKCC_MSR_B_PRAM[8] = { 0xFFFFFFFC, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000001,  0xFFFFFFFF };

void ec_elgamal_test(){
	uint32_t bob_msg[] = { 0x54657374 }; //String Test 
	//uint32_t bob_msg[] = { 0x77777774 }; //String Test 
	uint32_t alice_pri_key[8] =	{0xff548f30, 0x94a0070b, 0x51a75b87, 0x6a4dc6a0, 0x396797b5, 0x7fe9ddc7, 0xbcd6a5ff, 0xf0f5eeab};
	PKCC_ECC_CTX alice;
	PKCC_ECC_CTX bob;
	
	printf("\r\n\n ec_elgamal_test start");
	
	alice.pri_key = malloc(32);
	alice.ecdh_pub_key = malloc(sizeof(Point));
	alice.shared_key = malloc(sizeof(Point));
	memcpy(alice.pri_key, alice_pri_key, 32);
	
	/////////////////////////////////////alice public key gen////////////////////////////////////////////
	pkcc_ecc_ecdh_memory_set(&alice);
	pkcc_ecc_key_pair_gen(&alice);
	
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->y[i]); }
	
	/////////////////////////////////////bob msg(M) gen////////////////////////////////////////////
	bob.pri_key = malloc(32);
	bob.ecdh_pub_key = malloc(sizeof(Point));
	bob.shared_key = malloc(sizeof(Point));
	pkcc_ecc_ecdh_memory_set(&bob);
	
	int result = 1;
	Point msg_m;
	uint32_t rnd[8], rnd_temp[8];
	
	while(result){
		for( int i=0; i<8; i++)	rnd[i] = rand() % 0xFFFFFFFF;
		printf("\r\n bob MA befor M:"); for(int i = 0; i < 8; i++){ printf("%08x ", rnd[i]);}
    
//		rnd[0] ^= bob_msg[0]; 
//		printf("\r\n bob MSR befor M:"); for(int i = 0; i < 8; i++){ printf("%08x ", rnd[i]);}
		#if 0
		result = pkcc_MA(bob.pkcc_mode, bob.modular_n, rnd, bob_msg, rnd_temp);
		#else
		memcpy(rnd_temp, rnd, 32);
		rnd_temp[0] ^= bob_msg[0]; 
		#endif
		printf("\r\n bob MA after M:"); for(int i = 0; i < 8; i++){ printf("%08x ", rnd_temp[i]);}
		
	//	result = pkcc_MSR(bob.pkcc_mode, bob.modular_n, PKCC_MSR_A_PRAM, PKCC_MSR_B_PRAM, rnd, &msg_m);
		result = pkcc_MSR(bob.pkcc_mode, bob.modular_n, PKCC_MSR_A_PRAM, PKCC_MSR_B_PRAM, rnd_temp, &msg_m);
		printf("\r\n ret = %d", result);
	}
	printf("\r\n %s", bob_msg);
	
		
	//printf("\r\n bob M:"); for(int i = 0; i < 8; i++){ printf("%08x ", rnd[i]); }
	
	printf("\r\nmapping_msg pt.x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", msg_m.x[i]);}
    printf("\r\nmapping_msg pt.y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", msg_m.y[i]);}
    printf("\r\n");

	/////////////////////////////////////bob random(k) pic////////////////////////////////////////////
	
	memcpy(bob.pri_key, rnd, 32);
	pkcc_ecc_key_pair_gen(&bob);
	
	printf("\r\n bob.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pri_key[i]); }	
	printf("\r\n bob_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->x[i]); }
	printf("\r\n bob_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->y[i]); }
	
	Point ct;
	
	pkcc_shared_key_gen(&bob, alice.ecdh_pub_key); 
	pkcc_ECPA(bob.pkcc_mode, bob.modular_n, &msg_m, bob.shared_key, &ct);
	
	printf("\r\nct x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", ct.x[i]); }
    printf("\r\nct y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", ct.y[i]); }
    printf("\r\n");


	Point dt;
	pkcc_shared_key_gen(&alice, bob.ecdh_pub_key); 
	
	pkcc_ECPS(alice.pkcc_mode, alice.modular_n, &ct, alice.shared_key, &dt);
	
	printf("\r\ndt x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.x[i]); }
    printf("\r\ndt y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.y[i]); }
    printf("\r\n");
	
	for( int i=0; i<32; i++) dt.x[i] ^= rnd[i];
	printf("\r\ndt x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.x[i]); }
    printf("\r\ndt y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.y[i]); }
    printf("\r\n");
	
	free(alice.pri_key);
	free(alice.ecdh_pub_key);
	free(alice.shared_key);
	
	free(bob.pri_key);
	free(bob.ecdh_pub_key);
	free(bob.shared_key);
	
	printf("\r\n\n ec_elgamal_test end");
}

void ec_elgamal_function_call_test(){
	
	//uint8_t bob_msg[4] = { 0x54, 0x65, 0x73, 0x74 }; //String Test 
	//uint32_t bob_msg[] = { 0x54657374 }; //String Test 
	uint32_t alice_pri_key[8] =	{0xff548f30, 0x94a0070b, 0x51a75b87, 0x6a4dc6a0, 0x396797b5, 0x7fe9ddc7, 0xbcd6a5ff, 0xf0f5eeab};
	uint32_t bob_pri_key[8]	=	{0xdc3a837b, 0x7522988c, 0x66bcbd64, 0x86b0a825, 0x6a29e693, 0x82e54c81, 0xe9414829, 0x9a75f08a};

	PKCC_ECC_CTX alice;
	PKCC_ECC_CTX bob;
	
	printf("\r\n\n ec_elgamal_function_call_test start");
	
	alice.pri_key = malloc(32);
	alice.ecdh_pub_key = malloc(sizeof(Point));
	alice.shared_key = malloc(sizeof(Point));
	memcpy(alice.pri_key, alice_pri_key, 32);
	
	pkcc_ecc_ecdh_memory_set(&alice);
	pkcc_ecc_key_pair_gen(&alice);
	
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->y[i]); }
	
	
	int result = 1;
	Point mapping_msg;
	uint32_t msg[8];
	
	
	while(result != 0){
		printf("\r\n value : ");
		for(int i=0; i<8; i++) {
			msg[i] = (rand() % 0xFFFF);
			printf("0x%08x ", msg[i]);
			delay_us(10);
		}
		result = pkcc_MSR(alice.pkcc_mode, alice.modular_n, PKCC_MSR_B_PRAM, PKCC_MSR_A_PRAM, msg, &mapping_msg);
		
		if(result == 0)	break;
		else printf("\r\nvalue not in curve\r\n");
	}
	
	//printf("\r\n %s", bob_msg);
	
		
	printf("\r\n bob msg:"); for(int i = 0; i < 8; i++){
        printf("%08x ", msg[i]);
    }
	
	printf("\r\nmapping_msg pt.x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", mapping_msg.x[i]);}
    printf("\r\nmapping_msg pt.y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", mapping_msg.y[i]);}
    printf("\r\n");


	bob.pri_key = malloc(32);
	bob.ecdh_pub_key = malloc(sizeof(Point));
	bob.shared_key = malloc(sizeof(Point));
	memcpy(bob.pri_key, bob_pri_key, 32);
	
	pkcc_ecc_ecdh_memory_set(&bob);
	pkcc_ecc_key_pair_gen(&bob);
	
	printf("\r\n bob.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pri_key[i]); }	
	printf("\r\n bob_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->x[i]); }
	printf("\r\n bob_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->y[i]); }
	
	uint8_t len = 0;
	Point ct;
	
	//len = pkcc_ECPSM(bob.pkcc_mode, bob.modular_n, bob.pri_key, &bob.curve_point, bob.ecdh_pub_key); //bob's. pub_key gen
	//len = pkcc_ECPSM(bob.pkcc_mode, bob.modular_n, bob.pri_key, &alice.ecdh_pub_key, &ct); // bob's shared_key gen
//	pkcc_ecc_key_pair_gen(&bob);
	pkcc_shared_key_gen(&bob, alice.ecdh_pub_key);
	
	pkcc_ECPA(bob.pkcc_mode, bob.modular_n, &mapping_msg, bob.shared_key, &ct);
	

	
	printf("\r\nct x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", ct.x[i]); }
    printf("\r\nct y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", ct.y[i]); }
    printf("\r\n");
	
	
	pkcc_shared_key_gen(&alice, bob.ecdh_pub_key); 
	
	Point dt;
	
	pkcc_ECPS(alice.pkcc_mode, alice.modular_n, &ct, alice.shared_key, &dt);
	
	printf("\r\npt x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", mapping_msg.x[i]);}
    printf("\r\npt y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", mapping_msg.y[i]);}
    printf("\r\n");
	
	printf("\r\ndt x :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.x[i]); }
    printf("\r\ndt y :\t"); for(int i = 0; i < 8; i++){ printf("%08x ", dt.y[i]); }
    printf("\r\n");
	
	free(alice.pri_key);
	free(alice.ecdh_pub_key);
	free(alice.shared_key);
	
	free(bob.pri_key);
	free(bob.ecdh_pub_key);
	free(bob.shared_key);
	
	
	printf("\r\n ec_elgamal_function_call_test end");
}