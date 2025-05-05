#include "LinearTransferCurve.h"

float LinearTransferCurve::Encode(float linear) const {
    return linear; 
}

float LinearTransferCurve::Decode(float encoded) const {
    return encoded;
}
