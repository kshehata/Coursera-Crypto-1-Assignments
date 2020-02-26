#! /usr/bin/python
# Exercises from Week 4 of Coursera's Crypto course
# Copyright 2020 Kareem Shehata
#
# Decrypts a given ciphertext by querying a "padding oracle", web services that
# returns a different error code for padding errors than malformed messages.

import functools
import itertools
import urllib2
import sys
from binascii import hexlify, unhexlify
from multiprocessing import Pool

TARGET = 'http://crypto-class.appspot.com/po?er='
BLOCK_SIZE = 16

def check_padding(q):
  """ Returns True if padding is good on query, False otherwise"""
  target = TARGET + urllib2.quote(hexlify(q))
  req = urllib2.Request(target)
  try:
    f = urllib2.urlopen(req)
  except urllib2.HTTPError, e:          
    # print "Padding Oracle returned code", e.code
    return (e.code == 404)
  #print "Actually worked!"
  # this means we actually reproduced the message itself
  return False

def xor_guess(data, block_index, guess):
  start_byte = BLOCK_SIZE*(block_index + 1) - len(guess)
  new_chunk = "".join([ chr(ord(i) ^ j ^ ord(k)) for (i, j, k) in zip(
    data[start_byte:start_byte + len(guess)], [len(guess)]*len(guess), guess)])
  return (data[:start_byte] + new_chunk + data[BLOCK_SIZE*(block_index+1):])

def check_guess(ct, block_index, g):
  ct = ct[:BLOCK_SIZE*(block_index+2)]
  c2 = xor_guess(ct, block_index, g)
  # print ("Checking guess on block %d byte %d value %d ct %s"
  #   % (block_index, byte_index, v, hexlify(g)))
  return check_padding(c2)

def generate_guesses(suffix):
  return [ chr(i) + suffix for i in range(256) ]

def extract_result(r, b, i, offset=0):
  if r.count(True) <= 0:
    print "Value not found for block %d offset %d" % (b, i)
    raise Error("Value not found for block %d offset %d" % (b, i))
  elif r.count(True) > 1:
    print "More than one value found for block %d offset %d" % (b, i)

  return chr(r.index(True) + offset)


def decrypt(ct):
  num_blocks = len(ct) / BLOCK_SIZE - 1
  pool = Pool(processes=300)

  m = [""] * num_blocks
  for i in range(BLOCK_SIZE):
    results = []
    for b in range(num_blocks - 1):
      results.append(pool.map_async(functools.partial(check_guess, ct, b),
        generate_guesses(m[b])))

    for b in range(num_blocks - 1):
      m[b] = extract_result(results[b].get(), b, i) + m[b]
      print "Block %d so far is %s" % (b, hexlify(m[b]))

  print "Block messages are:", m

  # Last block special case
  # First figure out pad
  b = num_blocks - 1
  r = pool.map(functools.partial(check_guess, ct, b),
    [ chr(i) for i in range(1,17)])

  pad_length = ord(extract_result(r, b, 0, 1))
  print "Pad is length %d" % pad_length

  # Can't really check this with oracle, since the pad will cancel out.
  # Just assume it's good and move on
  m[b] = chr(pad_length) * pad_length

  for i in range(BLOCK_SIZE - pad_length):
    r = pool.map(functools.partial(check_guess, ct, b), generate_guesses(m[b]))
    m[b] = extract_result(r, b, i) + m[b]
    print "Block %d so far is %s" % (b, hexlify(m[b]))

  # remove pad from last block
  m[b] = m[b][:-pad_length]
  return "".join(m)

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print "Usage: padding_oracle.py [ciphertext]"
    exit(1)

  ct = unhexlify(sys.argv[1])
  
  m = decrypt(ct)
  print "Recoverd message: " + m
