#include "LMSColorSpace.h"
#include "LMSModel.h"
#include <stdexcept>

LMSColorSpace::LMSColorSpace(const std::string& name)
    : ColorSpace(name) {
}

Vector3D LMSColorSpace::ToXYZ(const ColorModel& model) const {
    if (auto lmsModel = dynamic_cast<const LMSModel*>(&model)) {
        const Vector3D& lms = lmsModel->GetLMS();

        // Inverse Bradford Matrix (LMS to XYZ)
        Matrix3x3 LMS_to_XYZ = Matrix3x3(
            1.94735469f, -1.41445123f, 0.36476327f,
            0.68990272f, 0.34832189f, -0.03715970f,
            0.03692773f, -0.06442179f, 1.02794063f
        );

        return LMS_to_XYZ * lms;
    }

    throw std::runtime_error("LMSColorSpace::ToXYZ - Unsupported color model");
}

std::unique_ptr<ColorModel> LMSColorSpace::FromXYZ(const Vector3D& xyz, const ColorModelType& type) const {
    if (type != ColorModelType::LMS) {
        throw std::runtime_error("LMSColorSpace::FromXYZ - Only LMS model supported in LMSColorSpace.");
    }

    // Bradford Matrix (XYZ to LMS)
    Matrix3x3 XYZ_to_LMS = Matrix3x3(
        0.4002400f, 0.7076000f, -0.0808100f,
        -0.2263000f, 1.1653200f, 0.0457000f,
        0.0000000f, 0.0000000f, 0.9182200f
    );

    Vector3D lms = XYZ_to_LMS * xyz;
    return std::make_unique<LMSModel>(lms);
}
