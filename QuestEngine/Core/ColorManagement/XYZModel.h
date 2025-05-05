#ifndef _XYZMODEL_H_
#define _XYZMODEL_H_
#include "ColorModel.h"

class XYZModel : public ColorModel
{
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_z = 0.0f;

public:
	XYZModel();
	XYZModel(float x, float y, float z);
	XYZModel(const Vector3D xyz);

	Vector3D GetXYZ() const // encoded values, as stored
	{
		return Vector3D(m_x, m_y, m_z);
	}

	std::unique_ptr<ColorModel> Clone() const override {
		return std::make_unique<XYZModel>(*this);
	}
};

#endif