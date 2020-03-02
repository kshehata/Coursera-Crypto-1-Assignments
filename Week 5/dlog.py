#! /usr/bin/python
# Exercises from Week 5 of Coursera's Crypto course
# Copyright 2020 Kareem Shehata
#
# Uses a meet-in-the-middle strategy to calculate the discrete log of a number
# base another number mod p. In other words, if g^x = h (mod p), then given the
# numbers h, g, and p, this routine will calculate x.

import sys
import gmpy2
from gmpy2 import mpz

def dlog(g, h, p, n=40):
  B = mpz(2) ** (n / 2)
  left_half_table = {}
  # l = h / g**x_1
  l = h
  inv_g = gmpy2.invert(g, p)
  if inv_g == 0:
    print "Cannot invert %s mod %s" % (str(g), str(p))
    return 0

  for x in range(0, B + 1): 
    left_half_table[l] = x
    l = (l * inv_g) % p

  # print left_half_table

  base = gmpy2.powmod(g, B, p)
  r = base
  for x in range(0, B + 1):
    # print "Looking for value %s (%d) in hash table" % (str(r), x)
    if left_half_table.has_key(r):
      return (x + 1) * B + left_half_table[r]
    r = gmpy2.t_mod(r*base, p)

  return 0

def dlog_with_check(g, h, p, n=40):
  # make sure we have mpz objects
  g = mpz(g)
  h = mpz(h)
  p = mpz(p)

  x = dlog(g, h, p, n)

  h2 = gmpy2.powmod(g, x, p)
  if h != h2:
    print "ERROR: DLOG found the wrong h!"
    print "%s ** %s (mod %s) should be %s got %s" % (
      str(g), str(x), str(p), str(h), str(h2))
    return 0

  return x

def check_dlog(p, g, x, n=40):
  # make sure everything is mpz already
  p = mpz(p)
  g = mpz(g)
  x = mpz(x)
  n = mpz(n)

  # figure out what operand should be
  h = gmpy2.powmod(g, x, p)

  # test dlog itself
  result = dlog(g, h, p, n)

  h2 = gmpy2.powmod(g, result, p)

  # print output
  if h != h2:
    print "ERROR: DLOG of %s base %s mod %s should be %s but got %s (%s)" % (
      str(h), str(g), str(p), str(x), str(result), str(h2))
    return False
  else:
    print "DLOG of %s base %s mod %s = %s" % (
      str(h), str(g), str(p), str(result))

def run_tests():
  check_dlog(54323, 12345, 1234, 12)
  check_dlog(777781, 555557, 444444, 20)
  check_dlog(
    '1809251394333065553493296640760748560207343510400633813116524750123642650649',
    '1153678116233732248745990358966319550630377323767515160209467723675444630318',
    653936872533)
  check_dlog(
    '67039039649712985497870124991029230637396829102961966888617807218608820150'
    + '367734884009371490834517138450159290932430254268769414059732849732168245'
    + '03042159',
    '20361504138786840196959404340122724172133245264983173112152469958214205976'
    + '432197956793699063774801435329909525308842444937248346129392079903054222'
    + '30770085',
    1073489355871)


if __name__ == "__main__":
  if len(sys.argv) == 2 and sys.argv[1] == "test":
    run_tests()
  if len(sys.argv) == 2 and sys.argv[1] == "example":
    g = '11717829880366207009516117596335367088558084999998952205' \
      + '59997945906392949973658374667057217647146031292859482967' \
      + '5428279466566527115212748467589894601965568'

    h = '323947510405045044356526437872806578864909752095244' \
      + '952783479245297198197614329255807385693795855318053' \
      + '2878928001494706097394108577585732452307673444020333'

    p = '134078079299425970995740249982058461274793658205923933' \
      + '77723561443721764030073546976801874298166903427690031' \
      + '858186486050853753882811946569946433649006084171'

    x = dlog_with_check(g, h, p)
    if x == 0:
      print "Could not find DLOG"
      exit(2)
    else:
      print x
      exit(0)

  elif len(sys.argv) == 4:
    x = dlog_with_check(*sys.argv[1:])
    if x == 0:
      print "Could not find DLOG"
      exit(2)
    else:
      print x
      exit(0)
  else:
    print "Usage: %s [g] [h] [p]" % sys.argv[0]
    print "Calculates x that solve h = g**x mod p"
    exit(1)
