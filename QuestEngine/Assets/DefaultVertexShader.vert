#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec2 shapePosition;
uniform vec2 cameraPosition;
uniform vec2 shapeScale;
uniform float shapeAngle;
uniform float cameraVerticalSize;
uniform float cameraHorizontalSize;		
void main()
{
	/*Transform vertice from localSpace into worldSpace */
	/*Scale vertex */
	vec2 scaleVertex = vec2(aPos.x*shapeScale.x, aPos.y * shapeScale.y);
	
	/*Rotation vertex */
	vec2 rotateVertex = vec2(0.0f, 0.0f);
	rotateVertex.x =  scaleVertex.x * cos(shapeAngle) - scaleVertex.y * sin(shapeAngle);
	rotateVertex.y =  scaleVertex.y * cos(shapeAngle) + scaleVertex.x * sin(shapeAngle);
	
	/*Translate vertex */
	vec2 vertexInWorldSpace = vec2(shapePosition.x + rotateVertex.x, shapePosition.y + rotateVertex.y);
	
	/*Transform vertice from worldSpace into cameraSpace (viewSpace or eyeSpace) */
	vec2 vertexInCameraSpace = vertexInWorldSpace - cameraPosition;
	
	/*Transform vertice from cameraSpace into NDCSpace*/
	vec2 vertexInNDCSpace;
	vertexInNDCSpace.x = vertexInCameraSpace.x*2.0/cameraHorizontalSize;
	vertexInNDCSpace.y = vertexInCameraSpace.y*2.0/cameraVerticalSize;
	
	gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, aPos.z, 1.0);
}