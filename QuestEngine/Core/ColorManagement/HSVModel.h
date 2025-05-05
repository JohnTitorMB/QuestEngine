#ifndef _HSVMODEL_H_
#define _HSVMODEL_H_
#include "ColorModel.h"
#include "RGBModel.h"

class HSVModel : public ColorModel
{
public:
	float m_h = 0.0f;
	float m_s = 0.0f;
	float m_v = 0.0f;

public:
	HSVModel();
	HSVModel(float h, float s, float v);

	Vector3D GetHSV() const 
	{
		return Vector3D(m_h, m_s, m_v);
	}

	RGBModel ToRGBModel() const;
	Vector3D ToRGB() const;
	static Vector3D ToRGB(Vector3D hsv);
	void FromEncodedRGB(const Vector3D& encodedRGB);

	std::unique_ptr<ColorModel> Clone() const override {
		return std::make_unique<HSVModel>(*this);
	}
};

#endif