#ifndef _ACESCCT_TRANSFERCURVE_H_
#define _ACESCCT_TRANSFERCURVE_H_

#include "TransferCurve.h"

class ACEScctTransferCurve : public TransferCurve
{
public:
    float Encode(float linear) const override;
    float Decode(float encoded) const override;
};

#endif