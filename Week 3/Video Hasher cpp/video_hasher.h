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

// Hash each block of the intput file, writing the blocks and hashes to output.
// Returns the hash of the first block.
string hash_file(istream& input_file, ostream& output_file);

// Verify the hashes in the input file and write the file without hashes to
// output. Returns true if all hashes correct.
bool verify_hashes(istream& input_file, string first_hash);

} // namespace video_hasher

#endif  // VIDEO_HASHER_VIDEO_HASHER_H
