from operator import mod
from signal import signal
from sys import byteorder
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

def key_gen(n, e, d):
    # ref : (modulus N, Public exponent e, Private exponent d, opt, opt, opt)
	key_component = (n, d, e)
	key = RSA.construct(key_component)
	return key

def encryption(bootloader, key, n_2 = 0, e_2 = 0, version = 0):
    # make files to check hash value & signature.
    f_DEBUG = open('hash_value.bin', 'wb')
    f_DEBUG2 = open('signature.bin', 'wb')
    
    # hash bootloader(firmware)
    hash_object = SHA256.new(n_2.to_bytes(256, byteorder= "little", signed = True) + e_2.to_bytes(4, byteorder= "little", signed = True) + bootloader)
    hash_value = hash_object.digest()
    f_DEBUG.write(hash_value)
    
    # encrypt hash value with private key
    encryptor = PKCS1_OAEP.new(key)
    signature = encryptor.encrypt(hash_value)
    f_DEBUG2.write(signature)
 
    pubkey_n = n_2.to_bytes(256, byteorder = "little")
    pubkey_e = e_2.to_bytes(4, byteorder = "little")
    
    length = len(bootloader)
    size = length.to_bytes(4, byteorder = "little")
    ver = version.to_bytes(4, byteorder="little")
    
    # write signature / other data / bootloader(firmware)
    f_out = open('enc_BL2.bin', 'wb')
    f_out.write(signature) # 256bytes
    f_out.write(size) # 4bytes
    f_out.write(ver) # 4bytes
    f_out.write(pubkey_n) # 256bytes
    f_out.write(pubkey_e) # 4bytes
    f_out.write(bootloader) # size of 2nd bootloader(bytes)
    
    f_DEBUG.close()
    f_DEBUG2.close()
    f_out.close()
    
    test_sim(key)
    
    return signature
    
def test_sim(key):
    F_in = open('enc_BL2.bin', 'rb')
    signature = F_in.read(256)
    size = F_in.read(4)
    ver = F_in.read(4)
    pubkey_n = F_in.read(256)
    pubkey_e = F_in.read(4)
    bootloader = F_in.read()
    
    decryptor = PKCS1_OAEP.new(key)
    hash_from_sig = decryptor.decrypt(signature)
    hash_object = SHA256.new(pubkey_n + pubkey_e + bootloader)
    hash_calculated = hash_object.digest()

    if (hash_from_sig == hash_calculated):
        print(" ... 검증 통과 ... ")
    else:
        print(" ... 검증 실패 ... ")

    F_in.close()


# hash_object = SHA256.new(data=b'First')
# hash_object.update(b'Second')
# hash_object.update(b'Third')
# 위 두 줄은 hash_object.update(b'SecondThird') 이거랑 같음