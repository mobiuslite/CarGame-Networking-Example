#include "cCheckpointTriggerGenerator.h"
#include <glm/geometric.hpp>
#include <iostream>

cCheckpointTriggerGenerator::cCheckpointTriggerGenerator(glm::vec3 position, float radius, bool isActive) : position(position), radius(radius), isActive(isActive)
{
}

cCheckpointTriggerGenerator::~cCheckpointTriggerGenerator()
{
}

bool cCheckpointTriggerGenerator::CheckContact(cCollider* collider)
{

	glm::vec3 positionA = this->position;
	glm::vec3 positionB = collider->position;

	//Finds the differences between the two spheres positions
	glm::vec3 differenceVector = positionA - positionB;
	float differenceLength = glm::length(differenceVector);

	//If the spheres are not touching, return
	if (differenceLength <= 0.0f || differenceLength >= this->radius + collider->radius)
	{
		return false;
	}

	//Spheres touching
	else
	{
		return true;
	}

}