#include "cSceneLoader.h"

#include <fstream>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

cSceneLoader::cSceneLoader()
{
	sceneMeshes = new std::vector<cMesh*>();
	loadedModels = false;
	cameraPosition = glm::vec3(0);
}

cSceneLoader::~cSceneLoader()
{
	for (cMesh* mesh : *sceneMeshes)
	{
		delete mesh;
	}

	sceneMeshes->clear();
	delete sceneMeshes;
	std::cout << "Scene loader has been deleted!" << std::endl;
}

cSceneLoader* cSceneLoader::GetSceneLoaderInstance()
{
	return &cSceneLoader::_instance;
}
cSceneLoader cSceneLoader::_instance;


bool cSceneLoader::LoadScene(std::string sceneName, cBasicTextureManager* textureManager)
{
	using namespace rapidxml;

	std::string sceneFile("assets/scenes/" + sceneName + ".xml");

	rapidxml::file<>* xmlFile = new file<>(sceneFile.c_str());
	xml_document<>* doc = new xml_document<>;
	doc->parse<0>(xmlFile->data());

	xml_node<>* sceneNode = doc->first_node("Scene");
	xml_node<>* cameraPosNode = sceneNode->first_node("Camera")->first_node("Transform")->first_node("Position");
	float camX = std::stof(cameraPosNode->first_attribute("x")->value());
	float camY = std::stof(cameraPosNode->first_attribute("y")->value());
	float camZ = std::stof(cameraPosNode->first_attribute("z")->value());
	cameraPosition = glm::vec3(camX, camY, camZ);

	rapidxml::xml_node<>* modelsNode = sceneNode->first_node("Models");
	for (rapidxml::xml_node<>* modelNode = modelsNode->first_node(); modelNode != NULL; modelNode = modelNode->next_sibling())
	{
		sModel newModel;
		newModel.fileName = modelNode->value();

		models.push_back(newModel);
	}

	rapidxml::xml_node<>* descNode = sceneNode->first_node("Description");
	for (rapidxml::xml_node<>* meshNode = descNode->first_node(); meshNode != NULL; meshNode = meshNode->next_sibling())
	{
		cMesh* newMesh = new cMesh();
		newMesh->friendlyName = meshNode->first_attribute("FriendlyName")->value();
		newMesh->meshName = meshNode->first_attribute("MeshName")->value();

		xml_node<>* transformNode = meshNode->first_node("Transform");

		glm::vec3 pos;
		pos.x = std::stof(transformNode->first_node("Position")->first_attribute("x")->value());
		pos.y = std::stof(transformNode->first_node("Position")->first_attribute("y")->value());
		pos.z = std::stof(transformNode->first_node("Position")->first_attribute("z")->value());
		newMesh->positionXYZ = pos;

		glm::vec3 rot;
		rot.x = glm::radians(std::stof(transformNode->first_node("Rotation")->first_attribute("x")->value()));
		rot.y = glm::radians(std::stof(transformNode->first_node("Rotation")->first_attribute("y")->value()));
		rot.z = glm::radians(std::stof(transformNode->first_node("Rotation")->first_attribute("z")->value()));
		newMesh->orientationXYZ = rot;

		newMesh->scale = std::stof(transformNode->first_node("Scale")->value());
		xml_node<>* colorNode = meshNode->first_node("Colors");
		if (colorNode != nullptr)
		{

			xml_node<>* overrideColorNode = colorNode->first_node("OverrideColor");
			if (overrideColorNode != nullptr)
			{
				std::string useColor = overrideColorNode->value();
				newMesh->bUseWholeObjectDiffuseColour = useColor == "true" ? true : false;
			}

			glm::vec4 colors;
			colors.r = std::stof(colorNode->first_node("Color")->first_attribute("r")->value());
			colors.g = std::stof(colorNode->first_node("Color")->first_attribute("g")->value());
			colors.b = std::stof(colorNode->first_node("Color")->first_attribute("b")->value());
			colors.a = std::stof(colorNode->first_node("Color")->first_attribute("a")->value());
			newMesh->wholeObjectDiffuseRGBA = colors;
		}
		xml_node<>* miscNode = meshNode->first_node("Misc");

		if (miscNode != nullptr)
		{
			std::string useLighting = miscNode->first_node("UseLighting")->value();
			newMesh->bDontLight = useLighting == "true" ? false : true;

			xml_node<>* useSpecular = miscNode->first_node("UseSpecular");
			if (useSpecular != nullptr)
			{
				newMesh->bUseSpecular = true;
			}

			xml_node<>* texturesNode = miscNode->first_node("Textures");
			if (texturesNode != nullptr)
			{
				int index = 0;
				for (rapidxml::xml_node<>* textureNode = texturesNode->first_node(); textureNode != NULL && index < newMesh->MAX_TEXTURES; textureNode = textureNode->next_sibling())
				{
					Texture newTexture;
					newTexture.name = textureNode->value();
					newTexture.ratio = std::stof(textureNode->first_attribute("Ratio")->value());

					newMesh->textures[index] = newTexture;

					if (newTexture.name != "" && textureManager->getTextureIDFromName(newTexture.name) == 0)
					{
						if (!textureManager->Create2DTextureFromBMPFile(newTexture.name, true))
						{
							std::cout << "ERROR! Could not add texture to manager: "<< newTexture.name << std::endl;
						}
					}

					index++;
				}
			}
			xml_node<>* alphaMaskNode = miscNode->first_node("AlphaMask");
			if (alphaMaskNode != nullptr)
			{
				newMesh->bUseAlphaMask = true;
				newMesh->alphaMaskName = alphaMaskNode->value();

				if (textureManager->getTextureIDFromName(newMesh->alphaMaskName) == 0)
				{
					if (!textureManager->Create2DTextureFromBMPFile(newMesh->alphaMaskName, true))
					{
						std::cout << "ERROR! Could not add alpha mask to manager: " << newMesh->alphaMaskName << std::endl;
					}
				}
			}
		}

		newMesh->bIsSceneObject = true;
		sceneMeshes->push_back(newMesh);
	}

	delete xmlFile;
	doc->clear();
	delete doc;
	return true;
}

bool cSceneLoader::LoadIntoVAO(cVAOManager* vao, GLuint program)
{
	bool loadedProperly = true;

	for (int i = 0; i < this->models.size(); i++)
	{
		sModelDrawInfo modelInfo;
		if (vao->LoadModelIntoVAO(models[i].fileName, modelInfo, program))
		{
			std::cout << "\tLoaded model: " << models[i].fileName << std::endl;

			models[i].defaultScale = modelInfo.defaultScale;
		}
		else
		{
			loadedProperly = false;
			std::cout << "\tERROR: issue loading '" << models[i].fileName << "' into VAO" << std::endl;
		}
	}	

	return loadedProperly;
}

std::vector<cMesh*>* cSceneLoader::GetLoadedSceneMeshes()
{
	return sceneMeshes;
}

std::vector<sModel>* cSceneLoader::GetModels()
{
	return &this->models;
}

glm::vec3 cSceneLoader::GetCameraStartingPosition()
{
	return cameraPosition;
}

