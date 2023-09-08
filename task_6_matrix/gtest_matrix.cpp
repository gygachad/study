// task_6_matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <gtest/gtest.h>

#include "matrix.h"

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

TEST_F(TestSerialization, matrix_0)
{
    matrix<int, 2, -1> mt0;
        
    for (auto c : mt0)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    EXPECT_TRUE(mt0.size() == 0);
    auto a = mt0[0][0];
    EXPECT_TRUE(a == -1);
    EXPECT_TRUE(mt0.size() == 0);
    mt0[100][100] = 314;
    mt0[100][101] = 315;
    mt0[101][101] = 316;
    EXPECT_TRUE(mt0[100][100] == 314);
    EXPECT_TRUE(mt0.size() == 3);
    
    for (auto c : mt0)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    } 
}

TEST_F(TestSerialization, matrix_1)
{
    matrix<int, 1, -1> mt1;
    mt1[6] = 6;
    EXPECT_TRUE(mt1[0] == -1);
    EXPECT_TRUE(mt1[6] == 6);
    EXPECT_TRUE(mt1[0] == -1);
    mt1[5] = 5;
    EXPECT_TRUE(mt1[2] == -1);
    EXPECT_TRUE(mt1[5] == 5);

    mt1[6] = -1;
    EXPECT_TRUE(mt1[6] != 6);
    EXPECT_TRUE(mt1[6] == -1);
    mt1[6] = 6;
    mt1[7] = 7;
    
    for (auto c : mt1)
    {
        int x;
        int v;
        std::tie(x, v) = c;
        std::cout << x << v << std::endl;
    }
}

TEST_F(TestSerialization, matrix_2)
{
    matrix<int, 2, -1> mt2;
    mt2[5][6] = 56;
    EXPECT_TRUE(mt2[0][0] == -1);
    EXPECT_TRUE(mt2[1][0] == -1);
    EXPECT_TRUE(mt2[5][7] == -1);
    EXPECT_TRUE(mt2[5][6] == 56);
    mt2[5][7] = 57;
    EXPECT_TRUE(mt2[5][6] == 56);
    EXPECT_TRUE(mt2[6][6] == -1);
    EXPECT_TRUE(mt2[5][7] == 57);
    EXPECT_TRUE(mt2[1][0] == -1);
    mt2[5][6] = -1;
    EXPECT_TRUE(mt2[5][6] == -1);
    EXPECT_TRUE(mt2[5][7] == 57);
    mt2[5][7] = -1;
    EXPECT_TRUE(mt2[5][7] != 57);
    EXPECT_TRUE(mt2[5][7] == -1);
    ((mt2[6][7] = 67) = -1) = 200;
    EXPECT_TRUE(mt2[6][7] == 200);
    EXPECT_TRUE(mt2.size() == 1);
}

TEST_F(TestSerialization, matrix_3)
{
    matrix<int, 3, -1> mt3;
    mt3[5][6][7] = 567;
    EXPECT_TRUE(mt3[5][6][7] == 567);
    EXPECT_TRUE(mt3[6][6][7] == -1);
    EXPECT_TRUE(mt3[5][5][7] == -1);
    EXPECT_TRUE(mt3[5][6][8] == -1);
    mt3[5][6][8] = 568;
    mt3[5][6][9] = 569;
    mt3[6][6][7] = 667;
    mt3[5][7][7] = 577;
    
    for (auto c : mt3)
    {
        int x;
        int y;
        int z;
        int v;
        std::tie(x, y, z, v) = c;
        std::cout << x << y << z << v << std::endl;
    }
    
    EXPECT_TRUE(mt3.size() == 5);
    EXPECT_TRUE(mt3[5][6][8] == 568);
    EXPECT_TRUE(mt3[5][6][9] == 569);
    EXPECT_TRUE(mt3[0][0][0] == -1);
    EXPECT_TRUE(mt3[6][6][7] == 667);
    EXPECT_TRUE(mt3[5][7][7] == 577);
    mt3[5][7][7] = -1;
    EXPECT_TRUE(mt3[5][7][7] == -1);
    mt3[7][6][7] = -1;
    EXPECT_TRUE(mt3[7][6][7] == -1);
    
    mt3[6][6][7] = -1;
    mt3[5][6][7] = -1;
    mt3[5][6][8] = -1;
    mt3[5][6][9] = -1;
    EXPECT_TRUE(mt3.size() == 0);
    EXPECT_TRUE(mt3[5][6][7] == -1);
    EXPECT_TRUE(mt3[5][6][8] == -1);
    EXPECT_TRUE(mt3[5][6][9] == -1);
    EXPECT_TRUE(mt3[5][6][9] == -1);
}

int main(int argc, char** argv) 
{    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}