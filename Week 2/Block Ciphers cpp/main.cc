//  Created by Kareem Shehata on 2020-02-17.
//  Copyright © 2020 Kareem Shehata. All rights reserved.

#include "utils.h"

#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <iostream>
#include <string>

using std::string;

string decrypt(byte_array key, byte_array ct) {
  string message;
  CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d(key.data(), key.size(), ct.data());

  CryptoPP::StringSource ss( &ct[16], ct.size() - 16, true, 
      new CryptoPP::StreamTransformationFilter( d,
          new CryptoPP::StringSink( message )
      ) // StreamTransformationFilter
  ); // StringSource

  return message;
}

int main(int argc, const char * argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " [key] [ciphertext]" << std::endl;
    return 1;
  }

  byte_array key = hex2bytes(argv[1]);
  byte_array ct = hex2bytes(argv[2]);

  std::cout << "Key:  " << bytes2hex(key) << std::endl;
  std::cout << "CT:  " << bytes2hex(ct) << std::endl;

  auto message = decrypt(key, ct);
  std::cout << "Message: " << message;

  return 0;
}
