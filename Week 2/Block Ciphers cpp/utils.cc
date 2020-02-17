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

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out) {
  // assume b1 and b2 are the same length. Up to caller to ensure that
  out.resize(b1.size());
  for (int i = 0; i < b1.size(); ++i) {
    out[i] = b1[i] ^ b2[i];
  }
}
