#include "cCollisionWorld.h"
#include <iostream>

cCollisionWorld::cCollisionWorld()
{
	this->collider = nullptr;
	this->server = nullptr;
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
				this->server->DoneLap();
			}
		}
	}
}

void cCollisionWorld::SetCollider(cCollider* collider)
{
	this->collider = collider;
}
void cCollisionWorld::SetServer(cServer* server)
{
	this->server = server;
}

void cCollisionWorld::AddCheckpointTrigger(cCheckpointTriggerGenerator* gen)
{
	this->colliderTriggers.push_back(gen);
}