#! /usr/bin/python
# Exercises from Week 1 of Crypto course
# Copyright 2020 Kareem Shehata

import binascii
import operator

cyphertext = [
"315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b163" +
"49c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857" +
"553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc415" +
"56bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d5" +
"24400a19b159610b11ef3e",

"234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b" +
"07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205" +
"132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b" +
"028aa76eb7b4ab24171ab3cdadb8356f",

"32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b07df44ba6e9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257527dc29398f5f3251a0d47e503c66e935de81230b59b7afb5f41afa8d661cb",
"315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3",
"271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027",
"466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83",
"32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac650e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef121336cb85ccb8f3315f4b52e301d16e9f52f904",
]

plaintext = [
"We can factor the number #5 with quantum computers# We can also factor the number ",
"Euler would probably enjoy that now his theorem becomes a corner stone of crypto ",
"The nice thing about Keeyloq is now we cryptographers can drive a lot of fancy cars",
"We can see the point where the chip is unhappy if a wrong bit is sent and consumes more power from the environment",
"A (private-key)  encryption scheme states 3 algorithms, namely a procedure for generating keys a procedure for encrypt",
]

def h2l(s):
	return [ ord(i) for i in binascii.unhexlify(s) ]

def xorlist(l1, l2):
	l = min(len(l1), len(l2))
	return [ i ^ j for (i,j) in zip(l1[:l], l2[:l]) ]

cyphertext_l = [ h2l(c) for c in cyphertext ]

def inc_if_exists(d, k):
	if k in d:
		d[k] = d[k] + 1
	else:
		d[k] = 1

def look_for_spaces(ct1, ct2, key):
	ct1l = h2l(ct1)
	ct2l = h2l(ct2)
	mp = xorlist(ct1l, ct2l)
	for i, c in zip(range(len(mp)), mp):
		c = c ^ ord(' ')
		if (c >= ord('a') and c <= ord('z')) or (c >= ord('A') and c <= ord('Z')) :
			k1 = ct1l[i] ^ ord(' ')
			k2 = ct1l[i] ^ c

			if key[i] is None:
				key[i] = {k1 : 1, k2 : 1}
			else:
				inc_if_exists(key[i], k1)
				inc_if_exists(key[i], k2)

			#print "Found:", i, hex(ct1l[i]), hex(ct2l[i]), hex(c), chr(c), hex(k1), hex(k2), hex(ct1l[i] ^ k1), hex(ct1l[i] ^ k2), hex(ct2l[i] ^ k1), hex(ct2l[i] ^ k2)

	return key

key = [None] * max([ len(i) for i in cyphertext_l ])
for i in range(len(cyphertext)):
	for j in range(i+1, len(cyphertext)):
		look_for_spaces(cyphertext[i], cyphertext[j], key)

def filter(k):
	if k is None:
		return 0
	else:
		return max(k.iteritems(), key=operator.itemgetter(1))[0]

def attempt_to_decrypt(ct, key):
	result = ""
	for c, k in zip(ct, key[:len(ct)]):
		if k is None or len(k) <= 0:
			result += "#"
			continue

		best_k = max(k.iteritems(), key=operator.itemgetter(1))
		if best_k[1] > 4:
			result += chr(c ^ best_k[0])
		else:
			result += "(" + ",".join([ chr(c ^ j) + "/" + str(v) for j,v in k.items() ]) + ")"
	return result

def refine_key(key, mt, ct):
	mt = [ ord(ch) for ch in mt ]
	l = min(len(mt), len(key), len(ct))
	for i, m, c in zip(range(l), mt[:l], ct[:l]):
		if m == ord('#'): continue
		k = m ^ c
		key[i] = { k : 99 }

for pt, ct in zip(plaintext, cyphertext_l[:len(plaintext)]):
	refine_key(key, pt, ct)

for i, k in zip(range(len(key)), key):
	print i, ":", k

for i, ct in zip( range(len(cyphertext_l)), cyphertext_l ):
	print i, attempt_to_decrypt(ct, key)
