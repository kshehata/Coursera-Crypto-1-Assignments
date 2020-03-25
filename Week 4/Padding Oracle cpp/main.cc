//  Created by Kareem Shehata on 2020-02-17.
//  Copyright © 2020 Kareem Shehata. All rights reserved.

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::make_unique;
using std::string;
using std::unique_ptr;

constexpr int BLOCK_SIZE = 16;
constexpr char URL_PREFIX[] = "http://crypto-class.appspot.com/po?er=";
constexpr char GUESS_ORDER[] = " etaonhisrdlu\nwmycgf,bp.vk\"I\'-T;HMWA_SB?x!jEzCqLDYJNO:PRGFKVUXQ)(0*128453679Z&][$/+@#%<=>\\^`{|}~";

CURL *curl;

int char2int(char input) {
  if (input >= '0' && input <= '9') return input - '0';
  if (input >= 'A' && input <= 'F') return input - 'A' + 10;
  if (input >= 'a' && input <= 'f') return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
string hex2bin(string src) {
  if (src.size() % 2 != 0) {
    throw std::invalid_argument("hex string must have even length");
  }

  string result;
  result.reserve(src.size() / 2);
  for (int i = 0; i < src.size(); i += 2) {
    result.push_back(char2int(src[i]) * 16 + char2int(src[i + 1]));
  }
  return result;
}

string bin2hex(string src) {
  std::ostringstream ss;
  ss << std::hex;

  unsigned char* d = (unsigned char*)src.data();
  for (int i = 0; i < src.size(); ++i) {
    ss << std::setw(2) << std::setfill('0') << (int)d[i];
  }
  return ss.str();
}

long try_url(string q) {
  string url = string(URL_PREFIX) + bin2hex(q);
  // cout << "Trying URL: " << url << " ";
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    cout << "CURL Failed: " << curl_easy_strerror(res) << endl;
    return -1;
  }
  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  // cout << "(" << response_code << ")" << endl;
  return response_code;
}

void xor_guess(string& block, const string& guess, int len) {
  for (int i = 1; i <= len; ++i) {
    block[block.size() - i - BLOCK_SIZE] ^= guess[guess.size() - i] ^ len;
  }
}

bool check_guess(string block, const string& guess, int len) {
  xor_guess(block, guess, len);
  return 404 == try_url(block);
}

string decode_block(const string& block, bool last_block = false) {
  string g;
  g.resize(BLOCK_SIZE);
  int pad = 0;

  if (last_block) {
    for (int i = 1; i <= BLOCK_SIZE; ++i) {
      g[BLOCK_SIZE - 1] = i;
      if (check_guess(block, g, 1)) {
        pad = i;
        break;
      }
    }
    cout << "Pad is length " << pad << endl;
    for (int i = 0; i < pad; ++i) {
      g[BLOCK_SIZE - i - 1] = pad;
    }
  }

  for (int l = pad + 1; l <= BLOCK_SIZE; ++l) {
    bool success = false;
    for (auto c : GUESS_ORDER) {
      g[BLOCK_SIZE - l] = c;
      if (check_guess(block, g, l)) {
        success = true;
        cout << "Found char " << c << " (" << (int)c << ")" << endl;
        break;
      }
    }
    if (!success) throw std::invalid_argument("Could not find char!");
  }
  if (pad > 0) {
    // remove the pad
    return g.substr(0, g.size() - pad);
  } else {
    return g;
  }
}

string decode(const string& ciphertext) {
  string m;
  for (int b = 0; b < ciphertext.size() / BLOCK_SIZE - 2; ++b) {
    cout << "Decoding block " << b << endl;
    m = m + decode_block(ciphertext.substr(0, (b + 2) * BLOCK_SIZE));
    cout << "Message so far: " << m << endl;
  }
  cout << "Decoding last block" << endl;
  m = m + decode_block(ciphertext, true);
  return m;
}

void usage(const string& name) {
  cout << "Usage: " << name << " [CT]" << endl;
}

int main(int argc, const char * argv[]) {

  if (argc != 2) {
    usage(argv[0]);
    return 1;
  }
 
  cout << "Hello World!" << endl;
  curl = curl_easy_init();
  if (!curl) {
    cout << "Could not initialize cURL" << endl;
    return 2;
  }

  auto m = decode(hex2bin(argv[1]));
  cout << "Result: " << m << endl;
  // if (c == 0) {
  //   cout << "Couldn't find char" << endl;
  // } else {
  //   cout << "Found character " << c << " (" << (int)c << ")" << endl;
  // }

  curl_easy_cleanup(curl);

  cout << "All done!" << endl;
  return 0;
}