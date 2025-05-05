#ifndef _COLORBASE_H_
#define _COLORBASE_H_

#include <memory>

class ColorModel;
class ColorSpace;

class ColorBase
{
public:
	virtual const std::shared_ptr<ColorSpace> GetColorSpace() const = 0;
	virtual const ColorModel& GetColorModel() const = 0;

public:
	float m_alpha = 1.0f;
};

#endif