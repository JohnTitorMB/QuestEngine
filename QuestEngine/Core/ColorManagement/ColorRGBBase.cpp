#include "ColorRGBBase.h"

const ColorModel& ColorRGBBase::GetColorModel() const {
    m_tempRGBModel.m_r = m_r;
    m_tempRGBModel.m_g = m_g;
    m_tempRGBModel.m_b = m_b;
    return m_tempRGBModel;
}