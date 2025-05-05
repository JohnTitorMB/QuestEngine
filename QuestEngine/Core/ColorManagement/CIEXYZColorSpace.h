#ifndef _XYZCOLORSPACE_H_
#define _XYZCOLORSPACE_H_

#include "ColorSpace.h"
#include "ColorModel.h"

class CIEXYZColorSpace : public ColorSpace {
public:
    CIEXYZColorSpace(const std::string& name = "CIEXYZ");

    Vector3D ToXYZ(const ColorModel& model) const override;
    std::unique_ptr<ColorModel> FromXYZ(const Vector3D& xyz, const ColorModelType& type) const override;
};

#endif
