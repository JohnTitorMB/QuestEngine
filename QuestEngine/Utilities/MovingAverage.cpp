#include "MovingAverage.h"

MovingAverage::MovingAverage()
{
	m_size = 3;
	m_data.resize(m_size);
	m_sum = 0.0f;
	m_currentFrameIndex = 0;
}

MovingAverage::MovingAverage(int _size)
{
	m_size = _size;
	m_data.resize(m_size);

	m_sum = 0.0f;
	m_currentFrameIndex = 0;
}

void MovingAverage::Resize(int newSize)
{
	m_size = newSize;
	m_data.resize(m_size);
}

void MovingAverage::AddSample(float value)
{
	m_sum -= m_data[m_currentFrameIndex];
	m_data[m_currentFrameIndex] = value;
	m_sum += value;
	m_currentFrameIndex++;

	if (m_currentFrameIndex >= m_size)
		m_currentFrameIndex = 0;
}

float MovingAverage::GetAverage()const
{
	if (m_size != 0)
	{
		return m_sum / m_size;
	}

	return 0.0f;
}



