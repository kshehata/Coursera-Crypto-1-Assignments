#include "utils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <utility>

using std::pair;
using std::make_pair;
using std::string;
using std::tuple;
using ::testing::get;
using ::testing::TestWithParam;
using ::testing::Values;

namespace block_cipher {
namespace testing {

class HexConversion : public TestWithParam<pair<string, byte_array>> {
 public:
  ~HexConversion() override {}
  void SetUp() override {}
  void TearDown() override {}

 protected:
};

TEST_P(HexConversion, Hex2Bytes) {
  const auto& in = GetParam().first;
  const auto& exp = GetParam().second;
  auto result = hex2bytes(in);
  EXPECT_EQ(result, exp);
}

TEST_P(HexConversion, Bytes2Hex) {
  const byte_array& in = GetParam().second;
  const auto& exp = GetParam().first;
  auto result = bytes2hex(in);
  EXPECT_EQ(result, exp);
}

INSTANTIATE_TEST_SUITE_P(HexConversionExamples,
                         HexConversion,
                         Values(
                          make_pair("", byte_array{}),
                          make_pair("00", byte_array{0x00}),
                          make_pair("deadbeef",
                            byte_array{0xde, 0xad, 0xbe, 0xef}),
                          make_pair(
                            "aa55aa55deadbeef0011223344556677aa55aa55deadbeef0011223344556677",
                            byte_array{0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77, 0xaa, 0x55, 0xaa, 0x55, 0xde, 0xad,
                              0xbe, 0xef, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77})
                          ));



class XorBlocks
    : public TestWithParam<tuple<byte_array, byte_array, byte_array>> {
 public:
  ~XorBlocks() override {}
  void SetUp() override {}
  void TearDown() override {}

 protected:
};

TEST_P(XorBlocks, Examples) {
  const auto& b1 = get<0>(GetParam());
  const auto& b2 = get<1>(GetParam());
  const auto& exp = get<2>(GetParam());
  byte_array result;
  result.resize(b1.size());
  xor_blocks(result.data(), b1.data(), b2.data(), b1.size());
  EXPECT_EQ(result, exp);
}

INSTANTIATE_TEST_SUITE_P(XorBlocksExamples,
                         XorBlocks,
                         Values(
                          make_tuple(byte_array{}, byte_array{}, byte_array{}),
                          make_tuple(byte_array{0x00}, byte_array{0x00},
                            byte_array{0x00}),
                          make_tuple(byte_array{0xde, 0xad, 0xbe, 0xef},
                            byte_array{0xaa, 0x55, 0xaa, 0x55},
                            byte_array{0x74, 0xf8, 0x14, 0xba})
                          ));

class IncBlockTest : public TestWithParam<pair<byte_array, byte_array>> {
 public:
  ~IncBlockTest() override {}
  void SetUp() override {}
  void TearDown() override {}

 protected:
};

TEST_P(IncBlockTest, Examples) {
  auto b = get<0>(GetParam());
  const auto& exp = get<1>(GetParam());
  byte_array result;
  result.resize(b.size());
  inc_block(b.data(), b.size());
  EXPECT_EQ(b, exp);
}

INSTANTIATE_TEST_SUITE_P(IncBlockTestExamples,
                         IncBlockTest,
                         Values(
                          make_pair(byte_array{}, byte_array{}),
                          make_pair(byte_array{0x00}, byte_array{0x01}),
                          make_pair(byte_array{0xff}, byte_array{0x00}),
                          make_pair(byte_array{0xde, 0xad, 0xbe, 0xef},
                            byte_array{0xde, 0xad, 0xbe, 0xf0}),
                          make_pair(byte_array{0xde, 0xad, 0xbe, 0xff},
                            byte_array{0xde, 0xad, 0xbf, 0x00}),
                          make_pair(byte_array{0xef, 0xff, 0xff, 0xff},
                            byte_array{0xf0, 0x00, 0x00, 0x00}),
                          make_pair(byte_array{0xff, 0xff, 0xff, 0xff},
                            byte_array{0x00, 0x00, 0x00, 0x00}),
                          make_pair(byte_array{
                            0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
                            byte_array{
                            0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}),
                          make_pair(byte_array{
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
                            byte_array{
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
                          ));

} // namespace block_cipher
} // namespace testing
