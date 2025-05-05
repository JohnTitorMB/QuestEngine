#ifndef _SRGBTRANSFERCURVE_H_
#define _SRGBTRANSFERCURVE_H_

#include "TransferCurve.h"

class sRGBTransferCurve : public TransferCurve
{
public:
    float Encode(float linear) const override;
    float Decode(float encoded) const override;
};

#endif
