#ifndef _COLORCONVERSION_H_
#define _COLORCONVERSION_H_

#include "Color.h"
#include "../ColorRGB.h"
#include "../LinearColorRGB.h"
#include "ColorModelType.h"
#include <memory>
#include "../CustomColorRGB.h"
#include "RGBColorSpace.h"

class ColorConversion
{
public:
    static Color Convert(
        const Color& sourceColor,
        ColorModelType targetModel,
        std::shared_ptr<ColorSpace> targetColorSpace
    );

    static CustomColorRGB ConvertRGBColor(const ColorRGBBase& colorRGB, std::shared_ptr<RGBColorSpace> targetColorSpace);

};


#endif
