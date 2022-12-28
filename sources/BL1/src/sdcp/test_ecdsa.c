
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecdsa.h"

#include "mbedtls/config.h"


#include <ecdh_t.h>


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sdcpclient.h"

void ecp_clear_precomputed( mbedtls_ecp_group *grp )
{
    if( grp->T != NULL )
    {
        size_t i;
        for( i = 0; i < grp->T_size; i++ )
            mbedtls_ecp_point_free( &grp->T[i] );
        free( grp->T );
    }
    grp->T = NULL;
    grp->T_size = 0;
}

static int myrand( void *rng_state, unsigned char *output, size_t len )
{
    size_t use_len;
    int rnd;
	
	uint32_t secretkey[32] = { 0x5d, 0xdf, 0x4f, 0x7f, 0x3d, 0x1b, 0x1b, 0xc8, 0x6a, 0x62, 0x53, 0x45, 0x58, 0xe6, 0xad, 0x07, 
							   0x7d, 0xa0, 0xd8, 0x68, 0x5f, 0x94, 0x7b, 0x49, 0x71, 0x80, 0x21, 0xcb, 0x56, 0x47, 0xa0, 0xb0 };

    if( rng_state != NULL )
        rng_state  = NULL;

	printf("\r\n myrand len : %d", len);
	
    while( len > 0 )
    {
        use_len = len;
        if( use_len > sizeof(int) )
            use_len = sizeof(int);

        rnd = rand();
		printf("\r\n rnd = %08x", rnd);
		//rnd = secretkey[];
        memcpy( output, &rnd, use_len );
        output += use_len;
        len -= use_len;
    }
	
	//print_bytes("myrand output", output, 32);
	printf("\r\n myrand output:");
	for( int j=0; j<32; j++) printf(" 0x%02x", output[j]);

    return( 0 );
}

unsigned char buf[256];

int ecdsa_test_tt(){
	mbedtls_ecdsa_context ecdsa;
	const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_info_from_grp_id(MBEDTLS_ECP_DP_SECP256R1);
	size_t sig_len;
	unsigned char tmp[256];
	
	uint32_t secretkey[8] = { 0x5ddf4f7f, 0x3d1b1bc8, 0x6a625345, 0x58e6ad07, 0x7da0d868, 0x5f947b49, 0x718021cb, 0x5647a0b0 };
	uint32_t publickey[8] = { 0xe6c371f9, 0xf0f3f2e7, 0xc12cb36a, 0xf08b446d, 0x57a6e0b6, 0xdecbd178, 0x4dd454b3, 0xd2dee243 };
	
	mbedtls_ecp_keypair *key;

	memset(buf, 0x00, 32);
	strcpy((char *)buf, "hello world"); //just fill with something other than 0x2A

	mbedtls_ecdsa_init(&ecdsa);
	
	mbedtls_mpi_init( &key->d );
    mbedtls_ecp_point_init( &key->Q );
	
//	print_bytes("ecdsa secret key", ecdsa.d.p, 32);
//	print_bytes("ecdsa public key", ecdsa.Q.X.p, 32);
//	
//	printf("\r\n hash size00 : %d", curve_info->bit_size);
//	printf("\r\n sig size00 : %d", sig_len);

	int ret_genkey = mbedtls_ecdsa_genkey(&ecdsa, curve_info->grp_id, myrand, NULL);

	print_bytes("ecdsa secret key", ecdsa.d.p, 32);
	print_bytes("ecdsa public key", ecdsa.Q.X.p, 32);
	
	printf("\r\n hash size01 : %d", curve_info->bit_size);
	printf("\r\n sig size01 : %d", sig_len);
	
	#if 1
	key->d.p = malloc(32);
	key->Q.X.p = malloc(32);
	memcpy(key->d.p, secretkey, 32);
	memcpy(key->Q.X.p, publickey, 32);
	
//	memcpy(ecdsa.d.p, key->d.p, 32);
//	memcpy(ecdsa.Q.X.p, key->Q.X.p, 32);
	
	for(int i=0, j=0; i<8; i++){
		ecdsa.d.p[i] =   (((key->d.p[j])&0xFF) << 24)
						|(((key->d.p[j]>>8)&0xFF) << 16)
						|(((key->d.p[j]>>16)&0xFF) << 8)
						|(((key->d.p[j]>>24)&0xFF) << 0);
		j++;
	}
	
	print_bytes("ecdsa secret key01", ecdsa.d.p, 32);
	print_bytes("ecdsa public key01", ecdsa.Q.X.p, 32);
	
	#endif
	
	int ret_write_sign = mbedtls_ecdsa_write_signature(&ecdsa, MBEDTLS_MD_SHA256, buf, curve_info->bit_size, tmp, &sig_len, myrand, NULL);

	printf("\r\n hash size02 : %d", curve_info->bit_size);
	printf("\r\n sig size02 : %d", sig_len);
	
	print_bytes(" buf", buf, 32);
	
	print_bytes(" tmp", tmp, 256);

//	printf("\r\nret_genkey = %d\n", ret_genkey);
	printf("\r\nret_write_sign = %d\n", ret_write_sign);
	
	ecp_clear_precomputed(&ecdsa.grp);

	int ret_verify = mbedtls_ecdsa_read_signature(&ecdsa, buf, curve_info->bit_size, tmp, sig_len);
	printf("\r\nret_verify = %d\n", ret_verify);
		
	return 0;
}
