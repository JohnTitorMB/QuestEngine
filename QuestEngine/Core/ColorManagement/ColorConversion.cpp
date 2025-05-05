#include "ColorConversion.h"

#include "ColorConversion.h"
#include "RGBModel.h"
#include "HSVModel.h"
#include "HSLModel.h"
#include "XYZModel.h"
#include "../../Math/Vector3D.h"
#include "ChromaticAdaptation.h"
#include <stdexcept>
#include <iostream>

Color ColorConversion::Convert(
    const Color& sourceColor,
    ColorModelType targetModel,
    std::shared_ptr<ColorSpace> targetColorSpace
) {
    auto sharedSrcSpace = sourceColor.GetColorSpace();

    Vector3D xyz = sharedSrcSpace->ToXYZ(sourceColor.GetColorModel());

    auto* srcRGBSpace = dynamic_cast<RGBColorSpace*>(sharedSrcSpace.get());
    auto* dstRGBSpace = dynamic_cast<RGBColorSpace*>(targetColorSpace.get());

    if (srcRGBSpace && dstRGBSpace) {
        Vector3D sourceWhiteXYZ = srcRGBSpace->GetGamut().GetWhitePointXYZ();
        Vector3D targetWhiteXYZ = dstRGBSpace->GetGamut().GetWhitePointXYZ();

        if (sourceWhiteXYZ != targetWhiteXYZ) {
            xyz = ChromaticAdaptation::Adapt(
                xyz,
                sourceWhiteXYZ,
                targetWhiteXYZ,
                ChromaticAdaptation::Method::Bradford
            );
        }
    }

    std::unique_ptr<ColorModel> model = targetColorSpace->FromXYZ(xyz, targetModel);

    return Color(std::move(model), targetColorSpace);
}

CustomColorRGB ColorConversion::ConvertRGBColor(const ColorRGBBase& colorRGB, std::shared_ptr<RGBColorSpace> targetRGBSpace)
{
    std::shared_ptr<ColorSpace> sourceSpace = colorRGB.GetColorSpace();
    Vector3D xyz = sourceSpace->ToXYZ(colorRGB.GetColorModel());
    
    if (auto* srcRGB = dynamic_cast<RGBColorSpace*>(sourceSpace.get()))
    {
        Vector3D srcWhite = srcRGB->GetGamut().GetWhitePointXYZ();
        Vector3D dstWhite = targetRGBSpace->GetGamut().GetWhitePointXYZ();

        constexpr float epsilon = 1e-4f;
        if ((srcWhite - dstWhite).Magnitude() > epsilon)
        {
            xyz = ChromaticAdaptation::Adapt(
                xyz,
                srcWhite,
                dstWhite,
                ChromaticAdaptation::Method::Bradford
            );
        }
    }

    std::unique_ptr<ColorModel> model = targetRGBSpace->FromXYZ(xyz, ColorModelType::RGB);
    auto* rgb = dynamic_cast<RGBModel*>(model.get());

    if (!rgb)
        throw std::runtime_error("Failed to convert to RGBModel");

    return CustomColorRGB(targetRGBSpace, rgb->m_r, rgb->m_g, rgb->m_b, colorRGB.m_alpha);
}
