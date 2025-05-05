#ifndef _CUSTOMCOLORRGB_H_
#define _CUSTOMCOLORRGB_H_

#include "ColorManagement/ColorRGBBase.h"
#include "LinearColorRGB.h"
#include <memory>

struct CustomColorRGB : public ColorRGBBase {
    CustomColorRGB(std::shared_ptr<ColorSpace> colorSpace, float r, float g, float b, float a = 1.0f);

    const std::shared_ptr<ColorSpace> GetColorSpace() const override;

private:
    std::shared_ptr<ColorSpace> m_colorSpace;
};

#endif
