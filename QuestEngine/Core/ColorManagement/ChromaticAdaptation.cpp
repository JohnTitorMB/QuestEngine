#include "ChromaticAdaptation.h"
#include <stdexcept>

const Matrix3x3& ChromaticAdaptation::GetTransformMatrix(Method method) {
    static const Matrix3x3 bradford(
        0.8951f, 0.2664f, -0.1614f,
        -0.7502f, 1.7135f, 0.0367f,
        0.0389f, -0.0685f, 1.0296f
    );

    switch (method) {
    case Method::Bradford: return bradford;
    default: throw std::runtime_error("Unsupported chromatic adaptation method");
    }
}

const Matrix3x3& ChromaticAdaptation::GetInverseTransformMatrix(Method method) {
    static const Matrix3x3 bradfordInv(
        0.9869929f, -0.1470543f, 0.1599627f,
        0.4323053f, 0.5183603f, 0.0492912f,
        -0.0085287f, 0.0400428f, 0.9684867f
    );

    switch (method) {
    case Method::Bradford: return bradfordInv;
    default: throw std::runtime_error("Unsupported chromatic adaptation method");
    }
}

Matrix3x3 ChromaticAdaptation::GetAdaptationMatrix(const Vector3D& sourceWhiteXYZ, const Vector3D& destWhiteXYZ, Method method) {
    const auto& M = GetTransformMatrix(method);
    const auto& M_inv = GetInverseTransformMatrix(method);

    const Vector3D sourceLMS = M * sourceWhiteXYZ;
    const Vector3D destLMS = M * destWhiteXYZ;

    Matrix3x3 D(
        destLMS.m_x / sourceLMS.m_x, 0.0f, 0.0f,
        0.0f, destLMS.m_y / sourceLMS.m_y, 0.0f,
        0.0f, 0.0f, destLMS.m_z / sourceLMS.m_z
    );

    return M_inv * D * M;
}

Vector3D ChromaticAdaptation::Adapt(const Vector3D& colorXYZ, const Vector3D& sourceWhiteXYZ, const Vector3D& destWhiteXYZ, Method method) {
    return GetAdaptationMatrix(sourceWhiteXYZ, destWhiteXYZ, method) * colorXYZ;
}
