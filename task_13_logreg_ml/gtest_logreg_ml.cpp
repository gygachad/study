#include <string>
#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "logreg_classifier.h"

class LogregClassifierTest : public ::testing::Test {
public:
    LogregClassifierTest() { /* init protected members here */ }
    ~LogregClassifierTest() { /* free protected members here */ }
    void SetUp() 
    {  
        EXPECT_TRUE(coef_stream.is_open());
        EXPECT_TRUE(data_stream.is_open());
    }
    void TearDown() { /* called after every test */ }

protected:
    std::ifstream coef_stream{ "../logreg_coef.txt" };
    std::ifstream data_stream{ "../test_data_logreg.txt" };
};

TEST_F(LogregClassifierTest, main_test)
{
    LogregClassifier::coef_t coef = LogregClassifier::read_vector(coef_stream);

    LogregClassifier predictor(10, 785);

    predictor.load_model(coef);

    for (;;)
    {
        BinaryClassifier::features_t features;

        if (!LogregClassifier::read_features(data_stream, features))
            break;

        features[0] = 1;

        float predict_value = 0;
        size_t item_type = predictor.predict_proba(features, predict_value);

        EXPECT_NEAR(1, predict_value, 1e-2);
    }
}