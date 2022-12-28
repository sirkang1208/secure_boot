
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
#define MBEDTLS_CHK(call) if((err = (call)) != 0) { goto exit; }

//#if defined(MBEDTLS_PLATFORM_C)
//#include "mbedtls/platform.h"
//#else
//#include <stdio.h>
//#include <stdlib.h>
#define mbedtls_printf          printf
#define mbedtls_exit            exit
//#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
//#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
//#endif /* MBEDTLS_PLATFORM_C */

int mbedtls_ecdh_test(){
	int ret = 1;
    int exit_code = 1;
    mbedtls_ecdh_context ctx_cli, ctx_srv;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    unsigned char cli_to_srv[32], srv_to_cli[32], y[32],z[32];
    const char pers[] = "ecdh";
//    ((void) argc);
//    ((void) argv);

    mbedtls_ecdh_init( &ctx_cli );
    mbedtls_ecdh_init( &ctx_srv );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    /*
     * Initialize random number generation
     */
    mbedtls_printf( "\r\n  0. Seeding the random number generator..." );
//    fflush( stdout );

	#if 0
    mbedtls_entropy_init( &entropy );
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               sizeof pers ) ) != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );
	#endif
#if 0 
	/*
     * Client: initialize context and generate keypair
     */
    mbedtls_printf( "\r\n  1. Setting up client context..." );
//    fflush( stdout );

    ret = mbedtls_ecp_group_load( &ctx_cli.grp, MBEDTLS_ECP_DP_SECP256R1 );  
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecp_group_load returned %d\n", ret );
        goto exit;
    }
	printf("\r\n --------------------------------------------------------------------------------------------");
	print_bytes("ctx_cli.d", &ctx_cli.d, 32);
	
    ret = mbedtls_ecdh_gen_public( &ctx_cli.grp, &ctx_cli.d, &ctx_cli.Q,
                                   mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_gen_public returned %d\n", ret );
        goto exit;
    }
	printf("\r\n -----------------------------11111111---------------------------------------------------------------");
	mbedtls_mpi_write_binary( &ctx_cli.d, cli_to_srv, 32 );
	print_bytes("client prikey", &ctx_cli.d, 32);
    ret = mbedtls_mpi_write_binary( &ctx_cli.Q.X, cli_to_srv, 32 );
	
	
	print_bytes(" client pubkey :", cli_to_srv, 32);
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_write_binary returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );
#endif
    /*
     * Server: initialize context and generate keypair
     */
    mbedtls_printf( " \r\n 2. Setting up server context..." );
 //   fflush( stdout );

    ret = mbedtls_ecp_group_load( &ctx_srv.grp, MBEDTLS_ECP_DP_SECP256R1 );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecp_group_load returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_ecdh_gen_public( &ctx_srv.grp, &ctx_srv.d, &ctx_srv.Q,
                                   mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_gen_public returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_mpi_write_binary( &ctx_srv.Q.X, srv_to_cli, 32 );
	print_bytes(" server pubkey :", srv_to_cli, 32);
	print_bytes(" server pubkey x:", &ctx_srv.Q.X, 32);
	ret = mbedtls_mpi_write_binary( &ctx_srv.Q.Y, y, 32 );
	print_bytes(" server pubkey y:", &ctx_srv.Q.Y, 32);
	print_bytes(" server pubkey y01:", y, 32);
	ret = mbedtls_mpi_write_binary( &ctx_srv.Q.Z, z, 32 );
	print_bytes(" server pubkey z:", &ctx_srv.Q.Z, 32);
	print_bytes(" server pubkey z01:", z, 32);
	
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_write_binary returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );

    /*
     * Server: read peer's key and generate shared secret
     */
    mbedtls_printf( "\r\n  3. Server reading client key and computing secret..." );
 //   fflush( stdout );

	ret = mbedtls_mpi_write_binary( &ctx_srv.Qp.Z, z, 32 );
	print_bytes(" ctx_srv Qp.z00:", z, 32);
	
    ret = mbedtls_mpi_lset( &ctx_srv.Qp.Z, 1 );
	
	
	ret = mbedtls_mpi_write_binary( &ctx_srv.Qp.Z, z, 32 );
	print_bytes(" ctx_srv Qp.z01:", z, 32);
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_lset returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_mpi_read_binary( &ctx_srv.Qp.X, cli_to_srv, 32 );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_read_binary returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_ecdh_compute_shared( &ctx_srv.grp, &ctx_srv.z,
                                       &ctx_srv.Qp, &ctx_srv.d,
                                       mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_compute_shared returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );

    /*
     * Client: read peer's key and generate shared secret
     */
    mbedtls_printf( "\r\n  4. Client reading server key and computing secret..." );
 //   fflush( stdout );

    ret = mbedtls_mpi_lset( &ctx_cli.Qp.Z, 1 );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_lset returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_mpi_read_binary( &ctx_cli.Qp.X, srv_to_cli, 32 );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_read_binary returned %d\n", ret );
        goto exit;
    }

    ret = mbedtls_ecdh_compute_shared( &ctx_cli.grp, &ctx_cli.z,
                                       &ctx_cli.Qp, &ctx_cli.d,
                                       mbedtls_ctr_drbg_random, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_compute_shared returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );

    /*
     * Verification: are the computed secrets equal?
     */
    mbedtls_printf( "\r\n  5. Checking if both computed secrets are equal..." );
 //   fflush( stdout );

    ret = mbedtls_mpi_cmp_mpi( &ctx_cli.z, &ctx_srv.z );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_compute_shared returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );

    exit_code = 0;

