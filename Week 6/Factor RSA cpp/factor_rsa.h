#ifndef DLOG_DLOG_H
#define DLOG_DLOG_H value

#include <gmpxx.h>
#include <string>

using std::string;

// Shortcut to calculate g**x mod p.
mpz_class exp_mod_n(const mpz_class& g, const mpz_class& x, const mpz_class& p);

// Calculate the ith root of N putting result in result. Return true if exact.
bool iroot(const mpz_class& n, unsigned long int i, mpz_class& result);

// Factor an RSA modulus assuming p and q are close together. Returns true if
// successful, and p and q are updated to output values. Otherwise, returns
// false and p and q are left unmodified.
bool factor_rsa_modulus(const mpz_class& N, mpz_class& p, mpz_class& q);

// Scan for a larger difference between p and q with limit l added to average.
bool scan_for_factor(const mpz_class& N, unsigned long int limit,
  mpz_class& p, mpz_class& q);

// Factor an RSA modulus assuming a ratio between p and q
bool factor_rsa_split(const mpz_class& N, int p_factor, int q_factor,
  mpz_class& p, mpz_class& q);

// Decrypt a ciphertext using the factors of modulus N and encryption key e
string decrypt_from_factors(const mpz_class& c, const mpz_class& e, 
  const mpz_class& p, const mpz_class& q);

#endif  // DLOG_DLOG_H