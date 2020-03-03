#! /usr/bin/python
# Exercises from Week 3 of Crypto course
# Copyright 2020 Kareem Shehata
#
# Verifies the hashes of a video file with hashes of the next block added at 
# end of every 1024 bytes.

from binascii import hexlify, unhexlify
from Crypto.Hash import SHA256
import os
import sys

BLOCK_SIZE = 1024
HASH_SIZE = 32

def usage():
  print "Usage: verify_hash.py [input file] [first hash]"

if __name__ == "__main__":
  if len(sys.argv) != 3:
    usage()
    exit(1)

  last_hash = unhexlify(sys.argv[2])
  valid = True
  with open(sys.argv[1]) as input_file:
    while True:
      start_loc = input_file.tell()
      data = input_file.read(BLOCK_SIZE + HASH_SIZE)
      if len(data) <= 0: break
      hash = SHA256.new(data).digest()
      if hash != last_hash:
        print ("HASH CHECK FAILED FOR " + str(start_loc) + ", expected "
          + hexlify(last_hash) + " got " + hexlify(hash))
        valid = False
      if len(data) >= HASH_SIZE:
        last_hash = data[-HASH_SIZE:]

  if valid:
    print "Hashes validated correct"
  else:
    exit(1)
