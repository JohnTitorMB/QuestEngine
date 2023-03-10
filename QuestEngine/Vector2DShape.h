#ifndef  _VECTOR2DSHAPE_H_
#define  _VECTOR2DSHAPE_H_
#include "Vector2D.h"
#include "Shape.h"
class Vector2DShape
{
public :
	Vector2DShape(Vector2D vec, Vector2D pos, float r, float g, float b, float a);
	~Vector2DShape();
	void SetValue(Vector2D vec);
	void SetPosition(Vector2D position);
	void Draw(Camera* camera, Window* window);

private : 
	Vector2D value;
	Shape* m_shape;
	Shader* m_shader;


};

#endif // ! _VECTOR2DSHAPE_H_

