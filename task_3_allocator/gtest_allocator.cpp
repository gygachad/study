#include <string>
#include <gtest/gtest.h>
#include "allocator.h"
#include "single_linked_list.h"

#include "version.h"

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

TEST_F(TestSerialization, allocator_test)
{
    EXPECT_TRUE(version() > 0);

    map<int, string, less<int>, linear_allocator<pair<const int, string>>> m;

    for (int i = 0; i < 0x20000; i++)
        m[i] = to_string(i);

    for (int i = 0; i < 0x20000; i++)
        m[i] = "_" + m[i] + "_";

    for (int i = 0; i < 0x20000; i++)
        EXPECT_TRUE(m[i] == "_" + to_string(i) + "_");

    single_linked_container<int> a;
    for (int i = 0; i < 10; i++)
        a.push_back(i);
}

TEST_F(TestSerialization, container_pushback_test)
{
    single_linked_container<int> a;
    for (int i = 0; i < 10; i++)
        a.push_back(i);
}

TEST_F(TestSerialization, container_operator_test)
{
    single_linked_container<int> a;
    for (int i = 0; i < 10; i++)
        a.push_back(i);

    for (int i = 0; i < 10; i++)
        EXPECT_TRUE(a[i] == i);
}

TEST_F(TestSerialization, container_copy_test)
{
    single_linked_container<int> a;
    for (int i = 0; i < 10; i++)
        a.push_back(i);

    single_linked_container<int> b(a);

    for (size_t i = 0; i < 10; i++)
        EXPECT_TRUE(b[i] == a[i]);
}

TEST_F(TestSerialization, container_opeartor_copy_test)
{
    single_linked_container<string, linear_allocator<string>> c;
    for (int i = 0; i < 10; i++)
        c.push_back("string " + to_string(i));

    single_linked_container<string, linear_allocator<string>> d;

    d = c;

    for (int i = 0; i < 10; i++)
        EXPECT_TRUE(d[i] == c[i]);
}

TEST_F(TestSerialization, container_iterator_test)
{
    single_linked_container<string, linear_allocator<string>> c;
    for (int i = 0; i < 10; i++)
        c.push_back("string " + to_string(i));

    for (const auto& m : c)
        cout << m << endl;
}

int main(int argc, char** argv) 
{    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}