#ifndef BLOCKS_UTILS_H
#define BLOCKS_UTILS_H

#include <string>
#include <vector>

using std::string;

typedef std::vector<unsigned char> byte_array;

constexpr int BLOCK_SIZE = 16;

byte_array hex2bytes(const string& in);

string bytes2hex(const byte_array& in);

void xor_blocks(unsigned char* out, const unsigned char* b1,
  const unsigned char* b2, size_t len);

#endif  // BLOCKS_UTILS_H
