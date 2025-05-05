#ifndef _LMSMODEL_H_
#define _LMSMODEL_H_
#include "ColorModel.h"

class LMSModel : public ColorModel
{
public:
	float m_l = 0.0f;
	float m_m = 0.0f;
	float m_s = 0.0f;
public:
	LMSModel();
	LMSModel(float l, float m, float s);
	LMSModel(const Vector3D& lms);

	Vector3D GetLMS() const 
	{
		return Vector3D(m_l, m_m, m_s);
	}

	void SetLMS(const Vector3D& lms)
	{
		m_l = lms.m_x;
		m_m = lms.m_y;
		m_s = lms.m_z;
	}

	std::unique_ptr<ColorModel> Clone() const override {
		return std::make_unique<LMSModel>(*this);
	}
};

#endif