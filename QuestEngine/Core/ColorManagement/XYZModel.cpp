#include "XYZModel.h"

XYZModel::XYZModel() :
	m_x{ 0.0f }, m_y{ 0.0f }, m_z{ 0.0f }
{
}

XYZModel::XYZModel(float x, float y, float z) :
	m_x{ x }, m_y{ y }, m_z{ z }
{

}

XYZModel::XYZModel(const Vector3D xyz) : 
	m_x{ xyz.m_x }, m_y{ xyz.m_y }, m_z{ xyz.m_z }
{

}
