#ifndef _LMSCOLORSPACE_H_
#define _LMSCOLORSPACE_H_
#include "ColorSpace.h"
#include "ColorModel.h"
class LMSColorSpace : public ColorSpace {

public:
    LMSColorSpace(const std::string& name = "LMS");

    Vector3D ToXYZ(const ColorModel& model) const override;
    std::unique_ptr<ColorModel> FromXYZ(const Vector3D& xyz, const ColorModelType& type) const override;
};

#endif