
/* ===============================================
	2nd Bootloader image generator
	This version is for Windows Visual Studio.
   ============================================== */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <openssl/applink.c>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "functions.h"

   /*
	   in Python Program
	   subprocess.run([cmd_command], shell=True, check=True)

	   argc == 5
	   argv[0] == exe directory path(default)
	   argv[1] == @@@.bin (bootloader image)
	   argv[2] == @@@.pem (rsa private key)
	   argv[3] == @@@.pem (rsa public key)
	   argv[4] == version number(ex: 10101)
   */
int main(int argc, char* argv[])
{
	// exception
	if (argc != 5)
	{
		printf("\nUsage: @@@.exe ./@@@.bin ./@@@.pem ./@@@.pem version\n");
		return -1;
	}

	// Load 2nd bootloader's image
	FILE* fp_bl2 = fopen(argv[1], "rb");
	if (fp_bl2 == NULL)
	{
		printf("\n[ERR] failed to open 2nd Bootloader File\n");
		return -1;
	}

	// Get the size of 2nd bootloader (bytes)
	fseek(fp_bl2, 0, SEEK_END);
	int bl2_size = ftell(fp_bl2);
	fseek(fp_bl2, 0, SEEK_SET);
	int total_size = 4 + 4 + 260 + bl2_size;

	// hash_input == 4(size) + 4(ver) + 260(pubkey) + others(2nd BL)
	char* hash_input = (char*)calloc(total_size, sizeof(char));

	// hash_input[268:] <= 2nd bootloader image
	fread(hash_input + 4 + 4 + 260, 1, bl2_size, fp_bl2);

	// hash_input[0:3] <= size of BL2
	FILE* fp_size = fopen("./temp_bl2_size.bin", "wb");
	fwrite(&bl2_size, 1, 4, fp_size);
	fclose(fp_size);

	FILE* fp_size2 = fopen("./temp_bl2_size.bin", "rb");
	fread(hash_input, 1, 4, fp_size);
	fclose(fp_size2);

	// hash_input[4:7] <= version
	int ver_int = atoi(argv[4]);
	FILE* fp_ver = fopen("./temp_bl2_ver.bin", "wb");
	fwrite(&ver_int, 1, 4, fp_ver);
	fclose(fp_ver);

	FILE* fp_ver2 = fopen("./temp_bl2_ver.bin", "rb");
	fread(hash_input + 4, 1, 4, fp_ver2);
	fclose(fp_ver2);

	// Load App Vendor's Public Key
	FILE* fp_pub2 = fopen(argv[3], "r");
	if (fp_pub2 == NULL)
	{
		printf("\n[ERR] failed to open Application Vendor's Public Key File\n");
		return -1;
	}
	RSA* rsa_pub2 = PEM_read_RSA_PUBKEY(fp_pub2, NULL, NULL, NULL);

	// Extract n and e(integer) from BIGNUM structure(openssl)
	const BIGNUM* n2_bn = BN_new();
	const BIGNUM* e2_bn = BN_new();
	const BIGNUM* d2_bn = BN_new();
	char* n2_str = NULL;
	char* e2_str = NULL;

	RSA_get0_key(rsa_pub2, &n2_bn, &e2_bn, &d2_bn);

	n2_str = BN_bn2hex(n2_bn);
	e2_str = BN_bn2hex(e2_bn);

	// convert character to hex
	int n2_int = 0;
	for (int index = 0; index < 512; index += 2)
	{
		n2_int = char2dec(n2_str[index]) * 16 + char2dec(n2_str[index + 1]) * 1;
		hash_input[(index / 2) + 4 + 4] = n2_int;
	}

	// converting endian process (4 bytes each)
	for (int i = 0 + 4 + 4; i < 256 + 4 + 4; i += 4)
	{
		for (int j = 0; j < 2; j++)
		{
			swap_char(&hash_input[i + j], &hash_input[i + (3 - j)]);
		}
	}

	// e = 65,537 = 0x0010_0010
	hash_input[256 + 4 + 4] = 0x01;
	hash_input[257 + 4 + 4] = 0x00;
	hash_input[258 + 4 + 4] = 0x01;
	hash_input[259 + 4 + 4] = 0x00;

	// SHA256 (openssl)
	// @ input : hash_input
	// @ output : hash_value
	unsigned char hash_value[SHA256_DIGEST_LENGTH] = {
		0,
	};
	SHA256(hash_input, total_size, hash_value);

	// converting endian process
	for (int i = 0; i < 32; i += 4)
	{
		for (int j = 0; j < 2; j++)
		{
			swap_char(&hash_value[i + j], &hash_value[i + (3 - j)]);
		}
	}

	// RSA start
	unsigned char cipher_text[256] = {
		0,
	};
	unsigned char plain_text[256] = {
		0,
	};

	// Load Chip Vendor's Private Key
	FILE* fp_priv1 = fopen(argv[2], "r");
	if (fp_priv1 == NULL)
	{
		printf("\n[ERR] failed to open Chip Vendor's Private Key File\n");
		return -1;
	}
	RSA* rsa_pri1 = PEM_read_RSAPrivateKey(fp_priv1, NULL, NULL, NULL);

	// Private Key Validity Check
	if (RSA_check_key(rsa_pri1) != 1)
	{
		printf("[ERR] Invalid RSA Private Ű\n");
		return -1;
	}

	// Padding Process
	unsigned char hash_value_padded[256] = {
		0,
	};
	for (int k = 0; k < 32; k++)
	{
		hash_value_padded[k + 4] = hash_value[k];
	}

	// RSA_private_encrypt (openssl)
	// @ input : hash_value_padded
	// @ output : cipher_text
	RSA_private_encrypt(RSA_size(rsa_pri1), hash_value_padded, cipher_text, rsa_pri1, RSA_NO_PADDING);

	// converting endian process
	for (int i = 0; i < 256; i += 4)
	{
		for (int j = 0; j < 2; j++)
		{
			swap_char(&cipher_text[i + j], &cipher_text[i + (3 - j)]);
		}
	}

	// make final image of 2nd bootloader
	FILE* fp_final = fopen("BL2_final_image.bin", "wb");
	if (fp_final == NULL)
	{
		printf("\n[ERR] faile to make final_image.bin\n");
		return -1;
	}

	fwrite(cipher_text, 1, 256, fp_final);
	fwrite(hash_input, 1, total_size, fp_final);

	printf("\n .. Make Final 2nd Bootloader Image Success .. \n");

	free(hash_input);

	fclose(fp_bl2);
	fclose(fp_pub2);
	fclose(fp_priv1);
	fclose(fp_final);

	printf("\n ... Program END ... \n");
	return 0;
}