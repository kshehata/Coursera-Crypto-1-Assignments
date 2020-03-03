#! /usr/bin/python
# Exercises from Week 3 of Crypto course
# Copyright 2020 Kareem Shehata
#
# Implements a function that hashes 1kb blocks of a file (assumed to be a video)
# and appends the hash to the previous 1 block.

from binascii import hexlify
from Crypto.Hash import SHA256
import os
import sys

BLOCK_SIZE = 1024

def usage():
  print "Usage: video_hash.py [input file] [output_file]"

def hash_block(file, last_hash, size=1024):
  b = file.read(size)
  h = SHA256.new()
  h.update(b)
  if (last_hash != None):
    h.update(last_hash)
  return h.digest()

if __name__ == "__main__":
  if len(sys.argv) != 3:
    usage()
    exit(1)

  input_filename = sys.argv[1]
  input_size = os.stat(input_filename).st_size
  if (len <= 0):
    print "No data in input file " + input_filename
    usage()
    exit(2)

  input_file = open(sys.argv[1], 'rb')
  output_file = open(sys.argv[2], 'wb')

  # Start at the end of the file and work backwards
  # May not have a full block at the end, so get initial size
  size = input_size % BLOCK_SIZE
  # Move to end of file
  input_file.seek(0, 2)
  # Don't have a previous hash for last block
  last_hash = None
  # Keep hashes so we can use them to write output file
  # NB: list is in reverse order!
  hashes = []
  while(input_file.tell() > 0):
    # Move back the block size so we can read that in
    input_file.seek(-size, 1)
    last_hash = hash_block(input_file, last_hash, size)
    # Move back again so that we're at the initial position
    input_file.seek(-size, 1)
    # Debug output
    #print input_file.tell(), hexlify(last_hash)
    # Add hash to the list
    hashes.append(last_hash)
    # After initial block at the end of the file, use standard block size
    size = BLOCK_SIZE

  # Print out the hash of the first block
  print hexlify(hashes.pop())

  # Write out the file with the hashes inserted between each block
  input_file.seek(0)
  while(len(hashes) > 0):
    b = input_file.read(BLOCK_SIZE)
    output_file.write(b)
    output_file.write(hashes.pop())

  # last block doesn't have a hash
  output_file.write(input_file.read())
  output_file.close()
  input_file.close()
