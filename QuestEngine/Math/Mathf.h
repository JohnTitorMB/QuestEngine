#ifndef _MATHF_H_ 
#define _MATHF_H_ 

class Mathf
{
public:
	static const float PI;
	static const float RadToDeg;
	static const float DegToRad;
	static const float Epsilon7;

	static float Clamp(float value, float min, float max);
	static float PingPong(float t, float length);
	static float Repeat(float t, float length);
};


#endif // !
