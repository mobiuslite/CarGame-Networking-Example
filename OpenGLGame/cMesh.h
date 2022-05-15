#pragma once

// This represents a single mesh object we are drawing

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct Texture
{
	std::string name;
	float ratio;
};

class cMesh
{
public:
	cMesh();
	std::string meshName;		// The 3D Mesh model we are drawing

	// Use these values to search for this specific instance of the object
	// We would set these ourselves...
	std::string friendlyName;
	unsigned int friendlyID;

	glm::vec3 positionXYZ;
	glm::vec3 orientationXYZ;		// "angle"
	float scale;

	// Sets the overall colour of the object
	//	by overwriting the colour variable
	// HACK: We'll get rid of this once we have textures
	glm::vec4 objectDebugColourRGBA;
	bool bUseObjectDebugColour;
	bool bDontLight;
	// Changes polygon mode to LINES (instead of solid)
	bool bIsWireframe;				
	// Turns of the depth buffer check when drawn
	bool bDisableDepthBufferCheck;	

	bool bIsDebugObject;
	// These are for colouring the ENTIRE object as one colour.
	// Later, these will be replaced:
	// * The diffuse will be replaced by textures
	// * The specular will be replaced by specular and gloss maps
	glm::vec4 wholeObjectDiffuseRGBA;		// The "colour" of the object
	bool bUseWholeObjectDiffuseColour;		// If true, then wholeObject colour (not model vertex) is used

	glm::vec3 wholeObjectSpecularRGB;		// Specular HIGHLIGHT colour (usually the same as the light, or white)
	float wholeObjectShininess_SpecPower;	// 1.0 to ??

	bool bUseSpecular;
	//float alphaTransparency;

	bool bUseAlphaMask;
	std::vector<Texture> textures;
	std::string alphaMaskName;

	const unsigned int MAX_TEXTURES = 4;


	bool bIsSceneObject;
	unsigned int getUniqueID(void);

	//Children
	std::vector<cMesh*> vec_pChildren;
private:
	unsigned int m_UniqueID;
	static unsigned int m_NextID;
	// All our unique objects start at 9999
	const static unsigned int INITIAL_UNIQUE_ID = 9999;
};