exit:

#if defined(_WIN32)
    mbedtls_printf( "\r\n  + Press Enter to exit this program.\n" );
    fflush( stdout ); getchar();
#endif

    mbedtls_ecdh_free( &ctx_srv );
    mbedtls_ecdh_free( &ctx_cli );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    mbedtls_exit( exit_code );
	return 0;
}


int mbedtls_ctr_drbg_random_t( void *p_rng, unsigned char *output, size_t output_len )
{
    uint8_t const_key[32] = { 0x98, 0x05, 0x57, 0xac, 0x55, 0xac, 0x49, 0xe0, 0xa8, 0xd5, 0xf2, 0x1a, 0x23, 0x64, 0xb7, 0xa5, 
							  0xb1, 0x5c, 0x01, 0x13, 0x65, 0xd5, 0x92, 0xd7, 0xa2, 0xd5, 0xec, 0xa3, 0x3c, 0xdf, 0xf0, 0xa0};
	uint8_t alice_pri_key[32] =	{0x03, 0xB4, 0x7F, 0x5A, 0x64, 0xF6, 0xF4, 0x5C, 0xBB, 0x4D, 0xF5, 0x32, 0xFA, 0x2F, 0xCA, 0x46, 
								 0x2E, 0xA2, 0x33, 0xD6, 0x28, 0x49, 0x8D, 0x23, 0x3C, 0x60, 0x66, 0xF6, 0xE3, 0xB6, 0xDF, 0x8F};
	uint8_t bob_pub_key[32] =	{0xa5, 0xd2, 0xfc, 0x14, 0x5e, 0xb8, 0x26, 0x0e, 0x03, 0x59, 0xcd, 0x9d, 0xf8, 0x68, 0x68, 0xd8,
								 0xb1, 0xe3, 0xc8, 0xd0, 0x60, 0x2d, 0x6e, 0xe8, 0xd2, 0xc6, 0xb8, 0x14, 0x22, 0xa3, 0x01, 0x75 };
	int ret;
    mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) p_rng;
    
	printf("\r\n !!!!!!!!!!!!!!!!!!! mbedtls_ctr_drbg_random_t start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	printf("\r\n Fixed random value Fixed random value Fixed random value Fixed random value");

	memcpy(output, alice_pri_key, 32); //ecdh test
	ret = 0;

	printf("\r\n mbedtls_ctr_drbg_random_with_add output_len : %d", output_len);
	print_bytes("mbedtls_ctr_drbg_random", output, output_len);
	
	printf("\r\n !!!!!!!!!!!!!!!!!!! mbedtls_ctr_drbg_random_t end!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return( ret );
}

int mbedtls_ctr_drbg_random_s( void *p_rng, unsigned char *output, size_t output_len )
{
    uint8_t const_key[32] = { 0x98, 0x05, 0x57, 0xac, 0x55, 0xac, 0x49, 0xe0, 0xa8, 0xd5, 0xf2, 0x1a, 0x23, 0x64, 0xb7, 0xa5, 
							  0xb1, 0x5c, 0x01, 0x13, 0x65, 0xd5, 0x92, 0xd7, 0xa2, 0xd5, 0xec, 0xa3, 0x3c, 0xdf, 0xf0, 0xa0};
	uint8_t alice_pri_key[32] =	{0x03, 0xB4, 0x7F, 0x5A, 0x64, 0xF6, 0xF4, 0x5C, 0xBB, 0x4D, 0xF5, 0x32, 0xFA, 0x2F, 0xCA, 0x46, 
								 0x2E, 0xA2, 0x33, 0xD6, 0x28, 0x49, 0x8D, 0x23, 0x3C, 0x60, 0x66, 0xF6, 0xE3, 0xB6, 0xDF, 0x8F};
	uint8_t bob_pub_key[32] =	{0xa5, 0xd2, 0xfc, 0x14, 0x5e, 0xb8, 0x26, 0x0e, 0x03, 0x59, 0xcd, 0x9d, 0xf8, 0x68, 0x68, 0xd8,
								 0xb1, 0xe3, 0xc8, 0xd0, 0x60, 0x2d, 0x6e, 0xe8, 0xd2, 0xc6, 0xb8, 0x14, 0x22, 0xa3, 0x01, 0x75 };
	int ret;
    mbedtls_ctr_drbg_context *ctx = (mbedtls_ctr_drbg_context *) p_rng;
    
	printf("\r\n !!!!!!!!!!!!!!!!!!! mbedtls_ctr_drbg_random_t start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	printf("\r\n Fixed random value Fixed random value Fixed random value Fixed random value");

	memcpy(output, alice_pri_key, 32); //ecdh test
	ret = 0;

	printf("\r\n mbedtls_ctr_drbg_random_with_add output_len : %d", output_len);
	print_bytes("mbedtls_ctr_drbg_random", output, output_len);
	
	printf("\r\n !!!!!!!!!!!!!!!!!!! mbedtls_ctr_drbg_random_t end!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    return( ret );
}

void test_mbedtls_ecdh_2(){
		int ret = 1;
    int exit_code = 1;
    mbedtls_ecdh_context ctx_cli, ctx_srv;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    unsigned char cli_to_srv[32], srv_to_cli[32];
    const char pers[] = "ecdh";
//    ((void) argc);
//    ((void) argv);

    mbedtls_ecdh_init( &ctx_cli );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    /*
     * Initialize random number generation
     */
    mbedtls_printf( "\r\n  0. Seeding the random number generator..." );


	/*
     * Client: initialize context and generate keypair
     */
    mbedtls_printf( "\r\n  1. Setting up client context..." );
//    fflush( stdout );

    ret = mbedtls_ecp_group_load( &ctx_cli.grp, MBEDTLS_ECP_DP_SECP256R1 );  
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecp_group_load returned %d\n", ret );
        goto exit;
    }
	printf("\r\n --------------------------------------------------------------------------------------------");
	print_bytes("ctx_cli.d", &ctx_cli.d, 32);
	
    ret = mbedtls_ecdh_gen_public( &ctx_cli.grp, &ctx_cli.d, &ctx_cli.Q,
                                   mbedtls_ctr_drbg_random_t, &ctr_drbg );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_gen_public returned %d\n", ret );
        goto exit;
    }
	printf("\r\n -----------------------------11111111---------------------------------------------------------------");
	mbedtls_mpi_write_binary( &ctx_cli.d, cli_to_srv, 32 );
	print_bytes("client prikey", cli_to_srv, 32);
    ret = mbedtls_mpi_write_binary( &ctx_cli.Q.X, cli_to_srv, 32 );
	print_bytes(" client pubkey :", cli_to_srv, 32);
	
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_write_binary returned %d\n", ret );
        goto exit;
    }

    mbedtls_printf( "\r\n ok\n" );	
	
	//////////////////////////////public key make end
	uint8_t z[32];
	uint8_t bob_pub_key_x[32]=	{0x2a, 0x0c, 0xa7, 0xa5, 0x18, 0x04, 0x90, 0xd3, 0x26, 0x5f, 0xf2, 0x5a, 0x21, 0x44, 0x6b, 0x97, 0x0e, 0x1c, 0xe1, 0xc2, 0x60, 0x06, 0xf8, 0xcc, 0x3f, 0xd6, 0xa2, 0xaf, 0x80, 0x83, 0xe4, 0xb2 };
	uint8_t bob_pub_key_y[32]=	{0x07, 0xaa, 0xd6, 0xd1, 0x04, 0x11, 0xb1, 0xb2, 0x53, 0x4e, 0x5f, 0xc6, 0xf7, 0xa3, 0x9c, 0x06, 0xfd, 0x79, 0x48, 0xb7, 0x3b, 0x17, 0xec, 0xdd, 0x5b, 0x9b, 0x2b, 0xed, 0x5d, 0xa2, 0x03, 0xec };
	
//	ret = mbedtls_mpi_lset( &ctx_srv.Qp.Z, 1 );
//	ret = mbedtls_mpi_write_binary( &ctx_srv.Qp.Z, z, 32 );
//	ret = mbedtls_mpi_read_binary( &ctx_srv.Qp.X, bob_pub_key_x, 32 );
//	ret = mbedtls_mpi_read_binary( &ctx_srv.Qp.Y, bob_pub_key_y, 32 );
		
//	print_bytes(" ctx_srv Qp.z01:", z, 32);
//	print_bytes(" ctx_srv Qp.X01:", bob_pub_key_x, 32);
//	print_bytes(" ctx_srv Qp.Y01:", bob_pub_key_y, 32);
		
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_lset returned %d\n", ret );
        goto exit;
    }
	ret = mbedtls_mpi_lset( &ctx_cli.Qp.Z, 1 );
	
    ret = mbedtls_mpi_read_binary( &ctx_cli.Qp.X, bob_pub_key_x, 32 );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_mpi_read_binary returned %d\n", ret );
        goto exit;
    }
	
	ret = mbedtls_ecdh_compute_shared( &ctx_cli.grp, &ctx_cli.z,
                                       &ctx_cli.Qp, &ctx_cli.d,
                                       NULL, NULL );
    if( ret != 0 )
    {
        mbedtls_printf( "\r\n failed\n  ! mbedtls_ecdh_compute_shared returned %d\n", ret );
        goto exit;
    }

