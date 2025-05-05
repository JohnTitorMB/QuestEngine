#ifndef _RGBMODEL_H_
#define _RGBMODEL_H_

#include "ColorModel.h"
#include "../../Math/Vector3D.h"

class RGBModel : public ColorModel
{
public:
    float m_r = 0.0f;
    float m_g = 0.0f;
    float m_b = 0.0f;

public:
    RGBModel();
    RGBModel(float r, float g, float b);
    RGBModel(const Vector3D& rgb);

    Vector3D GetRGB() const;

    static Vector3D ToHSV(Vector3D rgb);

    std::unique_ptr<ColorModel> Clone() const override {
        return std::make_unique<RGBModel>(*this);
    }
};

#endif
