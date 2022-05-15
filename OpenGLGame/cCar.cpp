#include "cCar.h"
#include <iostream>

cCar::cCar(cMesh* mesh)
{
	this->p_mesh = mesh;

	this->isBraking = false;
	this->isAccelerating = false;

	this->m_velocity = glm::vec3(0.0f);
	this->m_acceleration = glm::vec3(0.0f);
	this->m_mass = 100;

	this->collider = new cCollider(this->Transform().position, 1.0f);
}
cCar::~cCar()
{
	delete this->collider;
}

glm::vec3 cCar::Velocity()
{
	return this->m_velocity;
}
void cCar::SetVelocity(glm::vec3 velo)
{
	this->m_velocity = velo;
}

void cCar::Accelerate()
{
	this->isAccelerating = true;
}
void cCar::UnAccelerate()
{
	this->isAccelerating = false;
}

void cCar::Brake()
{
	this->isBraking = true;
}
void cCar::UnBrake()
{
	this->isBraking = false;
}

void cCar::TurnLeft(float deltaTime)
{
	this->Mesh()->orientationXYZ.y += TURN_SPEED * deltaTime;
}
void cCar::TurnRight(float deltaTime)
{
	this->Mesh()->orientationXYZ.y -= TURN_SPEED * deltaTime;
}

bool cCar::Integrate(float deltaTime)
{
	//a = F / M
	this->m_acceleration = ForceLongitudinal(this->isAccelerating ? 2500.0f : 0.0f) / this->m_mass;

	//Vnew = V + dt * a
	this->m_velocity = this->m_velocity + deltaTime * this->m_acceleration;

	if ((this->Mesh()->positionXYZ.x > -80.0f || this->m_velocity.x > 0.0f) && (this->Mesh()->positionXYZ.x < 80.0f || this->m_velocity.x < 0.0f))
	{
		if ((this->Mesh()->positionXYZ.z > -60.0f || this->m_velocity.z > 0.0f) && (this->Mesh()->positionXYZ.z < 60.0f || this->m_velocity.z < 0.0f))
		{
			//Pnew = P + dt * v
			this->Mesh()->positionXYZ = this->Mesh()->positionXYZ + deltaTime * this->m_velocity;

			//Updates the position of this car's collider
			this->collider->position = this->Transform().position;

			//std::cout << this->p_mesh->positionXYZ.x << ", " << this->p_mesh->positionXYZ.z << std::endl;
		}
		else
		{
			this->m_velocity = glm::vec3(0.0f);
		}
	}
	else
	{
		this->m_velocity = glm::vec3(0.0f);
	}
	
	return true;
}

glm::vec3 cCar::ForceDrag()
{
	//Fdrag = -Cdrag * V * |V|
	return -this->DRAG_CONSTANT * this->m_velocity * glm::length(this->m_velocity);
}
glm::vec3 cCar::ForceRollingResistance()
{
	//Frr = -Crr * V
	return -this->ROLLING_RESISTANCE * this->m_velocity;
}
glm::vec3 cCar::ForceTraction(float engineForce)
{
	//Ftraction = u * Engineforce,
	return this->ForwardVector() * engineForce;
}
glm::vec3 cCar::ForceLongitudinal(float engineForce)
{
	//Flong = Ftraction + Fdrag + Frr + Fg (Fg gravity is not used)

	if(isBraking)
		return -(this->ForwardVector() * this->BRAKE_CONSTANT) + ForceDrag() + ForceRollingResistance();

	return ForceTraction(engineForce) + ForceDrag() + ForceRollingResistance();
}