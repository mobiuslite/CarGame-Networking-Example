#pragma once
#include "cGameObject.h"
#include "cCollider.h"

class cNetworkCar : public cGameObject
{
public:
	cNetworkCar() = delete;
	cNetworkCar(cMesh* mesh);
	virtual ~cNetworkCar();

	virtual void Integrate(float deltaTime);

	void SetNetworkCar(glm::vec2 pos, glm::vec2 velo, float yRot);

private:
	glm::vec3 m_velocity;
};