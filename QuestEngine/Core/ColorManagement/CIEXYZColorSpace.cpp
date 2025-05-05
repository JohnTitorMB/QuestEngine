#include "CIEXYZColorSpace.h"
#include "XYZModel.h"
#include <stdexcept>
#include "ColorModel.h"
CIEXYZColorSpace::CIEXYZColorSpace(const std::string& name)
    : ColorSpace(name) {
}

Vector3D CIEXYZColorSpace::ToXYZ(const ColorModel& model) const {
    const XYZModel* xyzModel = dynamic_cast<const XYZModel*>(&model);
    if (xyzModel) {
        return xyzModel->GetXYZ();
    }

    throw std::runtime_error("CIEXYZColorSpace::ToXYZ - Unsupported model (expected XYZModel)");
}

std::unique_ptr<ColorModel> CIEXYZColorSpace::FromXYZ(const Vector3D& xyz, const ColorModelType& type) const {
    if (type != ColorModelType::XYZ) {
        throw std::runtime_error("XYZColorSpace::FromXYZ - Only XYZModel is supported in XYZColorSpace");
    }

    return std::make_unique<XYZModel>(xyz);
}
