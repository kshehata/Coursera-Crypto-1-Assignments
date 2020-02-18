#include "blocks.h"

#include <gtest/gtest.h>
#include <utility>

using std::tuple;
using ::testing::TestWithParam;
using ::testing::Values;

namespace testing {


class CheckRemovePadTest : 
    public TestWithParam<tuple<bool, byte_array, byte_array>> {

 protected:
  ~CheckRemovePadTest() override {}
  void SetUp() override {}
  void TearDown() override {}
};

TEST_P(CheckRemovePadTest, Examples) {
  const auto exp_success = get<0>(GetParam());
  auto in = get<1>(GetParam());
  const auto& exp = get<2>(GetParam());
  bool result = check_and_remove_pad(in);
  EXPECT_EQ(result, exp_success);
  if (exp_success) {
    EXPECT_EQ(in, exp);
  }
}

INSTANTIATE_TEST_SUITE_P(CheckRemovePadTestExamples,
                         CheckRemovePadTest,
                         testing::Values(
                          make_tuple(true, byte_array{}, byte_array{}),
                          make_tuple(true,
                            byte_array{
                              0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x01},
                            byte_array{
                              0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA}),
                          make_tuple(true,
                            byte_array{
                              0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
                              0xAA, 0x55, 0xAA, 0x55, 0x04, 0x04, 0x04, 0x04},
                            byte_array{
                              0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
                              0xAA, 0x55, 0xAA, 0x55}),
                          make_tuple(false, byte_array{0x04, 0x04},
                            byte_array{}),
                          make_tuple(false, byte_array{
                            0xAA, 0x55, 0xAA, 0x55, 0x04, 0x04, 0x05, 0x04},
                            byte_array{}),
                          make_tuple(false, byte_array{
                            0xAA, 0x55, 0xAA, 0x55, 0x03, 0x04, 0x04, 0x04},
                            byte_array{})
                          ));

} // namespace testing
