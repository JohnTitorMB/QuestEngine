#ifndef _ACESCC_TRANSFERCURVE_H_
#define _ACESCC_TRANSFERCURVE_H_

#include "TransferCurve.h"

class ACESccTransferCurve : public TransferCurve {
public:
    float Encode(float linear) const override;
    float Decode(float encoded) const override;
};

#endif