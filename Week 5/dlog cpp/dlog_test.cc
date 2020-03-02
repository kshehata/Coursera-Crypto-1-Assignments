#include "dlog.h"


#include <gmpxx.h>
#include <gtest/gtest.h>
#include <utility>

using std::pair;
using std::make_pair;
using std::make_unique;
using std::string;
using std::tuple;
using std::unique_ptr;
using ::testing::get;
using ::testing::TestWithParam;
using ::testing::Values;

namespace testing {


class DlogTest : 
    public TestWithParam<tuple<mpz_class, mpz_class, mpz_class, mpz_class, int>> {

 protected:
  ~DlogTest() override {}
  void SetUp() override {
    g_ = get<0>(GetParam());
    x_ = get<1>(GetParam());
    p_ = get<2>(GetParam());
    h_ = get<3>(GetParam());
    n_ = get<4>(GetParam());
  }
  void TearDown() override {}

  mpz_class g_;
  mpz_class x_;
  mpz_class p_;
  mpz_class h_;
  int n_;
};

TEST_P(DlogTest, ExpModN) {
  EXPECT_EQ(h_, exp_mod_n(g_, x_, p_));
}

TEST_P(DlogTest, Dlog) {
  EXPECT_EQ(x_, dlog(g_, h_, p_, n_));\
}

INSTANTIATE_TEST_SUITE_P(DlogTestExamples,
                         DlogTest,
                         Values(
                          make_tuple("12345", "1234", "54323", "6640", 12),
                          make_tuple("555557", "133332", "777781", "661555", 20),
                          make_tuple(
  "1153678116233732248745990358966319550630377323767515160209467723675444630318",
  "653936872533",
  "1809251394333065553493296640760748560207343510400633813116524750123642650649",
  "1346717359610222066042143255464844450174724877494852427753510799474619764949",
  40),
                          make_tuple(
  "20361504138786840196959404340122724172133245264983173112152469958214205976"
  "432197956793699063774801435329909525308842444937248346129392079903054222"
  "30770085",
  "1073489355871",
  "67039039649712985497870124991029230637396829102961966888617807218608820150"
  "367734884009371490834517138450159290932430254268769414059732849732168245"
  "03042159",
  "2057587726143882535542688452040204377649691462476959342168008460263139675178"
  "2755612429894197132020967988022176407191283440133977007909937700691038464661"
  "19", 40)
                          ));

} // namespace testing
