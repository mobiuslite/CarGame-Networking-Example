#pragma once
#include <vector>
#include "cCollider.h"
#include "cCheckpointTriggerGenerator.h"

class cCollisionWorld
{
public:
	cCollisionWorld();

	void Integrate(float deltaTime);

	void SetCollider(cCollider* collider);
	void AddCheckpointTrigger(cCheckpointTriggerGenerator* gen);

	std::vector<cCheckpointTriggerGenerator*> colliderTriggers;
	cCollider* collider;

private:
	float lapTime;
};