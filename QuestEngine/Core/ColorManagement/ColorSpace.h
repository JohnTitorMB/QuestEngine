#ifndef _COLORSPACE_H_
#define _COLORSPACE_H_

#include "../../Math/Matrix3x3.h"
#include "ColorModelType.h"
#include "ColorModel.h"
#include <memory>
#include <string>

class ColorSpace {
protected:
    std::string m_name;

public:
    ColorSpace(const std::string& name) : m_name(name) {}
    virtual ~ColorSpace() = default;

    virtual Vector3D ToXYZ(const ColorModel& model) const = 0;
    virtual std::unique_ptr<ColorModel> FromXYZ(const Vector3D& xyz, const ColorModelType& colorModelType) const = 0;

    const std::string& GetName() const { return m_name; }
};

#endif
