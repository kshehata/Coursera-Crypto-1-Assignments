#include "factor_rsa.h"

#include <unordered_map>

#include <iostream>

using std::string;
using std::cout;
using std::endl;

mpz_class exp_mod_n(const mpz_class& g, const mpz_class& x,
  const mpz_class& p) {

  mpz_class result;
  mpz_powm_sec(result.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());

  return result;
}

bool iroot(const mpz_class& n, unsigned long int i, mpz_class& result) {
  return mpz_root(result.get_mpz_t(), n.get_mpz_t(), i);
}

bool test_answer(const mpz_class& N, const mpz_class& A,
  mpz_class& p, mpz_class& q) {

  mpz_class x;
  bool exact = iroot(A*A - N, 2, x);
  p = A - x;
  q = A + x;

  return exact && mpz_probab_prime_p(p.get_mpz_t(), 0)
      && mpz_probab_prime_p(q.get_mpz_t(), 0) && (N == p*q);  
}

bool factor_rsa_modulus(const mpz_class& N, mpz_class& p, mpz_class& q) {
  mpz_class A;
  if (!iroot(N, 2, A)) {
    A += 1;
  }
  return test_answer(N, A, p, q);
}

bool scan_for_factor(const mpz_class& N, unsigned long int limit,
  mpz_class& p, mpz_class& q) {

  mpz_class A;
  if (!iroot(N, 2, A)) {
    A += 1;
  }

  for (unsigned long int i = 0; i < limit; ++i) {
    if (test_answer(N, A++, p, q)) return true;
  }

  return false;
}

bool check_x(const mpz_class& N, const mpz_class& A, const mpz_class& x,
  const mpz_class& p_factor, const mpz_class& q_factor,
  mpz_class& p, mpz_class& q) {

  mpz_class r;
  mpz_class t = A - x;
  mpz_tdiv_qr(p.get_mpz_t(), r.get_mpz_t(), t.get_mpz_t(),
    p_factor.get_mpz_t());
  if (r != 0) return false;

  t = A + x;
  mpz_tdiv_qr(q.get_mpz_t(), r.get_mpz_t(), t.get_mpz_t(),
    q_factor.get_mpz_t());
  if (r != 0) return false;

  return mpz_probab_prime_p(p.get_mpz_t(), 0)
      && mpz_probab_prime_p(q.get_mpz_t(), 0) && (N == p*q); ;
}

bool factor_rsa_split(const mpz_class& N, int p_factor, int q_factor,
  mpz_class& p, mpz_class& q) {

  mpz_class A;
  if (!iroot(N * (p_factor * q_factor), 2, A)) {
    A += 1;
  }

  mpz_class x;
  if (!iroot(A*A - (p_factor * q_factor) * N, 2, x)) {
    return false;
  }
  if (check_x(N, A, x, p_factor, q_factor, p, q)) return true;
  return check_x(N, A, -x, p_factor, q_factor, p, q);
}

string decrypt_from_factors(const mpz_class& c, const mpz_class& e, 
  const mpz_class& p, const mpz_class& q) {

  const mpz_class N = p*q;
  const mpz_class phi = (p - 1) * (q - 1);

  mpz_class d;
  if (!mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t())) {
    return "fail";
  }

  mpz_class m = exp_mod_n(c, d, N);

  // check this is correct
  mpz_class c2 = exp_mod_n(m, e, N);
  if (c != c2) {
    return "Encryptions don't match";
  }

  string result;
  result.resize(1024);
  size_t count;
  mpz_export((void*)result.data(), &count, 1, 1, 1, 0, m.get_mpz_t());
  result.resize(count);
  auto i = result.find('\0');
  if (i == string::npos) return "PKCS Invalid";
  return result.substr(i + 1);
}
