#include <gtest/gtest.h>

#include "lib.h"

class TestSerialization : public ::testing::Test {
public:
    TestSerialization() { /* init protected members here */ }
    ~TestSerialization() { /* free protected members here */ }
    void SetUp() { /* called before every test */ }
    void TearDown() { /* called after every test */ }

protected:
    /* none yet */
};

TEST_F(TestSerialization, DateJson) {
    EXPECT_TRUE(version() > 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}