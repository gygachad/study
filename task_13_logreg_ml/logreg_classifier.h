#pragma once

#include <vector>
#include <Dense>

#include "classifier.h"

class LogregClassifier: public BinaryClassifier 
{
public:
    using coef_t = features_t;

    LogregClassifier(Eigen::Index count, Eigen::Index size) : _count(count), _size(size), coef_matrix(count, size) {}

    bool load_model(const coef_t& coef);
    size_t predict_proba(const features_t& feat, float& predict_value) override;

    static bool read_features(std::istream& stream, features_t& features);
    static std::vector<float> read_vector(std::istream& stream);

protected:

    template<typename T>
    static auto sigma(T x) { return 1 / (1 + std::exp(-x)); }

    Eigen::Index _count;
    Eigen::Index _size;

    Eigen::MatrixXf coef_matrix;
};