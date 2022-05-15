#pragma once
#include <glm/vec3.hpp>

class cCollider
{
public:
	cCollider() = delete;
	cCollider(glm::vec3 position, float radius);

	glm::vec3 position;
	float radius;

	bool isActive;

private:

};