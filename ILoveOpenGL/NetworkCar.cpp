#include "NetworkCar.h"

cNetworkCar::cNetworkCar(cMesh* mesh)
{
	this->p_mesh = mesh;
	this->m_velocity = glm::vec3(0.0f);
}
cNetworkCar::~cNetworkCar()
{
}

void cNetworkCar::SetNetworkCar(glm::vec2 pos, glm::vec2 velo, float yRot)
{
	this->p_mesh->positionXYZ.x = pos.x;
	this->p_mesh->positionXYZ.z = pos.y;

	this->m_velocity.x = velo.x;
	this->m_velocity.z = velo.y;

	this->p_mesh->orientationXYZ.y = yRot;

}

void cNetworkCar::Integrate(float deltaTime)
{
	//Pnew = P + dt * v
	this->Mesh()->positionXYZ = this->Mesh()->positionXYZ + deltaTime * this->m_velocity;
}