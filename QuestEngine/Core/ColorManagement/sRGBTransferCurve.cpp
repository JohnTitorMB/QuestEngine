#include "sRGBTransferCurve.h"
#include <cmath>

float sRGBTransferCurve::Encode(float linear) const {
    if (linear <= 0.0031308f)
        return 12.92f * linear;
    else
        return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
}

float sRGBTransferCurve::Decode(float encoded) const {
    if (encoded <= 0.04045f)
        return encoded / 12.92f;
    else
        return std::pow((encoded + 0.055f) / 1.055f, 2.4f);
}
