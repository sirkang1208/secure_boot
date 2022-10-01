#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "functions.h"

/*
    입력 순서
    [1]BL2.bin [2]public_key_2.pem [3]private_key_1.pem
*/
int main(int argc, char *argv[])
{
    // Load 2nd bootloader's image
    FILE *bl2_fp = fopen("./input/BL2.bin", "rb");
    if (bl2_fp == NULL)
    {
        printf("\n[ERR] failed to open 2nd Bootloader File\n");
        return -1;
    }

    // Get the size of 2nd bootloader (bytes)
    fseek(bl2_fp, 0, SEEK_END);
    int bl2_size = ftell(bl2_fp);
    int total_size = 260 + bl2_size; // 260 for public key of app vendor
    fseek(bl2_fp, 0, SEEK_SET);

    // memory allocation
    // bl2 : byte array which saves image of 2nd bootloader
    // hash_input : byte array which saves total image(public key + BL2)
    unsigned char *bl2 = malloc(bl2_size);
    unsigned char *hash_input = malloc(total_size);

    fread(bl2, 1, bl2_size, bl2_fp);

    // Load App Vendor's Public Key
    FILE *pub2_fp = fopen("./input/public_key_2.pem", "r");
    if(pub2_fp == NULL)
    {
        printf("\n[ERR] failed to open Application Vendor's Public Key File\n");
        return -1;
    }
    RSA *rsa_pub2 = PEM_read_RSA_PUBKEY(pub2_fp, NULL, NULL, NULL);

    // Extract n and e(integer) from BIGNUM structure(openssl)
    const BIGNUM *n2_bn = BN_new();
    const BIGNUM *e2_bn = BN_new();
    const BIGNUM *d2_bn = BN_new();
    char *n2_str = NULL;
    char *e2_str = NULL;

    RSA_get0_key(rsa_pub2, &n2_bn, &e2_bn, &d2_bn);

    n2_str = BN_bn2hex(n2_bn);
    e2_str = BN_bn2hex(e2_bn);

    // convert character to hex (Follow the order of pycryptodome)
    int n2_int = 0;
    for (int index = 0; index < 512; index += 2)
    {
        n2_int = char2dec(n2_str[index]) * 16 + char2dec(n2_str[index + 1]) * 1;
        hash_input[255 - index / 2] = n2_int;
    }

    // e = 65,537
    hash_input[256] = 0x01;
    hash_input[257] = 0x00;
    hash_input[258] = 0x01;
    hash_input[259] = 0x00;

    // Fill the remainder of hash_input with bl2
    for (int t = 0; t < bl2_size; t++)
    {
        hash_input[260 + t] = bl2[t];
    }

    // SHA256(openssl)
    // @ input : array hash_input(public key + bl2)
    // @ output : 32byte hash_value
    unsigned char hash_value[SHA256_DIGEST_LENGTH] = {
        0,
    };
    SHA256(hash_input, total_size, hash_value);

    // converting process(to little endian)
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
    FILE *pri1_fp = fopen("./input/private_key_1.pem", "r");
    if(pri1_fp == NULL)
    {
        printf("\n[ERR] failed to open Chip Vendor's Private Key File\n");
        return -1;
    }
    RSA *rsa_pri1 = PEM_read_RSAPrivateKey(pri1_fp, NULL, NULL, NULL);

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

    // Write signature to file
    FILE *sig_fp = fopen("./output/signature.bin", "wb");
    if (sig_fp == NULL)
    {
        printf("[ERR] failed to open Signature File\n");
        return -1;
    }
    fwrite(cipher_text, 1, 256, sig_fp);

    printf("\n[DEBUG] Signing Process Success\n");

    // // Load Chip Vendor's Public Key
    // FILE* pubfp = fopen("./input/public_key_1.pem", "r");
    // RSA* rsa_pub = PEM_read_RSA_PUBKEY(pubfp, NULL, NULL, NULL);
    // Decryption using public key
    /*num = RSA_public_decrypt(num, cipher_text, plain_text, rsa_pub, RSA_NO_PADDING);
    printf("\nDecryption Result : \n");
    for (int i = 0; i < 256; i++)
    {
        printf("%02X ", plain_text[i]);
    }
    printf("\n");*/



    printf("\n... Program END ...\n\n");
    return 0;
}