#include "block_cipher.h"

#include "utils.h"
#include <algorithm>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

using CryptoPP::byte;

namespace block_cipher {

const string CBC::NAME = "CBC-AES";
const string CTR::NAME = "CTR-AES";

byte_array BlockCipher::encrypt(const string& m) {
  CryptoPP::AutoSeededRandomPool prng;

  byte_array iv;
  iv.resize(BLOCK_SIZE);
  prng.GenerateBlock(iv.data(), iv.size());

  return encrypt(m, iv.data());
}

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

void CBC::add_pad(unsigned char* b, size_t len) {
  unsigned char p = BLOCK_SIZE - len;
  for (unsigned char* c = b + len; c < b + BLOCK_SIZE; ++c) {
    (*c) = p;
  }
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

byte_array CBC::encrypt(const string& m, const unsigned char* iv) {
  check_key_set();

  byte_array ct(iv, iv + BLOCK_SIZE);
  ct.resize(((m.size() + 1)/BLOCK_SIZE + 2) * BLOCK_SIZE);

  CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e(key_.data(), key_.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    if ((m.size() - i) < BLOCK_SIZE) {
      // need to copy the last block of message to add pad
      std::copy(std::next(m.cbegin(), i), m.cend(),
        std::next(ct.begin(), i + BLOCK_SIZE));
      add_pad(&ct[i + BLOCK_SIZE], m.size() - i);
      xor_blocks(&ct[i + BLOCK_SIZE], &ct[i + BLOCK_SIZE], &ct[i], BLOCK_SIZE);
    } else {
      xor_blocks(&ct[i + BLOCK_SIZE], (unsigned char*)&m[i], &ct[i],
        BLOCK_SIZE);
    }
    e.ProcessString((byte*)&ct[i + BLOCK_SIZE], (byte*)&ct[i + BLOCK_SIZE],
      BLOCK_SIZE);
  }
  return ct;
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
      std::min(BLOCK_SIZE, ct.size() - i - BLOCK_SIZE));
    inc_block(ctr.data(), BLOCK_SIZE);
  }

  return message;
}

byte_array CTR::encrypt(const string& m, const unsigned char* iv) {
  check_key_set();

  byte_array ct(iv, iv + BLOCK_SIZE);
  ct.resize(m.size() + BLOCK_SIZE);
  byte_array ctr(iv, iv + BLOCK_SIZE);
  byte_array t;
  t.resize(BLOCK_SIZE);

  CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e(key_.data(), key_.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    e.ProcessString((byte*)t.data(), (byte*)ctr.data(), BLOCK_SIZE);
    xor_blocks(&ct[i + BLOCK_SIZE], t.data(), (unsigned char*)&m[i],
      std::min(BLOCK_SIZE, ct.size() - i - BLOCK_SIZE));
    inc_block(ctr.data(), BLOCK_SIZE);
  }

  return ct;
}

}