// task_7_bulk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "logreg_classifier.h"

int main(int argc, char* argv[])
{
    if (argc != 3) 
	{
        std::cout << "Usage: logreg_classifier model_path test_path\n";
        return 1;
    }
	
    const std::string filename(argv[1]);

    std::ifstream istream{ argv[1] };
    if (!istream.is_open()) 
    {
        std::cout << "Can't open file" << argv[1];
        return 1;
    }
    
    LogregClassifier::coef_t coef = LogregClassifier::read_vector(istream);

    LogregClassifier predictor(10,785);

    predictor.load_model(coef);

    std::ifstream test_data{ argv[2] };
    if (!test_data.is_open()) 
    {
        std::cout << "Can't open file " << argv[2];
        return 1;
    }

    for (;;) 
    {
        BinaryClassifier::features_t features;

        if (!LogregClassifier::read_features(test_data, features))
            break;

        std::cout << "Test value " << features[0] << std::endl;

        features[0] = 1;

        float predict_value = 0;

        size_t item_type = predictor.predict_proba(features, predict_value);

        std::cout << "Predict value " << predict_value << " item type " << item_type << std::endl;
    }

    return 0;
}