#ifndef BLOCKS_UTILS_H
#define BLOCKS_UTILS_H

#include <string>
#include <vector>

using std::string;

typedef std::vector<unsigned char> byte_array;

constexpr int BLOCK_SIZE = 16;

byte_array hex2bytes(const string& in);

string bytes2hex(const byte_array& in);

void xor_blocks(const byte_array& b1, const byte_array& b2, byte_array& out);

#endif  // BLOCKS_UTILS_H
