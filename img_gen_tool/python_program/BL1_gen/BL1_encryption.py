from operator import mod
from signal import signal
from sys import byteorder
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

def encryption(bootloader, n_2 = 0, e_2 = 0):
 
    pubkey_n = n_2.to_bytes(256, byteorder = "big")
    pubkey_e = e_2.to_bytes(4, byteorder = "little")

    # for i in range(256):
    #     print(pubkey_n[i],end=" ")
    # print("\n\n")
    
    P_N = bytearray(pubkey_n)
    
    # for i in range(256):
    #     print(P_N[i],end= " ")
    # print("\n\n")    
    
    for i in range(0,len(P_N),4):
        temp = P_N[3+i]
        P_N[3+i] = P_N[0+i]
        P_N[0+i] = temp
        temp = P_N[2+i]
        P_N[2+i] = P_N[1+i]
        P_N[1+i] = temp
    
    # for i in range(256):
    #     print(P_N[i],end= " ")
    # print("\n\n")
        
    # write signature / other data / bootloader(firmware)
    f_out = open('BL1_final_image.bin', 'wb')
    f_out.write(P_N) # 256bytes
    f_out.write(pubkey_e) # 4bytes
    f_out.write(bootloader) # size of 2nd bootloader(bytes)

    f_out.close()
    