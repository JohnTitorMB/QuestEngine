#include "Color.h"

Color::Color(std::unique_ptr<ColorModel> model, std::shared_ptr<ColorSpace> space, float alpha)
    : m_colorModel(std::move(model)),
    m_colorSpace(std::move(space))
{
     m_alpha = {alpha};
}

const std::shared_ptr<ColorSpace> Color::GetColorSpace() const {
    return m_colorSpace;
}

const ColorModel& Color::GetColorModel() const
{
    return *m_colorModel;
}