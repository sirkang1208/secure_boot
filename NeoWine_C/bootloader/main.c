
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

#include "functions.h"

/*
    in Python Program
    cmd_command = "~.exe" + " " + "./input/BL2.bin" + " " + "./input/private_key_1.pem" + " " + "./input/public_key_2.pem"
    subprocess.run([cmd_command], shell=True, check=True)
    argv[0] == exe directory path
    argv[1] == "./input/BL2.bin"
    argv[2] == "./input/private_key_1.pem"
    argv[3] == "./input/public_key_2.pem"
    argv[4] == version "10101" (1.1.1)
*/
int main(int argc, char* argv[])
{
    // exception
    if (argc != 5)
    {
        printf("\nNeoRSA.exe <./Bootloader.bin> <./private.pem> <./public.pem> <version>\n");
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
    int total_size = 4 + 260 + bl2_size; // 4 for ver, 260 for public key of app vendor
    fseek(fp_bl2, 0, SEEK_SET);

    // printf("\n[DEBUG] bl2_size = %d, total_size = %d\n", bl2_size, total_size);

    // memory allocation
    // hash_input : byte array which saves total image(public key + BL2)
    char* hash_input = (char*)calloc(total_size, sizeof(char));

    fread(hash_input+264, 1, bl2_size, fp_bl2);

    int ver_int = atoi(argv[4]);
    FILE* fp_ver = fopen("./ver.bin", "wb");
    fwrite(&ver_int, 1, 4, fp_ver);
    fclose(fp_ver);

    FILE* fp_ver2 = fopen("./ver.bin", "rb");
    fread(hash_input, 4, 1, fp_ver2);
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

    // convert character to hex (Follow the order of pycryptodome)
    int n2_int = 0;
    for (int index = 0; index < 512; index += 2)
    {
        n2_int = char2dec(n2_str[index]) * 16 + char2dec(n2_str[index + 1]) * 1;
        hash_input[(255 - index / 2) + 4] = n2_int;
    }

    // converting endian process
    unsigned char temp_3 = 0x00;
    for (int i = 0+4; i < 256+4; i += 4)
    {
        for (int j = 0; j < 2; j++)
        {
            temp_3 = hash_input[i + j];
            hash_input[i + j] = hash_input[i + (3 - j)];
            hash_input[i + (3 - j)] = temp_3;
        }
    }

    // e = 65,537
    hash_input[256+4] = 0x01;
    hash_input[257+4] = 0x00;
    hash_input[258+4] = 0x01;
    hash_input[259+4] = 0x00;

    // SHA256(openssl)
    // @ input : array hash_input(public key + bl2)
    // @ output : 32byte hash_value
    unsigned char hash_value[SHA256_DIGEST_LENGTH] = {
        0,
    };
    SHA256(hash_input, total_size, hash_value);

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

    // // Load Chip Vendor's Public Key
    // FILE* pubfp = fopen("./input/public_key_1.pem", "r");
    // RSA* rsa_pub = PEM_read_RSA_PUBKEY(pubfp, NULL, NULL, NULL);
    // Decryption using public key
    // num = RSA_public_decrypt(num, cipher_text, plain_text, rsa_pub, RSA_NO_PADDING);
    // printf("\nDecryption Result : \n");
    // for (int i = 0; i < 256; i++)
    // {
    //     printf("%02X ", plain_text[i]);
    // }

    FILE* fp_final = fopen("final_image.bin", "wb");
    if (fp_final == NULL)
    {
        printf("\n[ERR] faile to make final_image.bin\n");
        return -1;
    }

    fwrite(cipher_text, 1, 256, fp_final);
    fwrite(&bl2_size, 1, 4, fp_final);
    fwrite(hash_input, 1, total_size, fp_final);

    printf("\n[DEBUG] Make Final Image Success\n");

    free(hash_input);

    fclose(fp_bl2);
    fclose(fp_pub2);
    fclose(fp_priv1);
    fclose(fp_sig);
    fclose(fp_final);

    printf("\n... Program END ...\n");
    return 0;
}