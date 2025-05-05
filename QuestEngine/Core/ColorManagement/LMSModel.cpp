#include "LMSModel.h"

LMSModel::LMSModel() :
	m_l{ 0.0f }, m_m{ 0.0f }, m_s{ 0.0f }
{
}

LMSModel::LMSModel(float l, float m, float s) :
	m_l{ l }, m_m{ m }, m_s{ s }
{

}

LMSModel::LMSModel(const Vector3D& lms) :
	m_l{ lms.m_x }, m_m { lms.m_y}, m_s{ lms.m_z }
{

}