#ifndef _HSLMODEL_H_
#define _HSLMODEL_H_

#include "ColorModel.h"
#include "RGBModel.h"

class HSLModel : public ColorModel
{
public:
	float m_h = 0.0f; // Hue : [0, 360)
	float m_s = 0.0f; // Saturation : [0, 1]
	float m_l = 0.0f; // Lightness : [0, 1]

public:
	HSLModel();
	HSLModel(float h, float s, float l);

	Vector3D GetHSL() const { return Vector3D(m_h, m_s, m_l); }

	RGBModel ToRGBModel() const;
	Vector3D ToRGB() const;
	void FromEncodedRGB(const Vector3D& encodedRGB);

	std::unique_ptr<ColorModel> Clone() const override {
		return std::make_unique<HSLModel>(*this);
	}
};

#endif
