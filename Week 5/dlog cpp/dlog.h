#ifndef DLOG_DLOG_H
#define DLOG_DLOG_H value

#include <gmpxx.h>

const int n = 40;

mpz_class exp_mod_n(const mpz_class& g, const mpz_class& x, const mpz_class& p);

mpz_class dlog(const mpz_class& g, const mpz_class& h, const mpz_class& p,
  const int n = 40);

#endif  // DLOG_DLOG_H