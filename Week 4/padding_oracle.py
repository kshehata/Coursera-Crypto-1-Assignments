#! /usr/bin/python
# Exercises from Week 4 of Crypto course
# Copyright 2020 Kareem Shehata
#
# Decrypts a given ciphertext by querying a "padding oracle", web services that
# returns a different error code for padding errors than malformed messages.

import functools
import itertools
import urllib2
import random
import sys
from binascii import hexlify, unhexlify
from multiprocessing import Pool, TimeoutError, Lock

TARGET = 'http://crypto-class.appspot.com/po?er='
BLOCK_SIZE = 16

def dprint(s):
  print s
  # print >> sys.stderr, s
  # sys.stderr.flush()


def check_padding(q):
  """ Returns True if padding is good on query, False otherwise"""
  target = TARGET + urllib2.quote(hexlify(q))
  req = urllib2.Request(target)
  try:
    f = urllib2.urlopen(req)
  except urllib2.HTTPError, e:
    # dprint("Padding Oracle returned code", e.code)
    return (e.code == 404)
  #dprint("Actually worked!")
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
  # dprint("Checking guess on block %d byte %d value %d ct %s"
  #   % (block_index, byte_index, v, hexlify(g)))
  return (g, check_padding(c2))

def generate_guesses(suffix):
  return [ chr(i) + suffix for i in range(256) ]

def shuffle_map(f, l):
  random.shuffle(l)
  return map(f, l)

class BlockHunter(object):
  """Use Padding Oracle to hunt for decryption of a block"""

  def __init__(self, ct, block_index):
    super(BlockHunter, self).__init__()
    self.ct = ct
    self.block_index = block_index
    self.m = ""
    self.worker_results = None
    self.pool = None
    self.last_block = (len(ct) / BLOCK_SIZE - 2) == block_index
    self.pad_length = 0
    self.lock = Lock()

  def check_guess_wrapper(self, g):
    return self.pool.apply_async(check_guess, (self.ct, self.block_index, g),
        callback=self.callback)


  def start_hunt(self):
    self.lock.acquire()
    try:
      if self.worker_results is not None:
        dprint("Block %d already hunting!" % self.block_index)
        raise Error("Already hunting!!")

      self.pool = Pool(processes=100)

      if self.last_block and len(self.m) <= 0:
        dprint("Starting hunt for actual pad")
        self.worker_results = shuffle_map(self.check_guess_wrapper,
          [ chr(i) for i in range(1, BLOCK_SIZE + 1) ])
      else:
        dprint("Starting hunt for block %d index %d" % (
          self.block_index, len(self.m)))
        self.worker_results = shuffle_map(self.check_guess_wrapper,
          generate_guesses(self.m))

    finally:
      self.lock.release()

  def collect_results(self):
    self.lock.acquire()
    try:
      dprint("Collecting results for block %d" % self.block_index)
      self.pool.close()
    finally:
      self.lock.release()

    try:
      self.pool.join()
    except TimeoutError:
      dprint(("Timeout collecting result for block %d" % self.block_index))

    dprint("Block %d so far is %s" % (self.block_index, hexlify(self.m)))
    self.pool = None
    self.worker_results = None


  def callback(self, result):
    (guess, correct) = result
    if correct:
      self.lock.acquire()
      if self.last_block and len(self.m) <= 0:
        self.pad_length = ord(guess)
        dprint("Found correct pad %d" % self.pad_length)
        # Can't really check this with oracle, since the pad will cancel out.
        # Just assume it's good and move on
        self.m = chr(self.pad_length) * self.pad_length

      else:
        dprint("Found correct guess block %d: %s" % (self.block_index, self.m))
        self.m = guess

      try:
        self.pool.terminate()
      finally:
        self.lock.release()


  def complete(self):
    return len(self.m) >= BLOCK_SIZE

  def message(self):
    if self.pad_length > 0:
      return self.m[:-self.pad_length]
    else:
      return self.m



def decrypt(ct):
  num_blocks = len(ct) / BLOCK_SIZE - 1
  hunters = [ BlockHunter(ct, b) for b in range(num_blocks)]

  complete = False
  while not complete:
    complete = True
    for h in hunters:
      if not h.complete():
        complete = False
        h.start_hunt()
    for h in hunters:
      if not h.complete():
        h.collect_results()

  return "".join([ h.message() for h in hunters ])

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print "Usage: padding_oracle.py [ciphertext]"
    exit(1)

  ct = unhexlify(sys.argv[1])

  m = decrypt(ct)
  print "Recovered message: " + m
