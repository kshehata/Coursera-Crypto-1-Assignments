#include "video_hasher.h"


#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <utility>
#include <vector>

using CryptoPP::byte;
using std::cout;
using std::endl;
using std::ios;
using std::istringstream;
using std::ostringstream;
using std::pair;
using std::make_pair;
using std::make_unique;
using std::tuple;
using std::unique_ptr;
using std::vector;
using ::testing::get;
using ::testing::TestWithParam;
using ::testing::Values;

namespace video_hasher {

namespace testing {

string build_example_string(const size_t len) {
  string buffer;
  buffer.resize(len);
  for (size_t i = 0; i < len; ++i) {
    buffer[i] = (char)i;
  }
  return buffer;
}

TEST(VideoHasherTest, HashBlock) {
  string sample_data = build_example_string(BLOCK_SIZE);
  EXPECT_EQ(
    hex2bytes(
      "785b0751fc2c53dc14a4ce3d800e69ef9ce1009eb327ccf458afe09c242c26c9"),
    hash_block(sample_data, BLOCK_SIZE, ""));
}

TEST(VideoHasherTest, HashBlockWithPreviousHash) {
  string sample_data = build_example_string(BLOCK_SIZE);

  EXPECT_EQ(
    hex2bytes(
      "81ebb5853b0d69bbd145d6fcc0c6da8eab78764cca0442324169f9c22091d64c"),
    hash_block(sample_data, BLOCK_SIZE, hex2bytes(
      "785b0751fc2c53dc14a4ce3d800e69ef9ce1009eb327ccf458afe09c242c26c9")));
}

void compare_string_chunk(const string& s1, size_t offset, const string& s2) {
  ASSERT_GE(s1.size(), offset + s2.size());
  EXPECT_EQ(s1.substr(offset, s2.size()), s2);
}

// sample data
const vector<const string> sample_block = {
  string(BLOCK_SIZE, 0xAA),
  string(BLOCK_SIZE, 0x55),
  string(137, 0xA5)
};

const vector<const string> expected_hashes = {
  "ea4aa103e2c330062d05d18a265b5cf6ab44758033de2cc72eee1b1b07cba744",
  "5f67207d40403072e3ff1650409da6919cf5f31072a9c328fa47fb4acc46300b",
  "1ded1442ee143bb5b6fec84d1748da1b0a23a4425869e3ccfd7e7120a2d1bb65"
};

TEST(VideoHasherTest, HashFile) {
  string buffer;
  for (const auto& b : sample_block) {
    buffer += b;
  }

  istringstream input_file(buffer);
  ostringstream output_file;
  string first_hash = hash_file(input_file, output_file);
  EXPECT_EQ(hex2bytes(expected_hashes[0]), first_hash);

  size_t p = 0;
  for (int i = 0; i < sample_block.size(); ++i) {
    compare_string_chunk(output_file.str(), p, sample_block[i]);
    p += sample_block[0].size();
    if (i < expected_hashes.size() - 1) {
      auto h = hex2bytes(expected_hashes[i+1]);
      compare_string_chunk(output_file.str(), p, h);
      p += h.size();
    }
  }
}

string sample_data_with_hashes() {
  string buffer;
  for (int i = 0; i < sample_block.size(); ++i) {
    buffer += sample_block[i];
    if (i < expected_hashes.size() - 1) {
      buffer += hex2bytes(expected_hashes[i + 1]);
    }
  }
  return buffer;
}

TEST(VideoHasherTest, VerifyHashes) {
  istringstream input_file(sample_data_with_hashes());
  EXPECT_TRUE(verify_hashes(input_file, hex2bytes(expected_hashes[0])));
}

TEST(VideoHasherTest, VerifyHashesFirstHashWrong) {
  istringstream input_file(sample_data_with_hashes());
  EXPECT_FALSE(verify_hashes(input_file, hex2bytes(
    "ea4aa103e2c330062d05d18a265b5cf6ab44758033de2cc72eee1b1b07cba745")));
}

TEST(VideoHasherTest, VerifyHashesExtraData) {
  istringstream input_file(sample_data_with_hashes() + "#");
  EXPECT_FALSE(verify_hashes(input_file, hex2bytes(expected_hashes[0])));
}

} // namespace testing

} // namespace video_hasher
