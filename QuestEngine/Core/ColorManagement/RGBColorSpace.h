#ifndef _RGBCOLORSPACE_H_
#define _RGBCOLORSPACE_H_
#include "Gamut.h"
#include "ColorSpace.h"
#include "TransferCurve.h"
#include "ColorModel.h"

class RGBColorSpace : public ColorSpace {
protected:
    Gamut m_gamut;
    std::shared_ptr<TransferCurve> m_oetf;
    std::shared_ptr<TransferCurve> m_eotf;

public:
    RGBColorSpace(
        const std::string& name,
        const Gamut& gamut,
        std::shared_ptr<TransferCurve> oetf,
        std::shared_ptr<TransferCurve> eotf);

    Vector3D ToXYZ(const ColorModel& model) const override;
    std::unique_ptr<ColorModel> FromXYZ(const Vector3D& xyz, const ColorModelType& colorModelType) const override;

    std::shared_ptr<TransferCurve> GetOETF() const;
    std::shared_ptr<TransferCurve> GetEOTF() const;

    Vector3D Decode(const Vector3D& encoded) const;
    Vector3D Encode(const Vector3D& linear) const;

    const Gamut& GetGamut() const;
    void SetGamut(const Gamut& gamut);

};

#endif

