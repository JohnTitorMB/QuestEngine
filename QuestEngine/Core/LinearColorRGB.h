#ifndef _LINEARCOLORRGB_H_
#define _LINEARCOLORRGB_H_

#include "ColorManagement/ColorRGBBase.h"

struct LinearColorRGB : public ColorRGBBase {
    using ColorRGBBase::ColorRGBBase;

    static LinearColorRGB Lerp(const LinearColorRGB& colorA, const LinearColorRGB& colorB, float t);

    const std::shared_ptr<ColorSpace> GetColorSpace() const override;
};

#endif
