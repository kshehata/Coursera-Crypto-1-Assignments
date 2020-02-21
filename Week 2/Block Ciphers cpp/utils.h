#ifndef BLOCKS_UTILS_H
#define BLOCKS_UTILS_H

#include <string>
#include <vector>


namespace block_cipher {

using std::string;

// Convenience type for representing byte arrays
typedef std::vector<unsigned char> byte_array;

// Parse a string with hex numbers to a byte array
byte_array hex2bytes(const string& in);

// Enconde a byte array as a string of hex numbers
string bytes2hex(const byte_array& in);

// XOR two arrays of bytes. Assumed that b1 and b2 are the same length len.
// Output is stored in out of the same length. Output can be the same as inputs
void xor_blocks(unsigned char* out, const unsigned char* b1,
  const unsigned char* b2, size_t len);

// In-place increment a number stored as a byte array. Assumed to be big endian.
void inc_block(unsigned char* b, size_t len);

} // namespace block_sipher

#endif  // BLOCKS_UTILS_H
