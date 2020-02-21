//  Created by Kareem Shehata on 2020-02-17.
//  Copyright © 2020 Kareem Shehata. All rights reserved.
//
//  Example usage of block ciphers on command line.
//  Gets mode as a command line argument, but then reads first line as key in
//  hex format and then each line as ciphertext.
//
//  *** VERY IMPORTANT ***
//  NEVER read keys or sensitive values on the command line!!
//  Command line parameters can be read by other processes on your system and
//  could leak your keys to another process or user!
//  That's why this example reads the key from standard input.

#include "block_cipher.h"
#include "utils.h"

#include <iostream>
#include <string>

using namespace block_cipher;
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
    cout << "Key is first line parsed from stdin, with CT read one per line"
      << endl;
    return 1;
  }

  unique_ptr<BlockCipher> cipher;
  if (!string("CBC").compare(argv[1])) {
    cipher = make_unique<CBC>();
  } else if (!string("CTR").compare(argv[1])) {
    cipher = make_unique<CTR>();
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
