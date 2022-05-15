#pragma once
#include "cCollider.h"
class cCheckpointTriggerGenerator
{

public:
	cCheckpointTriggerGenerator() = delete;
	cCheckpointTriggerGenerator(glm::vec3 position, float radius, bool isActive = false);

	cCheckpointTriggerGenerator& operator=(const cCheckpointTriggerGenerator& other) = delete;

	~cCheckpointTriggerGenerator();

	bool CheckContact(cCollider* collider);

	bool isActive;
	glm::vec3 position;

private:
	float radius;
};