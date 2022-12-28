#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sdcpclient.h"
#include "testkeys.h"
#include "helpers.h"

#include "mbedtls/aes.h"

#include "CMSDK_CM3.h"
//#define GPIO0      0x01    
//#define GPIO0_LOW   0xFFFFFFFE

#define ASSERT_EQUAL(a, b) assert((a) == (b))

int sdcp()
{
	
	#if 1
    // Dump the model keys
    puts("\r\nModel and device keys:\r\n----------------------\n");
    print_bytes("pk_m", g_model_pub_key, sizeof(g_model_pub_key));
    print_bytes("sk_m", g_model_priv_key, sizeof(g_model_priv_key));

    // First, the device is provisioned at the factory, which
    // results in the model signature.
    puts("\r\nFactory provisioning:\r\n---------------------\n");
    sdcp_keypair device_key = { 0 };
//	print_bytes("device_key.pk", device_key.pk, sizeof(device_key.pk));
//	print_bytes("device_key.sk", device_key.sk, sizeof(device_key.sk));
	
//	CMSDK_GPIO0->DATAOUT |=  0x01;
    ASSERT_EQUAL(0, sdcpcli_keygen(&device_key));
//	CMSDK_GPIO0->DATAOUT &=  0xFFFFFFFE;
    print_bytes("pk_d", device_key.pk, sizeof(device_key.pk));
    print_bytes("sk_d", device_key.sk, sizeof(device_key.sk));

	
    uint8_t key_hash[SDCP_DIGEST_SIZE_V1] = { 0 };
    ASSERT_EQUAL(0, sdcpcli_hash(device_key.pk, sizeof(device_key.pk), key_hash, sizeof(key_hash)));
		print_bytes("device_key.pk", device_key.pk, sizeof(device_key.pk));
    print_bytes("key_hash", key_hash, sizeof(key_hash));
	

    sdcp_signature model_signature = { 0 };
    ASSERT_EQUAL(0, sdcpcli_sign_hash(g_model_priv_key, sizeof(g_model_priv_key),key_hash, sizeof(key_hash), &model_signature));
		print_bytes("s_m", (const uint8_t*)&model_signature, sizeof(model_signature));

    // The model private key is never used outside of the factory. It
    // must never be present on the device. Zero it out now.
    secure_zero_memory(g_model_priv_key, sizeof(g_model_priv_key));
		
    // At boot-time, the bootloader measures and signs the
    // device firmware with the device private key, which is only
    // accessible to the bootloader itself.
    puts("\nDevice bootloader:\n------------------\n");
		
    // Clearly bogus firmware data.
    const uint8_t firmware[] = { 'f', 'i', 'r', 'm', 'w', 'a', 'r', 'e' };
    print_bytes("firmware", firmware, sizeof(firmware));

    // Sign the firmware and generate the ECDH firmware key pair.
    sdcp_keypair firmware_key = { 0 };
    sdcp_firmware_signature firmware_signature = { 0 };
	
    ASSERT_EQUAL(0, sdcpcli_sign_firmware(
        device_key.sk, sizeof(device_key.sk),
        firmware, sizeof(firmware),
        &firmware_key,
        &firmware_signature));
		
    print_bytes("h_f", firmware_signature.hash, sizeof(firmware_signature.hash));
    print_bytes("pk_f", firmware_key.pk, sizeof(firmware_key.pk));
    print_bytes("sk_f", firmware_key.sk, sizeof(firmware_key.sk));
    print_bytes("s_d", (const uint8_t*)&firmware_signature.sig, sizeof(firmware_signature.sig));
	
    // Zero the device private key to ensure that the code running after
    // the bootloader does not have access to it.		
    secure_zero_memory(device_key.sk, sizeof(device_key.sk));	
    #endif
		
	#if 1
    // During the connection protocol, the host will send its public
    // ECDH key, as well as some random bytes. The client uses this
    // to do key agreement, and creates an attestation claim
    puts("\nConnect:\n--------\n");
    uint8_t host_random[SDCP_RANDOM_SIZE_V1] = { 0 };
    ASSERT_EQUAL(0, sdcpcli_gen_rand(host_random, sizeof(host_random)));
    print_bytes("h_r", host_random, sizeof(host_random));

    sdcp_keypair host_key = { 0 }; 
	ASSERT_EQUAL(0, sdcpcli_keygen(&host_key));
    print_bytes("pk_h", host_key.pk, sizeof(host_key.pk));

    uint8_t device_random[SDCP_RANDOM_SIZE_V1] = { 0 };
    ASSERT_EQUAL(0, sdcpcli_gen_rand(device_random, sizeof(device_random)));	
    print_bytes("r_d", device_random, sizeof(device_random));

    uint8_t device_master_secret[SDCP_DIGEST_SIZE_V1] = { 0 };	
    ASSERT_EQUAL(0, sdcpcli_derive_master_secret( ///not free
        host_key.pk, sizeof(host_key.pk),
        firmware_key.sk, sizeof(firmware_key.sk),
        host_random, sizeof(host_random),
        device_random, sizeof(device_random),
        device_master_secret, sizeof(device_master_secret)));		
    print_bytes("ms", device_master_secret, sizeof(device_master_secret));

    sdcp_application_keys app_keys = { 0 };		
    ASSERT_EQUAL(0, sdcpcli_derive_application_keys(
        device_master_secret, sizeof(device_master_secret),
        &app_keys));				
    print_bytes("s", app_keys.s, sizeof(app_keys.s));
    print_bytes("k", app_keys.k, sizeof(app_keys.k));
		
    secure_zero_memory(device_master_secret, sizeof(device_master_secret));

    // Create and MAC the attestation claim. We do this in two stages
    // client-side to allow caching of the claim hash, which will remain the
    // same for this boot cycle.
    //
    // There is no support for real certificate validation yet.
    // For now, just pass in the model public key in place of the
    // ASN.1 DER encoded X509 cert.
    uint8_t claim_hash[SDCP_DIGEST_SIZE_V1] = { 0 };
	
    ASSERT_EQUAL(0, sdcpcli_hash_claim(
        g_model_pub_key, sizeof(g_model_pub_key), // Pub key in place of cert for now.
        device_key.pk, sizeof(device_key.pk),
        firmware_key.pk, sizeof(firmware_key.pk),
        &firmware_signature,
        &model_signature,
        claim_hash, sizeof(claim_hash)));		
    print_bytes("H(c)", claim_hash, sizeof(claim_hash));

    uint8_t mac[SDCP_DIGEST_SIZE_V1] = { 0 };
    ASSERT_EQUAL(0, sdcpcli_mac_claim_hash(
        &app_keys,
        claim_hash, sizeof(claim_hash),
        mac, sizeof(mac)));	
	print_bytes("m", mac, sizeof(mac));
    // Now simulate a reconnect. The host will send new random bytes for
    // the client to MAC.
    puts("\nReconnect:\n----------\n");		
    ASSERT_EQUAL(0, sdcpcli_gen_rand(host_random, sizeof(host_random)));	
    print_bytes("r", host_random, sizeof(host_random));
		
    ASSERT_EQUAL(0, sdcpcli_mac_reconnect(
        &app_keys,
        host_random, sizeof(host_random),
        mac, sizeof(mac)));		
    print_bytes("m", mac, sizeof(mac));
#endif
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 1
static const char hello_str[] = "Hello, world!Hello, world!Hello, world!Hello, world!Hello, world!Hello, world!Hello, world!Hello, world!";
static const size_t hello_len = 104;
#else
static const char hello_str[] = "Hello, world!";
static const size_t hello_len = 13;
#endif
static const unsigned char *hello_buffer = (const unsigned char *) hello_str;
//static const size_t hello_len = sizeof(hello_str);


static void print_hex(const char *title, const unsigned char buf[], size_t len)
{
    printf("%s: ", title);

    for (size_t i = 0; i < len; i++) {
        if (buf[i] < 0xF) {
            printf("0%x", buf[i]);
        } else {
            printf("%x", buf[i]);
        }
    }

    printf("\n");
}

void mbedtls_sha256_test(){
	 printf("\n\n");

    /*
     * Method 1: use all-in-one function of a specific SHA-xxx module
     */
    unsigned char output1[32]; /* SHA-256 outputs 32 bytes */

    /* 0 here means use the full SHA-256, not the SHA-224 variant */
    mbedtls_sha256(hello_buffer, hello_len, output1, 0);

    print_hex("Method 1", output1, sizeof output1);
	

    printf("\nDONE\n");
}

static const unsigned char aes_test_ecb_dec[3][16] =
{
    { 0x44, 0x41, 0x6A, 0xC2, 0xD1, 0xF5, 0x3C, 0x58,
      0x33, 0x03, 0x91, 0x7E, 0x6B, 0xE9, 0xEB, 0xE0 },
    { 0x48, 0xE3, 0x1E, 0x9E, 0x25, 0x67, 0x18, 0xF2,
      0x92, 0x29, 0x31, 0x9C, 0x19, 0xF1, 0x5B, 0xA4 },
    { 0x05, 0x8C, 0xCF, 0xFD, 0xBB, 0xCB, 0x38, 0x2D,
      0x1F, 0x6F, 0x56, 0x58, 0x5D, 0x8A, 0x4A, 0xDE }
};

static const unsigned char aes_test_ecb_enc[3][16] =
{
    { 0xC3, 0x4C, 0x05, 0x2C, 0xC0, 0xDA, 0x8D, 0x73,
      0x45, 0x1A, 0xFE, 0x5F, 0x03, 0xBE, 0x29, 0x7F },
    { 0xF3, 0xF6, 0x75, 0x2A, 0xE8, 0xD7, 0x83, 0x11,
      0x38, 0xF0, 0x41, 0x56, 0x06, 0x31, 0xB1, 0x14 },
    { 0x8B, 0x79, 0xEE, 0xCC, 0x93, 0xA0, 0xEE, 0x5D,
      0xFF, 0x30, 0xB4, 0xEA, 0x21, 0x63, 0x6D, 0xA4 }
};

int mbedtls_aes_test(){
	int i, len, output_length;
    unsigned char key[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
							  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
    unsigned char temp[32];
//    unsigned char p_i[32] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
//    unsigned char p_i[32] = { 0xf2, 0x90, 0x00, 0x87, 0x2a, 0x49, 0x9f, 0xe0, 0x53, 0x4e, 0xcc, 0x93, 0xca, 0xdb, 0xea, 0x2f };
	unsigned char p_i[32] = { 0xdc, 0x95, 0xc0, 0x78, 0xa2, 0x40, 0x89, 0x89, 0xad, 0x48, 0xa2, 0x14, 0x92, 0x84, 0x20, 0x87 };
	unsigned char p_o[32], output[32];
	
    mbedtls_aes_context ctx;

    len = 4;
	output_length = 16;
    // Check if input is multiple of 32
    //

    // Check if there is enough space in the output buffer
    //
    
    

    // Set-up the MySQL key
    //
    //memset( key, 0, 16 );
	
	for(int j=0; j<32; j++) p_i[j] = 0;

	uint8_t expect1[16] = { 0xf2, 0x90, 0x00, 0xb6, 0x2a, 0x49, 0x9f, 0xd0, 0xa9, 0xf3, 0x9a, 0x6a, 0xdd, 0x2e, 0x77, 0x80 };
	
	mbedtls_aes_init( &ctx );
    mbedtls_aes_setkey_enc( &ctx, key, 256 );
    mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_ENCRYPT, p_i, p_o );


	
	print_bytes("key", key, 32);
	print_bytes("pt", p_i, 16);
	print_bytes("ct", p_o, 16);
	print_bytes("expect2", expect1, 16);
	if(memcmp(expect1, p_o, 16)) printf("\r\n AES 1 ENC FAIL");
	else printf("\r\n AES 1 ENC PASS");	
	uint8_t key02[32] 	= { 0x08, 0x35, 0xe1, 0x07, 0xbc, 0x7e, 0x6f, 0x9b, 0x6a, 0x8b, 0xbd, 0xcb, 0x77, 0xd7, 0x8a, 0x00, 0x2e, 0xde, 0xe2, 0xae, 0xcc, 0x34, 0x8b, 0xdb, 0x81, 0x3a, 0xaf, 0xa0, 0x4a, 0xc0, 0x18, 0xa0 };
	uint8_t pt2[16] 	= { 0xbb, 0x27, 0xb0, 0x75, 0x32, 0x75, 0x9f, 0x64, 0x5e, 0x9c, 0x1f, 0x9a, 0x2d, 0x3d, 0xb6, 0x6c };
	uint8_t expect2[16] = { 0x16, 0xa9, 0xbd, 0x29, 0xd7, 0x67, 0x97, 0xae, 0x38, 0xd3, 0xcb, 0x70, 0xcd, 0x6d, 0xf8, 0x1b };
	
	memset(key02, 0, 32);
	memset(pt2, 0, 32);
	pt2[15] = 2;
	
	mbedtls_aes_setkey_enc( &ctx, key02, 256 );
    mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_ENCRYPT, pt2, p_o );


	
	print_bytes("key", key02, 32);
	print_bytes("pt02", pt2, 16);
	print_bytes("ct02", p_o, 16);
	print_bytes("expect2", expect2, 16);
	if(memcmp(expect2, p_o, 16)) printf("\r\n AES 2 ENC FAIL");
	else printf("\r\n AES 2 ENC PASS");
#if 0
    mbedtls_aes_setkey_dec( &ctx, key, 256 );
    mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_DECRYPT, p_o, temp );

	print_bytes("key", key, 32);
	print_bytes("ct", p_o, 16);
	print_bytes("dt", temp, 16);
#endif

    return( 0 );

}


