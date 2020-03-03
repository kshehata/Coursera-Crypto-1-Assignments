#! /usr/bin/python
# Exercises from Week 6 of Crypto course
# Copyright 2020 Kareem Shehata
#
# Attempts to factor RSA modulus when p and q are close together

import gmpy2
from gmpy2 import mpz

def factor_rsa_modulus(N):
  N = mpz(N)
  A, exact = gmpy2.iroot(N, 2)
  if not exact:
    A = A + 1

  return test_avg(A, N)

def test_avg(A, N):
  x, exact = gmpy2.iroot(A*A - N, 2)
  if exact and gmpy2.is_prime(A - x) and gmpy2.is_prime(A + x):
    p = A - x
    q = A + x
    if (p*q) != N:
      print "Found pair %s, %s but they don't produce N" % (str(p), str(q))
      return None
    return p, q

  return None

def scan_for_factors(N, l=1000):
  N = mpz(N)
  A, exact = gmpy2.iroot(N, 2)
  if not exact:
    A = A + 1

  for i in range(l):
    result = test_avg(A, N)
    if result is not None:
      return result
    A = A + 1

  print "Not found"
  return None

def check_x(A, x):
  p, r = gmpy2.t_divmod(A - x, 6)
  if r != 0: return None
  q, r = gmpy2.t_divmod(A + x, 4)
  if r != 0: return None

  if not gmpy2.is_prime(p) or not gmpy2.is_prime(q):
    return None

  return p, q
  

def factor_rsa_modulus_split(N):
  N = mpz(N)
  A, exact = gmpy2.iroot(24*N, 2)
  if not exact:
    A = A + 1

  x, exact = gmpy2.iroot(A*A - 24*N, 2)
  if not exact:
    print "Exact x not found"
    return None

  # have to check both positive and negative roots
  r = check_x(A, x)
  if r is None:
    r = check_x(A, -x)

  # Neither one worked
  if r is None:
    print "Cannot find p and q that fit"
    return None

  p, q = r
  if (p * q) != N:
      print "Found pair %s, %s but they don't produce N" % (str(p), str(q))
      return None

  return p, q

def decrypt_from_factors(c, e, p, q, N = 0):
  if N == 0:
    N = p*q

  c = mpz(c)

  d = gmpy2.invert(e, (p - 1)*(q - 1))
  m = gmpy2.powmod(c, d, N)

  # check that it as correct
  c2 = gmpy2.powmod(m, e, N)
  if c2 != c:
    print "Decryption failed!"
    return None

  # Undo PKCS with separator 00 (not FF!)
  # Have to reverse string and slice off header applied by gmpy2
  m = gmpy2.to_binary(m)[:1:-1]
  if ord(m[0]) != 2:
    print "Invalid PKCS header found"
    return None
  start = m.find(chr(0))
  if start < 0:
    print "Could not find end of pad"
    return None

  return m[start:]


EXAMPLE_1_N = mpz(
  "17976931348623159077293051907890247336179769789423065727343008115"
+ "77326758055056206869853794492129829595855013875371640157101398586"
+ "47833778606925583497541085196591615128057575940752635007475935288"
+ "71082364994994077189561705436114947486504671101510156394068052754"
+ "0071584560878577663743040086340742855278549092581")

EXAMPLE_2_N = mpz(
  "6484558428080716696628242653467722787263437207069762630604390703787"
+ "9730861808111646271401527606141756919558732184025452065542490671989"
+ "2428844841839353281972988531310511738648965962582821502504990264452"
+ "1008852816733037111422964210278402893076574586452336833570778346897"
+ "15838646088239640236866252211790085787877")

EXAMPLE_3_N = mpz(
  "72006226374735042527956443552558373833808445147399984182665305798191"
+ "63556901883377904234086641876639384851752649940178970835240791356868"
+ "77441155132015188279331812309091996246361896836573643119174094961348"
+ "52463970788523879939683923036467667022162701835329944324119217381272"
+ "9276147530748597302192751375739387929")

EXAMPLE_4_E = 65537

EXAMPLE_4_CIPHERTEXT = (
  "2209645186741038177630656113488341801741006978789283107173183914367613560012"
+ "0538004282329650473509424343946219751512256465839967942889460764542040581564"
+ "7489880137348641204523252293201764879166664029975091887299716905260832220677"
+ "7160001932926087000957999372407745896777369781757126722995114866295962793479"
+ "1540")


if __name__ == "__main__":
  p, q = factor_rsa_modulus(EXAMPLE_1_N)
  print "Example 1 result is:"
  print "p = " + str(p)
  print "q = " + str(q)
  print

  p2, q2 = scan_for_factors(EXAMPLE_2_N, 2**20)
  print "Example 2 result is:"
  print "p = " + str(p2)
  print "q = " + str(q2)
  print

  p3, q3 = factor_rsa_modulus_split(EXAMPLE_3_N)
  print "Example 3 result is:"
  print "p = " + str(p3)
  print "q = " + str(q3)
  print

  m = decrypt_from_factors(EXAMPLE_4_CIPHERTEXT, EXAMPLE_4_E, p, q)
  print "Example 4 message is: " + m
