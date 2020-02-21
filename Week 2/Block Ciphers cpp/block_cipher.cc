#include "block_cipher.h"

#include "utils.h"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>

using CryptoPP::byte;

namespace block_cipher
{

const string CBC::NAME = "CBC-AES";
const string CTR::NAME = "CTR-AES";

bool CBC::check_and_remove_pad(string& m) {
  if (m.size() <= 0) {
    return true;
  }

  int pad_len = m.back();
  if (pad_len > m.size()) {
    return false;
  }

  for (int i = m.size() - 1; i >= m.size() - pad_len; --i) {
    if (m[i] != pad_len) {
      return false;
    }
  }

  m.resize(m.size() - pad_len);
  return true;
}

string CBC::decrypt(const byte_array& ct) {
  check_key_set();

  if (ct.size() % BLOCK_SIZE != 0) {
    throw CryptoPP::InvalidArgument(
      "Ciphertext must be multiple of block size");
  }

  string message;
  message.resize(ct.size() - BLOCK_SIZE);
  CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption d(key_.data(), key_.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    d.ProcessString((byte*)&message[i], (byte*)&ct[i + BLOCK_SIZE], BLOCK_SIZE);
    xor_blocks((unsigned char*)&message[i], &ct[i],
      (unsigned char*)&message[i], BLOCK_SIZE);
  }

  check_and_remove_pad(message);
  return message;
}

byte_array CBC::encrypt(const byte_array& m) {
  return {};
}

string CTR::decrypt(const byte_array& ct) {
  check_key_set();

  string message;
  message.resize(ct.size() - BLOCK_SIZE);

  byte_array ctr(ct.begin(), std::next(ct.begin(), BLOCK_SIZE));
  byte_array t;
  t.resize(BLOCK_SIZE);
  CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e(key_.data(), key_.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    e.ProcessString((byte*)t.data(), (byte*)ctr.data(), BLOCK_SIZE);
    xor_blocks((unsigned char*)&message[i], t.data(), &ct[i + BLOCK_SIZE],
      ct.size() - i - BLOCK_SIZE);
    inc_block(ctr.data(), BLOCK_SIZE);
  }

  return message;
}

byte_array CTR::encrypt(const byte_array& m) {
  return {};
}

}