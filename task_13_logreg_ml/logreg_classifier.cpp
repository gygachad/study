
#include <cassert>
#include <numeric>
#include <cmath>

#include "logreg_classifier.h"

bool LogregClassifier::read_features(std::istream& stream, BinaryClassifier::features_t& features)
{
    std::string line;
    std::getline(stream, line);

    features.clear();
    std::istringstream linestream{ line };

    float value;
    while (linestream >> value) {
        features.push_back(value);
    }
    return stream.good();
}

std::vector<float> LogregClassifier::read_vector(std::istream& stream)
{
    std::vector<float> result;

    std::copy(std::istream_iterator<float>(stream),
        std::istream_iterator<float>(),
        std::back_inserter(result));
    return result;
}

bool LogregClassifier::load_model(const coef_t& coef)
{
    Eigen::Index coef_size = coef.size();

    if (coef_size != _count * _size)
        return false;

    for (Eigen::Index i = 0; i < _count; ++i) 
        for (Eigen::Index j = 0; j < _size; ++j) 
            coef_matrix(i, j) = coef[i*_size + j];

    return true;
}

size_t LogregClassifier::predict_proba(const BinaryClassifier::features_t& feat, float& predict_value)
{
    Eigen::MatrixXf features_matrix(_size, 1);

    for (Eigen::Index i = 0; i < _size; i++)
        features_matrix(i,0) = feat[i];

    Eigen::MatrixXf predict_matrix = coef_matrix * features_matrix;

    coef_t predict_values(_count);

    for (Eigen::Index i = 0; i < _count; i++)
        predict_values[i] = sigma(predict_matrix(i, 0));

    auto predict_value_it = std::max_element(predict_values.begin(), predict_values.end());

    predict_value = *predict_value_it;

    return std::distance(predict_values.begin(), predict_value_it);
}