//    mbedtls_printf( "\r\n ok\n" );
	
	
	
	exit:

    mbedtls_ecdh_free( &ctx_cli );
    mbedtls_ctr_drbg_free( &ctr_drbg );
//    mbedtls_entropy_free( &entropy );

    //mbedtls_exit( exit_code );
	
}


int test_ecdh_2(){
	#if 0
	uint32_t alice_pri_key[8] =	{0xff548f30, 0x94a0070b, 0x51a75b87, 0x6a4dc6a0, 0x396797b5, 0x7fe9ddc7, 0xbcd6a5ff, 0xf0f5eeab};
	uint32_t bob_pri_key[8]	=	{0xdc3a837b, 0x7522988c, 0x66bcbd64, 0x86b0a825, 0x6a29e693, 0x82e54c81, 0xe9414829, 0x9a75f08a};
	#else
	//uint32_t alice_pri_key[8] =	{0xE3B6DF8F, 0x3C6066F6, 0x28498D23, 0x2EA233D6, 0xFA2FCA46, 0xBB4DF532, 0x64F6F45C, 0x03B47F5A};
	//uint32_t bob_pri_key[8]	=	{0x82F5C426, 0xAD358BB9, 0xD878A13D, 0x51990BB5, 0x9521DBB1, 0x8DE08E0D, 0xCC637F7C, 0xBC9710E9};
	
	//uint32_t alice_pri_key[8] =	{0x03B47F5A, 0x64F6F45C, 0xBB4DF532, 0xFA2FCA46, 0x2EA233D6, 0x28498D23, 0x3C6066F6, 0xE3B6DF8F};
	
	uint32_t bob_pri_key[8]	=	{0xBC9710E9, 0xCC637F7C, 0x8DE08E0D, 0x9521DBB1, 0x51990BB5, 0xD878A13D, 0xAD358BB9, 0x82F5C426};
	#endif
//	uint32_t bob_pub_key_x[8]=	{0x2a0ca7a5, 0x180490d3, 0x265ff25a, 0x21446b97, 0x0e1ce1c2, 0x6006f8cc, 0x3fd6a2af, 0x8083e4b2};
//	uint32_t bob_pub_key_y[8]=	{0x07aad6d1, 0x0411b1b2, 0x534e5fc6, 0xf7a39c06, 0xfd7948b7, 0x3b17ecdd, 0x5b9b2bed, 0x5da203ec};
//	
//	uint32_t bob_pub_key_x[8] = { 0xa5d2fc14, 0x5eb8260e, 0x0359cd9d, 0xf86868d8, 0xb1e3c8d0, 0x602d6ee8, 0xd2c6b814, 0x22a30175 };
//	uint32_t bob_pub_key_y[8] = { 0x7db6e6ab, 0xf1226556, 0x2364f79c, 0x91fc1824, 0xbdddc734, 0x58775dcb, 0xeffbb591, 0x96692ad2 };
#if 0
	uint32_t alice_pri_key[8] =	{ 0xa0f0df3c, 0xa3ecd5a2, 0xd792d565, 0x13015cb1, 0xa5b76423, 0x1af2d5a8, 0xe049ac55, 0xac570598 };
	uint32_t bob_pub_key_x[8] = { 0xa431e37f, 0x391129ca, 0x5793dbb9, 0xee1e841d, 0x35173195, 0xdd02fbb9, 0x056598c8, 0xadc6255a };
	uint32_t bob_pub_key_y[8] = { 0x873ce131, 0x110734c6, 0x4a782ce0, 0x85ad5820, 0x9ae18923, 0x763844d9, 0x65eb2691, 0xd17bcb64 };
#else	
	uint32_t alice_pri_key[8] =	{ 0x980557ac, 0x55ac49e0, 0xa8d5f21a, 0x2364b7a5, 0xb15c0113, 0x65d592d7, 0xa2d5eca3, 0x3cdff0a0 };
	uint32_t bob_pub_key_x[8] = { 0x5a25c6ad, 0xc8986505, 0xb9fb02dd, 0x95311735, 0x1d841eee, 0xb9db9357, 0xca291139, 0x7fe331a4 };
	uint32_t bob_pub_key_y[8] = { 0x64cb7bd1, 0x9126eb65, 0xd9443876, 0x2389e19a, 0x2058ad85, 0xe02c784a, 0xc6340711, 0x31e13c87 };
#endif	
	uint32_t test_pri_key[8], test_public_x[8],test_public_y[8];
	
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
//	memcpy(bob.pri_key, bob_pri_key, 32);
	
	pkcc_ecc_ecdh_memory_set(&alice);
//	pkcc_ecc_ecdh_memory_set(&bob);
	
//	pkcc_ecc_key_pair_gen(&alice);
//	pkcc_ecc_key_pair_gen(&bob);

	memcpy(bob.ecdh_pub_key->x, bob_pub_key_x, 32);
	memcpy(bob.ecdh_pub_key->y, bob_pub_key_y, 32);
	
	print_bytes("alice_pri_key", alice_pri_key, 32);	
	
//	uint32_inverse(alice_pri_key, alice.pri_key);
//	print_bytes("alice_pri_key", alice.pri_key, 32);
//	
//	uint32_inverse(bob_pub_key_x, bob.ecdh_pub_key->x);
//	print_bytes("bob.ecdh_pub_key->x", bob.ecdh_pub_key->x, 32);
//	
//	uint32_inverse(bob_pub_key_y, bob.ecdh_pub_key->y);
//	print_bytes("bob.ecdh_pub_key->y", bob.ecdh_pub_key->y, 32);
	
	printf("\r\n g.x  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.x[i]); }	
	printf("\r\n g.y  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.curve_point.y[i]); }	
	printf("\r\n mod.n  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.modular_n[i]); }	

	
//	int32_t temp2[8],temp3[8];
//	
//	for( int i=0, j=7; j>=0; j--) {
//		temp2[i] = 	 ((((alice_pri_key[j])&0xff)<<0))
//					|((((alice_pri_key[j])>>8)&0xff)<<8)
//					|((((alice_pri_key[j])>>16)&0xff)<<16)
//					|((((alice_pri_key[j])>>24)&0xff)<<24);
//		i++;
//	}
	

	
	pkcc_shared_key_gen(&alice, bob.ecdh_pub_key);

//	pkcc_shared_key_gen(&bob, alice.ecdh_pub_key);
	
	printf("\r\n alice.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.pri_key[i]); }	
	printf("\r\n alice_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->x[i]); }
	printf("\r\n alice_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.ecdh_pub_key->y[i]); }
	
//	printf("\r\n bob.pri_key  :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.pri_key[i]); }	
//	printf("\r\n bob_public.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->x[i]); }
//	printf("\r\n bob_public.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.ecdh_pub_key->y[i]); }
	printf("\r\n");
	
	printf("\r\n alice_shared_key.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.shared_key->x[i]); }
	printf("\r\n alice_shared_key.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", alice.shared_key->y[i]); }
//	printf("\r\n bob_shared_key.x :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.shared_key->x[i]); }
//	printf("\r\n bob_shared_key.y :\t"); for(int i = 0; i < 8; i++){ printf("0x%08x ", bob.shared_key->y[i]); }
	
//	if(memcmp(alice.shared_key, bob.shared_key, 32)) { 
//		printf("\r\n shared_key not matched \r\n ECDH TEST FAIL!!"); 
//		free(alice.pri_key);
//		free(alice.ecdh_pub_key);
//		free(alice.shared_key);
//		
//		free(bob.pri_key);
//		free(bob.ecdh_pub_key);
//		free(bob.shared_key);
//		return 1;
//	} else {
//		printf("\r\n shared_key matched \r\n ECDH TEST PASS");
//	}

	
	free(alice.pri_key);
	free(alice.ecdh_pub_key);
	free(alice.shared_key);
	
	free(bob.pri_key);
	free(bob.ecdh_pub_key);
	free(bob.shared_key);

	printf("\r\n ecdh_pkcc_function_call_test end");
	
	return 0;
}


void test_mbed_ecdh_3(){
	const uint8_t* pk; size_t pk_len;
    const uint8_t* sk; size_t sk_len;
    uint8_t* secret; size_t secret_len;

    int err = 0;

    mbedtls_ecp_point q;
    mbedtls_ecp_point_init(&q);

    mbedtls_mpi d, z;
    mbedtls_mpi_init(&d);
    mbedtls_mpi_init(&z);

    mbedtls_ecp_group grp;
    mbedtls_ecp_group_init(&grp);
    mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP256R1);

    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);

    const char personalization[] = "sdcpcli_secret_agreement";

    MBEDTLS_CHK(mbedtls_ecp_point_read_binary(&grp, &q, pk, SDCP_PUBLIC_KEY_SIZE_V1));
    MBEDTLS_CHK(mbedtls_ecp_check_pubkey(&grp, &q));

    MBEDTLS_CHK(mbedtls_mpi_read_binary(&d, sk, SDCP_PRIVATE_KEY_SIZE_V1));
    MBEDTLS_CHK(mbedtls_ecp_check_privkey(&grp, &d));

    MBEDTLS_CHK(mbedtls_ctr_drbg_seed(
        &ctr_drbg,
        mbedtls_entropy_func, &entropy,
        (const uint8_t*)personalization, sizeof(personalization)));

    MBEDTLS_CHK(mbedtls_ecdh_compute_shared(
        &grp, &z, &q, &d,
        mbedtls_ctr_drbg_random, &ctr_drbg));

    assert(mbedtls_mpi_size(&z) == SDCP_CURVE_FIELD_SIZE_V1);
    MBEDTLS_CHK(mbedtls_mpi_write_binary(&z, secret, SDCP_CURVE_FIELD_SIZE_V1));

exit:
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_ecp_group_free(&grp);
    mbedtls_mpi_free(&z);
    mbedtls_mpi_free(&d);
    mbedtls_ecp_point_free(&q);

}


//void mbedtls_ecdsa_test(){
//	mbedtls_ecdsa_context ecdsa;
//	const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_info_from_grp_id(MBEDTLS_ECP_DP_SECP256R1);
//	size_t sig_len;
//	unsigned char tmp[200];

//	memset(buf, 0x2A, sizeof(buf));
//	strcpy((char *)buf, "hello world"); //just fill with something other than 0x2A

//	mbedtls_ecdsa_init(&ecdsa);

//	int ret_genkey = mbedtls_ecdsa_genkey(&ecdsa, curve_info->grp_id, myrand, NULL);
//	int ret_write_sign = mbedtls_ecdsa_write_signature(&ecdsa, MBEDTLS_MD_SHA256, buf, curve_info->bit_size, tmp, &sig_len, myrand, NULL);



//	printf("ret_genkey = %d\n", ret_genkey);
//	printf("ret_write_sign = %d\n", ret_write_sign);

//	//ecp_clear_precomputed(&ecdsa.grp);

//	int ret_verify = mbedtls_ecdsa_read_signature(&ecdsa, buf, curve_info->bit_size, tmp, sig_len);
//	printf("ret_verify = %d\n", ret_verify);
//}