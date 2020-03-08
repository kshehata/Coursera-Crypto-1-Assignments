#ifndef VIDEO_HASHER_VIDEO_HASHER_H
#define VIDEO_HASHER_VIDEO_HASHER_H

#include <iostream>
#include <cryptopp/simple.h>

using std::istream;
using std::ostream;
using std::string;


namespace video_hasher {

constexpr size_t BLOCK_SIZE = 1024;

// Utility function to converter of hex numbers to bytes
string hex2bytes(const string& in);
// Utility function to conver a string of bytes to the hex representation
string bytes2hex(const string& in);

// Compute the hash for a block by reading len bytes from a string
string hash_block(const string& input, const size_t len,
  const string& last_hash);

string hash_file(istream& input_file, ostream& output_file);

} // namespace video_hasher

#endif  // VIDEO_HASHER_VIDEO_HASHER_H
