from platform import mac_ver
import sys
from turtle import pu
import numpy as np
from cryptography import *
from OpenSSL import crypto
import base64

#Global Variable
RSA = crypto.TYPE_RSA
key_size = 2048

#generate key_pair

def getKeySet():
    key = crypto.PKey()
    cert = crypto.X509()
    crypto.PKey.generate_key(key,RSA,key_size)
    cert.set_pubkey(key)
    return key, cert

#generate MACs and combine with the message
def sender(message,key):
    MAC = crypto.sign(key,message.encode(), "sha256")
    print(len(MAC))
    outbound = message.encode() + MAC
    with open("MAC.bin","wb") as fw:
        fw.write(outbound) 
    return outbound

def receiver(inbound, cert):
    MACSize = 256
    message = inbound[:-MACSize]
    MAC = inbound[-MACSize:]
    try:
        crypto.verify(cert, MAC, message, "sha256")
        return message
    except:
        print("Unauthentic")
        return ""

# msg = ["0B","6C","F8","FF","C3","76","75","A5","A9","5B","79","01","64","15","ED","D2","C8","71","79","9A","2C","A1","54","86","B9","FC","22","C9","E1","A6","8B","7D"]

#message = [0x0B,0x6C,0xF8,0xFF,0xC3,0x76,0x75,0xA5,0xA9,0x5B,0x79,0x01,0x64,0x15,0xED,0xD2,0xC8,0x71,0x79,0x9A,0x2C,0xA1,0x54,0x86,0xB9,0xFC,0x22,0xC9,0xE1,0xA6,0x8B,0x7D]
message = "0000000B0000006C000000F8000000FF000000C30000007600000075000000A5000000A90000005B00000079000000010000006400000015000000ED000000D2000000C800000071000000790000009A0000002C000000A10000005400000086000000B9000000FC00000022000000C9000000E1000000A60000008B0000007D"
# byte_array = bytearray(256)
# for i in range(3,256,4):
#     if int((i-3)/4) <32:
#         byte_array[i] = message[int((i-3)/4)]
    
# with open("sha.bin","wb") as f:
#     f.write(byte_array)
# f.close()

# with open("sha.bin","rb") as fr:
#     data = fr.read()

PR, PU = getKeySet()
outbound = sender(message,PR)
message1 = receiver(outbound,PU)
print(message1)



# arr = []
# list_arr = []
# for i in range(0,len(msg),1):
#     arr.append(msg[i].zfill(8))
#     print(arr[i],end="")

# sha = "0x0000000B,0x0000006C,0x000000F8,0x000000FF,0x000000C3,\
#     0x00000076,0x00000075,0x000000A5,0x000000A9,0x0000005B,0x00000079\
#         ,0x00000001,0x00000064,0x00000015,0x000000ED,0x000000D2,0x000000C8,\
#             0x00000071,0x00000079,0x0000009A,0x0000002C,0x000000A1,0x00000054,\
#                 0x00000086,0x000000B9,0x000000FC,0x00000022,0x000000C9,0x000000E1,\
#                     0x000000A6,0x0000008B,0x0000007D"

