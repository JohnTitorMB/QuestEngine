#ifndef _TRANSFERCURVE_H_
#define _TRANSFERCURVE_H_

class TransferCurve {
public:
    virtual float Encode(float linear) const = 0;
    virtual float Decode(float encoded) const = 0;
};

#endif
