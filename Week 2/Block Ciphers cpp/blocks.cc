#include "blocks.h"


#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>

using CryptoPP::byte;

bool check_and_remove_pad(string& m) {
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

string decrypt_cbc(const byte_array& key, const byte_array& ct) {

  if (key.size() != BLOCK_SIZE) {
    // TDDO: should throw an exception if key is wrong length
    return "";
  }
  if (ct.size() < BLOCK_SIZE) {
    return "";
  }
  if (ct.size() % BLOCK_SIZE != 0) {
    //TODO: should throw an exception if CT is wrong length
    return "";
  }

  string message;
  message.resize(ct.size() - BLOCK_SIZE);
  CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption d(key.data(), key.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    d.ProcessString((byte*)&message[i], (byte*)&ct[i + BLOCK_SIZE], BLOCK_SIZE);
    xor_blocks((unsigned char*)&message[i], &ct[i],
      (unsigned char*)&message[i], BLOCK_SIZE);
  }

  check_and_remove_pad(message);
  return message;
}

string decrypt_ctr(const byte_array& key, byte_array& ct) {
  if (key.size() != BLOCK_SIZE) {
    // TDDO: should throw an exception if key is wrong length
    return "";
  }
  if (ct.size() < BLOCK_SIZE) {
    return "";
  }

  string message;
  message.resize(ct.size() - BLOCK_SIZE);
  CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e(key.data(), key.size());

  for (int i = 0; i < ct.size() - BLOCK_SIZE; i += BLOCK_SIZE) {
    e.ProcessString((byte*)&message[i], (byte*)ct.data(), BLOCK_SIZE);
    xor_blocks((unsigned char*)&message[i], &ct[i + BLOCK_SIZE],
      (unsigned char*)&message[i], ct.size() - i - BLOCK_SIZE);
    inc_block(ct.data(), BLOCK_SIZE);
  }

  return message;
}
