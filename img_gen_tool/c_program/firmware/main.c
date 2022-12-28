
/* ===============================================
	Firmware image generator
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

	   argc == 4
	   argv[0] == exe directory path(default)
	   argv[1] == @@@.bin (firmware image)
	   argv[2] == @@@.pem (rsa private key)
	   argv[3] == version number(ex: 10101)
   */
int main(int argc, char* argv[])
{
	// exception
	if (argc != 4)
	{
		printf("\nUsage: @@@.exe ./@@@.bin ./@@@.pem version\n");
		return -1;
	}

	// Load firmware's image
	FILE* fp_firmware = fopen(argv[1], "rb");
	if (fp_firmware == NULL)
	{
		printf("\n[ERR] failed to open Firmware File\n");
		return -1;
	}

	// Get the size of Firmware (bytes)
	fseek(fp_firmware, 0, SEEK_END);
	int firmware_size = ftell(fp_firmware);
	fseek(fp_firmware, 0, SEEK_SET);
	int total_size = 4 + 4 + firmware_size;

	// hash_input == 4(size) + 4(ver) + others(Firmware)
	char* hash_input = (char*)calloc(total_size, sizeof(char));

	fread(hash_input + 4 + 4, 1, firmware_size, fp_firmware);

	// hash_input[0:3] = size of Firmware
	FILE* fp_size = fopen("./temp_firm_size.bin", "wb");
	fwrite(&firmware_size, 1, 4, fp_size);
	fclose(fp_size);

	FILE* fp_size2 = fopen("./temp_firm_size.bin", "rb");
	fread(hash_input, 1, 4, fp_size);
	fclose(fp_size2);

	// hash_input[4:7] = version
	int ver_int = atoi(argv[3]);
	FILE* fp_ver = fopen("./temp_firm_ver.bin", "wb");
	fwrite(&ver_int, 1, 4, fp_ver);
	fclose(fp_ver);

	FILE* fp_ver2 = fopen("./temp_firm_ver.bin", "rb");
	fread(hash_input + 4, 1, 4, fp_ver2);
	fclose(fp_ver2);

	// SHA256
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

	// Load Application Vendor's Private Key
	FILE* fp_priv2 = fopen(argv[2], "r");
	if (fp_priv2 == NULL)
	{
		printf("\n[ERR] failed to open Chip Vendor's Private Key File\n");
		return -1;
	}
	RSA* rsa_priv2 = PEM_read_RSAPrivateKey(fp_priv2, NULL, NULL, NULL);

	// Private Key Validity Check
	if (RSA_check_key(rsa_priv2) != 1)
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

	// RSA_private_encrypt
	// @ input : hash_value_padded
	// @ output : cipher_text
	RSA_private_encrypt(RSA_size(rsa_priv2), hash_value_padded, cipher_text, rsa_priv2, RSA_NO_PADDING);

	// converting endian process
	for (int i = 0; i < 256; i += 4)
	{
		for (int j = 0; j < 2; j++)
		{
			swap_char(&cipher_text[i + j], &cipher_text[i + (3 - j)]);
		}
	}

	// make final image of firmware
	FILE* fp_final = fopen("Firm_final_image.bin", "wb");
	if (fp_final == NULL)
	{
		printf("\n[ERR] faile to make final_firmware.bin\n");
		return -1;
	}

	fwrite(cipher_text, 1, 256, fp_final);
	fwrite(hash_input, 1, total_size, fp_final);

	printf("\n .. Make Final Firmware Image Success .. \n");

	free(hash_input);

	fclose(fp_firmware);
	fclose(fp_priv2);
	fclose(fp_final);

	printf("\n ... Program END ... \n");
	return 0;
}