#ifndef _CAMERACONTROLLER_H_
#define _CAMERACONTROLLER_H_
#include "../Core/Component.h"
#include "../Core/Components/Camera.h"
#include "../Core/Events/EventCallback.h"
#include "../Core/Inputs/InputAction.h"
#include "../Utilities/MovingAverage.h"
#include "../Core/Inputs/InputMap.h"


class CameraController : public Component
{
private:
	InputMap* m_inputMap;
	CameraComponent* m_cameraComponent;
protected:
	void Update() override;
	void Start() override;
public:
	CameraController();
	CameraController(const CameraController& cameraController);
	~CameraController();
	Component* Clone()override;
	void AssignPointerAndReference()override;

	void MoveRight(InputCallbackData data);
	void MoveLeft(InputCallbackData data);
	void MoveForward(InputCallbackData data);
	void MoveBackward(InputCallbackData data);

	void ScrollMove(InputCallbackData data);

	void RotateX(InputCallbackData data);
	void RotateY(InputCallbackData data);
	void ControlCamera(InputCallbackData data);
	void UnControlCamera(InputCallbackData data);

	void AutoMove(float speed);

	float m_pitch = 0;
	float m_yaw = 0;
	bool m_controlCamera = false;
	float m_sensibility = 0.2f;
	float m_pitchMinValue = -89;
	float m_pitchMaxValue = 89;
	MovingAverage m_averageDeltaX;
	MovingAverage m_averageDeltaY;


	float m_deltaX = 0;
	float m_deltaY = 0;

	float movementSpeed = 0.03f;

	void DigitalInputTestStarted(InputCallbackData data);
	void DigitalInputTestOnGoing(InputCallbackData data);
	void DigitalInputTestCompleted(InputCallbackData data);
	void DigitalInputTestOnModified(InputCallbackData data);

	void AnalogInputTestStarted(InputCallbackData data);
	void AnalogInputTestOnGoing(InputCallbackData data);
	void AnalogInputTestOnModified(InputCallbackData data);
	void AnalogInputTestCompleted(InputCallbackData data);
};

#endif
