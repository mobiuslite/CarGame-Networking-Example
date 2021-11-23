#include "cGameObject.h"
#include <glm/gtx/euler_angles.hpp>

cGameObject::cGameObject()
{
	this->p_mesh = nullptr;
}
cGameObject::cGameObject(cMesh* mesh)
{
	this->p_mesh = mesh;
}
cGameObject::~cGameObject()
{
}

sTransform cGameObject::Transform()
{
	sTransform transform;
	transform.position = this->p_mesh->positionXYZ;
	transform.rotation = this->p_mesh->orientationXYZ;

	return transform;
}
void cGameObject::Translate(glm::vec3 positionToAdd)
{
	this->p_mesh->positionXYZ += positionToAdd;
}
void cGameObject::Rotate(glm::vec3 rotationToAdd)
{
	this->p_mesh->orientationXYZ += rotationToAdd;
}

glm::vec3 cGameObject::ForwardVector()
{
	glm::vec4 direction = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

	//Get rotation matrix from the gameobject's rotation
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(this->p_mesh->orientationXYZ.x, -this->p_mesh->orientationXYZ.y, -this->p_mesh->orientationXYZ.z);
	rotationMatrix *= direction;

	//Extracts the matrix into a vec4
	glm::vec4 forwardVec = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	forwardVec = forwardVec * rotationMatrix;

	glm::vec3 returnTest = forwardVec;
	return glm::normalize(returnTest);
}

cMesh* cGameObject::Mesh()
{
	return this->p_mesh;
}