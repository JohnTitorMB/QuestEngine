#ifndef _CHROMATICADAPTATION_H_
#define _CHROMATICADAPTATION_H_

#include "../../Math/Matrix3x3.h"
#include "../../Math/Vector3D.h"

class ChromaticAdaptation {
public:
    enum class Method {
        Bradford,
        // VonKries, etc.
    };

    static Matrix3x3 GetAdaptationMatrix(const Vector3D& sourceWhiteXYZ, const Vector3D& destWhiteXYZ, Method method = Method::Bradford);
    static Vector3D Adapt(const Vector3D& colorXYZ, const Vector3D& sourceWhiteXYZ, const Vector3D& destWhiteXYZ, Method method = Method::Bradford);

private:
    static const Matrix3x3& GetTransformMatrix(Method method);
    static const Matrix3x3& GetInverseTransformMatrix(Method method);
};

#endif
