#include "utils.h"

#include <cstdlib>
#include <cryptopp/hex.h>

namespace block_cipher {

using CryptoPP::byte;

byte_array hex2bytes(const string& in) {
  byte_array result;

  CryptoPP::StringSource ss(in, true,
    new CryptoPP::HexDecoder(
      new CryptoPP::StringSinkTemplate<byte_array>(result)));

  return result;
}

string bytes2hex(const byte_array& in) {
  string result;

  CryptoPP::StringSource ss(in.data(), in.size(), true,
    new CryptoPP::HexEncoder(
      new CryptoPP::StringSink(result), false));

  return result;
}

void xor_blocks(unsigned char* out, const unsigned char* b1,
  const unsigned char* b2, size_t len) {
  for (int i = 0; i < len; ++i) {
    out[i] = b1[i] ^ b2[i];
  }
}

void inc_block(unsigned char* b, size_t len) {
  if (len <= 0) return;
  for (unsigned char* p = b + len - 1; p >= b; --p) {
    if (++(*p) != 0) break;
  }
}

} // namespace block_cipher
