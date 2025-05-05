#include "ACESccTransferCurve.h"
#include <cmath>

float ACESccTransferCurve::Encode(float linear) const {
    if (linear <= 0.0f)
        return (std::log10(std::pow(2.0f, -16.0f) + 9.72f) - std::log10(9.72f)) / (std::log10(2.0f) * 17.52f) + 0.0729055f;
    return (std::log10(linear + 9.72f) - std::log10(9.72f)) / (std::log10(2.0f) * 17.52f) + 0.0729055f;
}

float ACESccTransferCurve::Decode(float encoded) const {
    return powf(2.0f, (encoded - 0.07290553419583557f) * 17.52f) - 9.72f;
}
