#! /usr/bin/python
# Exercises from Week 2 of Crypto course
# Copyright 2020 Kareem Shehata

import binascii
import Crypto.Cipher.AES
import struct

BLOCK_SIZE = 16

def split_blocks(s):
	return [ s[i:i + BLOCK_SIZE] for
	  i in range(0, len(s), BLOCK_SIZE) ]

def xor_block(b1, b2):
	return ''.join([chr(ord(_a) ^ ord(_b)) for _a, _b in zip(b1, b2)])

def remove_pad(m):
	pad_length = ord(m[-1:])

	if pad_length > len(m):
		raise Exception("Pad length longer than message")

	for c in m[-pad_length:]:
		if ord(c) != pad_length:
			raise Exception("Invalid pad")

	return m[:-pad_length]

def decrypt_cbc(k, c):
	if len(c) % BLOCK_SIZE != 0:
		raise Exception("Ciphertext not a multiple of block size")
	if len(c) / BLOCK_SIZE < 2:
		return ""

	p = c[:BLOCK_SIZE]
	cipher = Crypto.Cipher.AES.new(k)
	m = ""

	for b in split_blocks(c[BLOCK_SIZE:]):
		m = m + xor_block(cipher.decrypt(b), p)
		p = b

	return remove_pad(m)

def increment_block(b):
	b2 = b""
	for i in range(len(b)-1, 0, -1):
		v = ord(b[i:i+1]) + 1
		if v > 255:
			b2 = chr(0) + b2
		else:
			return b[:i] + chr(v) + b2

	# rollover condition, should be all zeros
	return b2
			

def decrypt_ctr(k, c):
	if len(c) <= BLOCK_SIZE:
		return ""

	ctr = c[:BLOCK_SIZE]
	cipher = Crypto.Cipher.AES.new(k)
	m = ""

	for i in range(BLOCK_SIZE, len(c), BLOCK_SIZE):
		t = cipher.encrypt(ctr)
		m = m + xor_block(c[i : i + BLOCK_SIZE], t)
		ctr = increment_block(ctr)

	return m

def cbc():
	key = binascii.unhexlify('140b41b22a29beb4061bda66b6747e14')
	ciphertext1 = binascii.unhexlify('4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81')
	ciphertext2 = binascii.unhexlify('5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253')

	print "Question 1"
	print "Key =", binascii.hexlify(key)
	print "CT =", binascii.hexlify(ciphertext1)
	print "M =", decrypt_cbc(key, ciphertext1)
	print ""

	print "Question 2"
	print "Key =", binascii.hexlify(key)
	print "CT =", binascii.hexlify(ciphertext2)
	print "M =", decrypt_cbc(key, ciphertext2)
	print ""

def ctr():
	key = binascii.unhexlify('36f18357be4dbd77f050515c73fcf9f2')
	ciphertext3 = binascii.unhexlify('69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329')
	ciphertext4 = binascii.unhexlify('770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451')

	print "Question 3"
	print "Key =", binascii.hexlify(key)
	print "CT =", binascii.hexlify(ciphertext3)
	print "M =", decrypt_ctr(key, ciphertext3)
	print ""

	print "Question 4"
	print "Key =", binascii.hexlify(key)
	print "CT =", binascii.hexlify(ciphertext4)
	print "M =", decrypt_ctr(key, ciphertext4)
	print ""

cbc()
ctr()
