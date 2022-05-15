#include "NetworkCar.h"

float lerp(float a, float b, float t);
glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

cNetworkCar::cNetworkCar(cMesh* mesh)
{
	this->p_mesh = mesh;
	this->m_velocity = glm::vec3(0.0f);
}
cNetworkCar::~cNetworkCar()
{
}

void cNetworkCar::SetNetworkCar(glm::vec2 pos, glm::vec2 velo, float yRot)
{
	//this->p_mesh->positionXYZ.x = pos.x;
	//this->p_mesh->positionXYZ.z = pos.y;

	positionTween.x = pos.x;
	positionTween.z = pos.y;

	this->m_velocity.x = velo.x;
	this->m_velocity.z = velo.y;

	//this->p_mesh->orientationXYZ.y = yRot;
	yRotationTween = yRot;
	this->elaspedTweenTime = 0.0f;

	this->destoryElasped = 0.0f;
}

bool cNetworkCar::Integrate(float deltaTime)
{
	//Pnew = P + dt * v
	
	if (this->useDeadReck)
	{
		if ((this->Mesh()->positionXYZ.x > -80.0f || this->m_velocity.x > 0.0f) && (this->Mesh()->positionXYZ.x < 80.0f || this->m_velocity.x < 0.0f))
		{
			if ((this->Mesh()->positionXYZ.z > -60.0f || this->m_velocity.z > 0.0f) && (this->Mesh()->positionXYZ.z < 60.0f || this->m_velocity.z < 0.0f))
			{
				this->p_mesh->positionXYZ = this->p_mesh->positionXYZ + deltaTime * this->m_velocity;
			}
		}
	}

	this->elaspedTweenTime += deltaTime;

	//Tweens rotation
	if (yRotationTween != this->p_mesh->orientationXYZ.y && elaspedTweenTime < tweenTime)
	{
		if (this->useTween)
			this->p_mesh->orientationXYZ.y = lerp(this->p_mesh->orientationXYZ.y, yRotationTween, elaspedTweenTime / tweenTime);
		else
			this->p_mesh->orientationXYZ.y = yRotationTween;
	}

	//Tweens position
	if (positionTween != this->p_mesh->positionXYZ && elaspedTweenTime < tweenTime)
	{
		if (this->useTween)
			this->p_mesh->positionXYZ = lerp(this->p_mesh->positionXYZ, positionTween, elaspedTweenTime / tweenTime);
		else
			this->p_mesh->positionXYZ = positionTween;
	}

	destoryElasped += deltaTime;

	if (destoryElasped > destroyTime)
	{
		return false;
	}

	return true;
}

void cNetworkCar::SetTween(bool state)
{
	this->useTween = state;
}
void cNetworkCar::SetDeadReck(bool state)
{
	this->useDeadReck = state;
}

float lerp(float a, float b, float t)
{
	if (t > 1.0f)
		t = 1.0f;

	return a + t * (b - a);
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
{
	if (t > 1.0f)
		t = 1.0f;

	return a + t * (b - a);
}