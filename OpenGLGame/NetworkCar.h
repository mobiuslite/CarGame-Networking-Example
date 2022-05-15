#pragma once
#include "cGameObject.h"
#include "cCollider.h"

class cNetworkCar : public cGameObject
{
public:
	cNetworkCar() = delete;
	cNetworkCar(cMesh* mesh);
	virtual ~cNetworkCar();

	virtual bool Integrate(float deltaTime);

	void SetTween(bool state);
	void SetDeadReck(bool state);

	void SetNetworkCar(glm::vec2 pos, glm::vec2 velo, float yRot);

private:
	glm::vec3 m_velocity;

	float yRotationTween;
	glm::vec3 positionTween;

	float tweenTime = 0.333f;
	float elaspedTweenTime = 0.0f;

	float destroyTime = 10.0f;
	float destoryElasped = 0.0f;

	bool useDeadReck;
	bool useTween;
};