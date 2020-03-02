#include "dlog.h"

#include <unordered_map>

#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace std {
template <> struct hash<mpz_class> {
  size_t operator()(const mpz_class& x) const {
    return (x.get_mpz_t()[0]._mp_size == 0) ? 0 :
      static_cast<size_t>(x.get_mpz_t()[0]._mp_d[0]);
  }
};
}

mpz_class exp_mod_n(const mpz_class& g, const mpz_class& x,
  const mpz_class& p) {

  mpz_class result;
  mpz_powm_sec(result.get_mpz_t(), g.get_mpz_t(), x.get_mpz_t(), p.get_mpz_t());

  return result;
}

mpz_class dlog(const mpz_class& g, const mpz_class& h, const mpz_class& p,
  const int n) {

  const long B = 1 << (n/2);

  std::unordered_map<mpz_class, long> left_half_table;
  left_half_table.reserve(B);

  mpz_class l = h;
  mpz_class inv_g;
  mpz_invert(inv_g.get_mpz_t(), g.get_mpz_t(), p.get_mpz_t());
  left_half_table[l] = 0;
  for (long x = 1; x <= B; ++x) {
    l = (l * inv_g) % p;
    left_half_table[l] = x;
  }

  // cout << "Hash Map:" << endl;
  // for (auto i : left_half_table) {
  //   cout << i.first << " <- " << i.second << endl;
  // }
  // cout << endl;

  const mpz_class base = exp_mod_n(g, B, p);
  mpz_class r = base;

  for (long x = 0; x <= B; ++x) {
    // cout << "Looking for value " << r.get_str(10) << " (" << x
    //   << ") in hash map" << endl;
    const auto it = left_half_table.find(r);
    if (it != left_half_table.end()) {
      return (x + 1) * B + it->second;
    }
    r = (r * base) % p;
  }

  return 0;
}
