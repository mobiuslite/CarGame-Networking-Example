#pragma once
#include <vector>
#include <string>
#include "../cMesh.h"
#include "../cVAOManager.h"
#include <glad/glad.h>

#include "sModel.h"

#include "../cBasicTextureManager/cBasicTextureManager.h"

class cSceneLoader
{
public:
	~cSceneLoader();

	//Gets the instance of the scene loader
	static cSceneLoader* GetSceneLoaderInstance();

	//Loads and parses a json file what contains info about a scene.
	bool LoadScene(std::string sceneName, cBasicTextureManager* textureManager);

	//Loads the scene into the VAO manager
	bool LoadIntoVAO(cVAOManager* vao, GLuint program);

	//Returns the meshes generated from the scene.
	std::vector<cMesh*>* GetLoadedSceneMeshes();
	std::vector<sModel>* GetModels();

	glm::vec3 GetCameraStartingPosition();

	bool SaveScene(std::string sceneName, glm::vec3 cameraPos);

private:
	cSceneLoader();

	std::vector<sModel> models;
	std::vector<cMesh*>* sceneMeshes;
	glm::vec3 cameraPosition;

	static cSceneLoader _instance;

	bool loadedModels;
};
