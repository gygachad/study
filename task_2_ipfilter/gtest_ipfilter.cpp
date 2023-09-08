#include <string>
#include <gtest/gtest.h>

#include "gtest_ipfilter.h"
#include "ip_pool.h"

#include "version.h"
#include "str_tool.h"

using namespace std;

class TestSerialization : public ::testing::Test {
public:
    TestSerialization() { /* init protected members here */ }
    ~TestSerialization() { /* free protected members here */ }
    void SetUp() { /* called before every test */ }
    void TearDown() { /* called after every test */ }

protected:
    /* none yet */
};

TEST_F(TestSerialization, MainTest)
{
    EXPECT_TRUE(version() > 0);

    ip_addr addr;

    //EXPECT_FALSE(addr.parse("1aaa.2.3.4"));
    EXPECT_FALSE(addr.parse("1.2."));
    EXPECT_FALSE(addr.parse("1.2.3.4."));

    string test_mask = "1.*.*.4";

    addr.parse("1.2.3.4");
    EXPECT_TRUE(addr.match(test_mask));

    addr.parse("2.2.3.4");
    EXPECT_FALSE(addr.match(test_mask));

    ip_addr a;
    ip_addr b;

    a.parse("2.2.3.4");
    b.parse("1.2.3.4");
    EXPECT_GT(a, b);
    EXPECT_NE(a, b);

    a.parse("10.2.3.4");
    b.parse("100.2.3.4");
    EXPECT_LT(a, b);
    EXPECT_NE(a, b);

    a.parse("10.2.3.4");
    b.parse("10.2.3.4");
    EXPECT_LE(a, b);
    EXPECT_EQ(a, b);

    ip_pool pool = ip_pool();

    vector<string> not_filtered = str_tool::split(string(NOT_FILTERED), '\n');

    for(auto& line : not_filtered)
    {
        vector<string> v = str_tool::split(line, '\t');

        ip_addr ip = ip_addr();

        if (ip.parse(v.at(0)))
            pool.push_back(ip);
    }

    pool.reverse_sort();

    ip_pool filtered_by_first;
    filtered_by_first.push_back(pool.filter("1.*.*.*"));

    ip_pool filtered_by_first_and_second;
    filtered_by_first_and_second.push_back(pool.filter("46.70.*.*"));

    ip_pool filtered_by_any;
    filtered_by_any.push_back(pool.filter_any("46"));

    string filtered = "";

    filtered += pool.to_string();
    filtered += filtered_by_first.to_string();
    filtered += filtered_by_first_and_second.to_string();
    filtered += filtered_by_any.to_string();

    filtered.erase(remove(filtered.begin(), filtered.end(), '\r'), filtered.end());
    filtered.erase(remove(filtered.begin(), filtered.end(), '\n'), filtered.end());

    string filtered_check(FILTERED);

    EXPECT_TRUE(filtered_check == filtered);
}

int main(int argc, char** argv) 
{    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}