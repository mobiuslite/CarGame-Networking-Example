#include "cCollisionWorld.h"
#include <iostream>

cCollisionWorld::cCollisionWorld()
{
	this->collider = nullptr;
	this->lapTime = 0.0f;
}

void cCollisionWorld::Integrate(float deltaTime)
{
	for (std::vector<cCheckpointTriggerGenerator*>::iterator it = this->colliderTriggers.begin(); it != this->colliderTriggers.end(); it++)
	{
		//If the checkpoint is active, and the collider has collided with it.
		if ((*it)->isActive && (*it)->CheckContact(this->collider))
		{
			//Disables the current checkpoint
			(*it)->isActive = false;

			//Enables the next checkpoint
			if ((it + 1) != this->colliderTriggers.end())
			{
				
				(*(it + 1))->isActive = true;
			}

			//Done lap
			else
			{
				std::cout << "Lap Time: " << lapTime << std::endl;

				this->lapTime = 0.0f;
				(*this->colliderTriggers.begin())->isActive = true;
			}
		}
	}

	lapTime += deltaTime;
}

void cCollisionWorld::SetCollider(cCollider* collider)
{
	this->collider = collider;
}
void cCollisionWorld::AddCheckpointTrigger(cCheckpointTriggerGenerator* gen)
{
	this->colliderTriggers.push_back(gen);
}