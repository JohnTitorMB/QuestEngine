#include "RGBColorSpace.h"
#include "RGBModel.h"
#include "HSVModel.h"
#include "HSLModel.h"
#include "LinearTransferCurve.h"
#include <iostream>
RGBColorSpace::RGBColorSpace(const std::string& name, const Gamut& gamut, std::shared_ptr<TransferCurve> oetf, std::shared_ptr<TransferCurve> eotf) :
	ColorSpace(name), m_gamut{ gamut }, m_oetf{ oetf }, m_eotf{eotf}
{
}

Vector3D RGBColorSpace::ToXYZ(const ColorModel& model) const
{
    const RGBModel* rgbModel = dynamic_cast<const RGBModel*>(&model);
    if (rgbModel)
    {
        Vector3D linear = Decode(rgbModel->GetRGB());
        return m_gamut.GetToXYZMatrix() * linear;
    }

    const HSVModel* hsv = dynamic_cast<const HSVModel*>(&model);
    if (hsv)
    {
        Vector3D linear = Decode(hsv->ToRGB());
        return m_gamut.GetToXYZMatrix() * linear;
    }

    const HSLModel* hsl = dynamic_cast<const HSLModel*>(&model);
    if (hsl)
    {
        Vector3D linear = Decode(hsl->ToRGB());
        return m_gamut.GetToXYZMatrix() * linear;
    }

    throw std::runtime_error("ColorSpace::ToXYZ - Unsupported color model (expected RGB, HSV, or HSL)");
}

std::unique_ptr<ColorModel> RGBColorSpace::FromXYZ(const Vector3D& xyz, const ColorModelType& colorModelType) const
{

    Vector3D linearRGB = m_gamut.GetFromXYZMatrix() * xyz;


    switch (colorModelType)
    {
    case ColorModelType::RGB:
    {
        Vector3D encodedRGB = Encode(linearRGB);

        return std::make_unique<RGBModel>(encodedRGB);
    }

    case ColorModelType::HSV:
    {
        Vector3D encodedRGB = Encode(linearRGB);
        auto hsv = std::make_unique<HSVModel>();
        hsv->FromEncodedRGB(encodedRGB);
        return hsv;
    }

    case ColorModelType::HSL:
    {
        Vector3D encodedRGB = Encode(linearRGB);
        auto hsl = std::make_unique<HSLModel>();
        hsl->FromEncodedRGB(encodedRGB);
        return hsl;
    }

    default:
        throw std::runtime_error("ColorSpace::FromXYZ - Unsupported color model type.");
    }
}


std::shared_ptr<TransferCurve> RGBColorSpace::GetOETF() const {
    return m_oetf;
}

std::shared_ptr<TransferCurve> RGBColorSpace::GetEOTF() const {
    return m_eotf;
}

Vector3D RGBColorSpace::Decode(const Vector3D& encoded) const {
    return {
        m_eotf->Decode(encoded.m_x),
        m_eotf->Decode(encoded.m_y),
        m_eotf->Decode(encoded.m_z)
    };
}

Vector3D RGBColorSpace::Encode(const Vector3D& linear) const {
    return {
        m_oetf->Encode(linear.m_x),
        m_oetf->Encode(linear.m_y),
        m_oetf->Encode(linear.m_z)
    };
}

const Gamut& RGBColorSpace::GetGamut() const
{
    return m_gamut;
}

void RGBColorSpace::SetGamut(const Gamut& gamut)
{
    m_gamut = gamut;
}
