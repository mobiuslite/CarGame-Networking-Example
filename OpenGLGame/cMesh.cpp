#include "cMesh.h"


cMesh::cMesh()
{
	this->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	this->scale = 1.0f;

	this->bIsWireframe = false;
	this->bDisableDepthBufferCheck = false;

	this->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// White
	this->bUseObjectDebugColour = false;
	this->bDontLight = false;

	this->bIsDebugObject = false;

	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	this->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// The "colour" of the object
	this->bUseWholeObjectDiffuseColour = false;
	// Specular HIGHLIGHT colour (usually the same as the light, or white)
	this->wholeObjectSpecularRGB = glm::vec3(1.0f, 1.0f, 1.0f);			
	// Specular highlight POWER (or shininess). Starts at 1.0 and goes to ? (like 100, 1000, 10000 is OK)
	this->wholeObjectShininess_SpecPower = 50.0f;	// 1.0 to ??

	//this->alphaTransparency = 1.0f;

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		Texture newTexture;
		newTexture.ratio = 0.0f;
		this->textures.push_back(newTexture);
	}

	this->bUseAlphaMask = false;
	this->bUseSpecular = false;

	// Assign a unique ID
	this->m_UniqueID = cMesh::m_NextID;
	cMesh::m_NextID++;

	bIsSceneObject = false;
}


unsigned int cMesh::getUniqueID(void)
{
	return this->m_UniqueID;
}

//static 
unsigned int cMesh::m_NextID = cMesh::INITIAL_UNIQUE_ID;

