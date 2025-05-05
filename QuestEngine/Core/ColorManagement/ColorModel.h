#ifndef _COLORMPDEL_H_
#define _COLORMPDEL_H_

#include "../../Math/Vector3D.h"
#include "../../Math/Vector2D.h"

class ColorModel {
public:
    virtual ~ColorModel() = default;

    virtual std::unique_ptr<ColorModel> Clone() const = 0;
};

#endif // !_COLORMPDEL_H_