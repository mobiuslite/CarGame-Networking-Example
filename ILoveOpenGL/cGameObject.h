#pragma once
#include "cMesh.h"
#include "sTransform.h"

class cGameObject
{
public:
	cGameObject();
	cGameObject(cMesh* mesh);
	virtual ~cGameObject();

	//Read only position and orientation
	sTransform Transform();
	//Move the game object by that amount;
	void Translate(glm::vec3 positionToAdd);
	//In radians
	void Rotate(glm::vec3 rotationToAdd);
	//Gets the direction the game object is facing
	glm::vec3 ForwardVector();

	virtual void Integrate(float deltaTime) = 0;

	cMesh* Mesh();
protected:
	cMesh* p_mesh;
};