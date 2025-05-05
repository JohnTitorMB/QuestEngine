#ifndef _LINEARTRANSFERCURVE_H_
#define _LINEARTRANSFERCURVE_H_

#include "TransferCurve.h"

class LinearTransferCurve : public TransferCurve
{
public:
    float Encode(float linear) const override;
    float Decode(float encoded) const override;
};

#endif
