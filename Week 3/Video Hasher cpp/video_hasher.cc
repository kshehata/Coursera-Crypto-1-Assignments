#include "video_hasher.h"

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace video_hasher {

using std::vector;
using CryptoPP::byte;

string hex2bytes(const string& in) {
  string result;

  CryptoPP::StringSource ss(in, true,
    new CryptoPP::HexDecoder(
      new CryptoPP::StringSink(result)));

  return result;
}

string bytes2hex(const string& in) {
  string result;

  CryptoPP::StringSource ss((byte*)in.data(), in.size(), true,
    new CryptoPP::HexEncoder(
      new CryptoPP::StringSink(result), false));

  return result;
}

string hash_block(const string& input, const size_t len,
  const string& last_hash) {

  CryptoPP::SHA256 sha;
  sha.Update((byte*)input.data(), len);
  if (last_hash.size() > 0) {
    sha.Update((byte*)last_hash.data(), last_hash.size());
  }

  string output;
  output.resize(CryptoPP::SHA256::DIGESTSIZE);
  sha.Final((byte*)output.data());
  return output;
}

string hash_file(istream& input_file, ostream& output_file) {
  string buffer;
  buffer.resize(BLOCK_SIZE);
  vector<string> hashes;
  string last_hash;

  // move to the end and work out way back by blocks
  input_file.seekg(0, std::ios_base::end);
  auto end = input_file.tellg();
  hashes.reserve(end / BLOCK_SIZE);
  auto size = end % BLOCK_SIZE;

  while (input_file.tellg() > 0) {
    input_file.seekg(-size, std::ios_base::cur);
    input_file.read((char*)buffer.data(), size);
    // TODO: should check that file read was successful
    last_hash = hash_block(buffer, size, last_hash);
    input_file.seekg(-size, std::ios_base::cur);

    // keep track of hashes to play back later
    hashes.push_back(last_hash);
    size = BLOCK_SIZE;
  }

  // remove the first hash, already have it in last_hash
  hashes.pop_back();
  
  // replay the input file with hashes
  input_file.seekg(0, std::ios_base::beg);
  while(!input_file.eof() && hashes.size() > 0) {
    input_file.read((char*)buffer.data(), BLOCK_SIZE);
    output_file.write(buffer.data(), BLOCK_SIZE);

    // write out the hash
    string h = hashes.back();
    output_file.write(h.data(), h.size());
    hashes.pop_back();
  }

  // output the last block without a hash
  size = end - input_file.tellg();
  input_file.read((char*)buffer.data(), size);
  output_file.write(buffer.data(), size);

  return last_hash;
}

bool verify_hashes(istream& input_file, string last_hash) {
  input_file.seekg(0, std::ios_base::end);
  const auto end = input_file.tellg();
  input_file.seekg(0, std::ios_base::beg);
  const auto beg = input_file.tellg();
  const auto len = end - beg;

  string block_buffer;
  block_buffer.resize(BLOCK_SIZE);
  string hash;
  hash.resize(CryptoPP::SHA256::DIGESTSIZE);
  bool ok = true;

  while(!input_file.eof()) {
    auto remain = end - input_file.tellg();
    if (remain < BLOCK_SIZE + CryptoPP::SHA256::DIGESTSIZE) {
      input_file.read((char*)block_buffer.data(), remain);
      string received_hash = hash_block(block_buffer, remain, "");
      ok = ok && !last_hash.compare(received_hash);
      break;
    }

    input_file.read((char*)block_buffer.data(), block_buffer.size());
    input_file.read((char*)hash.data(), hash.size());
    string received_hash = hash_block(block_buffer, block_buffer.size(), hash);
    ok = ok && !last_hash.compare(received_hash);
    last_hash = hash;
  }
  return ok;
}

} // namespace video_hasher
