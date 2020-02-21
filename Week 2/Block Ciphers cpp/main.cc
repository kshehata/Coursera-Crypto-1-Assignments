//  Created by Kareem Shehata on 2020-02-17.
//  Copyright © 2020 Kareem Shehata. All rights reserved.

#include "block_cipher.h"
#include "utils.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::make_unique;
using std::string;
using std::unique_ptr;

int main(int argc, const char * argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " [mode]" << endl;
    cout << "Mode can be CBC or CTR" << endl;
    cout << "Key is first input parsed from stdin, with CT read one per line"
      << endl;
    return 1;
  }

  unique_ptr<block_cipher::BlockCipher> cipher;
  if (!string("CBC").compare(argv[1])) {
    cipher = make_unique<block_cipher::CBC>();
  } else if (!string("CTR").compare(argv[1])) {
    cipher = make_unique<block_cipher::CTR>();
  }

  string line;
  getline(cin, line);
  cipher->set_key(hex2bytes(line));

  while(getline(cin, line)) {
    byte_array ct = hex2bytes(line);
    cout << "Key: " << bytes2hex(cipher->get_key()) << endl;
    cout << "CT : " << bytes2hex(ct) << endl;
    cout << "MT : " << cipher->decrypt(ct) << endl;
    cout << endl;
  }

  return 0;
}
