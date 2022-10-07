
/*
    This version is for Windows Visual Studio.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <openssl/applink.c>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

/*
    in Python Program
    cmd_command = "~.exe" + " " + "./input/firmware.bin" + " " + "./input/private_key_2.pem"
    subprocess.run([cmd_command], shell=True, check=True)
    argv[0] == exe directory path
    argv[1] == "./firmware.bin"
    argv[2] == "./private_key_2.pem"
*/
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("\nfirmware.exe <./Bootloader.bin> <./private.pem>\n");
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

    printf("\n[DEBUG] firmware_size = %d\n", firmware_size);

    // memory allocation
    // firmware : byte array which saves image of Firmware
    unsigned char* firmware = (char*)malloc(sizeof(char) * firmware_size);

    fread(firmware, 1, firmware_size, fp_firmware);

    // SHA256(openssl)
    // @ input : firmware
    // @ output : 32byte hash_value
    unsigned char hash_value[SHA256_DIGEST_LENGTH] = {
        0,
    };
    SHA256(firmware, firmware_size, hash_value);

    // converting endian process
    unsigned char temp = 0x00;
    for (int i = 0; i < 32; i += 4)
    {
        for (int j = 0; j < 2; j++)
        {
            temp = hash_value[i + j];
            hash_value[i + j] = hash_value[i + (3 - j)];
            hash_value[i + (3 - j)] = temp;
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

    // RSA_private_encrypt (openssl)
    // @ input : hash_value_padded
    // @ output : cipher_text
    RSA_private_encrypt(RSA_size(rsa_priv2), hash_value_padded, cipher_text, rsa_priv2, RSA_NO_PADDING);

    // converting endian process
    unsigned char temp_2 = 0x00;
    for (int i = 0; i < 256; i += 4)
    {
        for (int j = 0; j < 2; j++)
        {
            temp_2 = cipher_text[i + j];
            cipher_text[i + j] = cipher_text[i + (3 - j)];
            cipher_text[i + (3 - j)] = temp_2;
        }
    }

    // Write signature to file
    FILE* fp_sig = fopen("signature.bin", "wb");
    if (fp_sig == NULL)
    {
        printf("[ERR] failed to open Signature File\n");
        return -1;
    }
    fwrite(cipher_text, 1, 256, fp_sig);

    printf("\n[DEBUG] Signing Process Success\n");

    FILE* final_fp = fopen("final_firmware.bin", "wb");
    if (final_fp == NULL)
    {
        printf("\n[ERR] faile to make final_firmware.bin\n");
        return -1;
    }

    char ver_num[4] = { 0x01, 0x00, 0x01, 0x00 };
    fwrite(cipher_text, 1, 256, final_fp);
    fwrite(&firmware_size, 1, 4, final_fp);
    fwrite(ver_num, 1, 4, final_fp);
    fwrite(firmware, 1, firmware_size, final_fp);

    printf("\n[DEBUG] Make Final Firmware Image Success\n");

    free(firmware);

    fclose(fp_firmware);
    fclose(fp_priv2);
    fclose(fp_sig);
    fclose(final_fp);

    printf("\n... Program END ...\n");
    return 0;
}