#ifndef _COLORRGBBASE_H_
#define _COLORRGBBASE_H_

#include "ColorBase.h"
#include "RGBModel.h"

struct ColorRGBBase : public ColorBase {
    float m_r = 0.0f;
    float m_g = 0.0f;
    float m_b = 0.0f;

    ColorRGBBase() = default;
    ColorRGBBase(float r, float g, float b, float a = 1.0f) {
        m_r = r;
        m_g = g;
        m_b = b;
        m_alpha = a;
    }

    const ColorModel& GetColorModel() const override;

protected:
    mutable RGBModel m_tempRGBModel;
};

#endif