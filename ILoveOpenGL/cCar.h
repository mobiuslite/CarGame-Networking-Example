#pragma once
#include "cGameObject.h"
#include "cCollider.h"

class cCar : public cGameObject
{
public:
	cCar(cMesh* mesh);
	virtual ~cCar();

	void Accelerate();
	void UnAccelerate();
	void Brake();
	void UnBrake();

	void TurnLeft(float deltaTime);
	void TurnRight(float deltaTime);

	virtual bool Integrate(float deltaTime);

	glm::vec3 ForceDrag();
	glm::vec3 ForceRollingResistance();
	glm::vec3 ForceTraction(float engineForce);

	glm::vec3 ForceLongitudinal(float engineForce);

	cCollider* collider;

	glm::vec3 Velocity();

private:
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	float m_mass;

	bool isBraking;
	bool isAccelerating;

	const float DRAG_CONSTANT = 2.5257f;
	const float ROLLING_RESISTANCE = 35.8f;
	const float BRAKE_CONSTANT = 70.0f;

	const float TURN_SPEED = 2.5f;
};