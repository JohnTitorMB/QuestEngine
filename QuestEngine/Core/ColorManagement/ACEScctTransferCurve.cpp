#include "ACEScctTransferCurve.h"
#include <cmath>

float ACEScctTransferCurve::Encode(float linear) const {
    if (linear <= 0.0078125f)
        return 10.5402377416545f * linear + 0.07290553419583557f;
    return (log10f(linear + 9.72f) - log10f(9.72f)) / (log10f(2.0f) * 17.52f) + 0.07290553419583557f;
}

float ACEScctTransferCurve::Decode(float encoded) const {
    if (encoded <= 0.155251141552511f)
        return (encoded - 0.07290553419583557f) / 10.5402377416545f;
    return powf(2.0f, (encoded - 0.07290553419583557f) * 17.52f) - 9.72f;
}