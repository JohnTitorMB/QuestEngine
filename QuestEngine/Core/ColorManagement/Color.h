#ifndef _COLOR_H_
#define _COLOR_H_

#include <memory>
#include "ColorModel.h"
#include "ColorSpace.h"
#include "ColorBase.h"
class Color : public ColorBase {
public:
    Color(std::unique_ptr<ColorModel> model, std::shared_ptr<ColorSpace> space, float alpha = 1.0f);

    const std::shared_ptr<ColorSpace> GetColorSpace() const override;
    const ColorModel& GetColorModel() const override;

protected:
    std::unique_ptr<ColorModel> m_colorModel;
    std::shared_ptr<ColorSpace> m_colorSpace;
};

#endif // !_COLOR_H_
