#include "utils.h"

#include <cstdlib>
#include <cryptopp/hex.h>

using CryptoPP::byte;

byte_array hex2bytes(const string& in) {
  byte_array result;
  CryptoPP::HexDecoder decoder;
  decoder.Put( (byte*)in.data(), in.size() );
  decoder.MessageEnd();

  CryptoPP::word64 size = decoder.MaxRetrievable();
  if(size && size <= SIZE_MAX) {
    result.resize(size);   
    decoder.Get((byte*)&result[0], result.size());
  }
  return result;
}

string bytes2hex(const byte_array& in) {
  string result;
  CryptoPP::HexEncoder encoder(NULL, false);
  encoder.Put(in.data(), in.size());
  encoder.MessageEnd();

  CryptoPP::word64 size = encoder.MaxRetrievable();
  if(size) {
    result.resize(size);
    encoder.Get((byte*)&result[0], result.size());
  }
  return result;
}

void xor_blocks(unsigned char* out, const unsigned char* b1,
  const unsigned char* b2, size_t len) {
  for (int i = 0; i < len; ++i) {
    out[i] = b1[i] ^ b2[i];
  }
}
