#ifndef BLOCKS_BLOCK_CIPHER_H
#define BLOCKS_BLOCK_CIPHER_H

#include "utils.h"
#include <cryptopp/simple.h>

namespace block_cipher {

constexpr int BLOCK_SIZE = 16;

// Generic base class for all block ciphers. Assumes AES is used under CBC or
// CTR mode, thus block size fixed to 16-bytes and 16-byte key expected.
class BlockCipher {
public:
  BlockCipher(){}
  virtual ~BlockCipher(){}

  // Set key to use for encryption or decryption operations. Must be exactly
  // BLOCK_SIZE in length, otherwise InvalidKeyLength is thrown.
  virtual void set_key(const byte_array& key) {
    if (key.size() != BLOCK_SIZE) {
      throw CryptoPP::InvalidKeyLength(name(), key.size());
    }
    key_ = key;
  }

  // Get key currently in use
  virtual const byte_array& get_key() const {
    return key_;
  }

  // Name of this block cipher
  virtual const string& name() const = 0;

  // Decrypt a ciphertext to message. Implemented by concrete classes.
  virtual string decrypt(const byte_array& ct) = 0;

  // Encrypt a message to ciphertext. Implemented by concrete classes.
  virtual byte_array encrypt(const byte_array& m) = 0;

protected:
  // key to use for encryption
  byte_array key_;

  // helper to check if key is set correctly and throw InvalidArgument if not.
  void check_key_set() {
    if (key_.size() != BLOCK_SIZE) {
      throw CryptoPP::InvalidArgument("Key not set");
    }
  }
};

// Concrete class implementation of CBC-AES BlockCipher
class CBC : public BlockCipher {
public:
  CBC(){}
  ~CBC(){}

  static const string NAME;
  
  const string& name() const override { return NAME; }

  string decrypt(const byte_array& ct) override;

  byte_array encrypt(const byte_array& m) override;

  // Utility function to remove block pad. Returns true if pad is valid,
  // false otherwise. If pad is invalid, string is not modified.
  static bool check_and_remove_pad(string& m);
};

// Concrete class implementation of CTR-AES BlockCipher
class CTR : public BlockCipher {
public:
  CTR(){}
  ~CTR(){}
  
  static const string NAME;
  
  const string& name() const override { return NAME; }

  string decrypt(const byte_array& ct) override;

  byte_array encrypt(const byte_array& m) override;
};

} // namespace block_cipher

#endif  // BLOCKS_BLOCK_CIPHER_H
