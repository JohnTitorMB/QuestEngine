#ifndef  _MOVINGAVERAGE_H
#define _MOVINGAVERAGE_H

#include <vector>
class MovingAverage
{
	int m_size = 3;
	std::vector<float> m_data;
	float m_sum;
	int m_currentFrameIndex;

public:
	MovingAverage();
	MovingAverage(int _size);

	void Resize(int newSize);
	void AddSample(float value);
	float GetAverage()const;
};
#endif // ! _MOVINGAVERAGE_H

