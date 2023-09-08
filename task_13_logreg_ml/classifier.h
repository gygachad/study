#pragma once

#include <vector>

class BinaryClassifier {
public:
    using features_t = std::vector<float>;

    virtual ~BinaryClassifier() {}

    virtual size_t predict_proba(const features_t& feat, float& predict_value) = 0;
};
