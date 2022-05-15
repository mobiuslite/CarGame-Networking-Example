#pragma once
#include <vector>
#include "cCollider.h"
#include "cCheckpointTriggerGenerator.h"
#include "Server.h"

class cCollisionWorld
{
public:
	cCollisionWorld();

	void Integrate(float deltaTime);

	void SetCollider(cCollider* collider);
	void SetServer(cServer* server);

	void AddCheckpointTrigger(cCheckpointTriggerGenerator* gen);

	std::vector<cCheckpointTriggerGenerator*> colliderTriggers;
	cCollider* collider;

private:

	cServer* server;
};