//#include <glad/glad.h>
//
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)

#include "cVAOManager.h"
#include "cMesh.h"
#include "cShaderManager.h"

#include "cLightManager.h"
#include "cLightHelper.h"
#include "SceneLoader/cSceneLoader.h"
#include "SceneLoader/sModel.h"

#include "FlyCamera/cFlyCamera.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "cGameObject.h"
#include <algorithm>
#include "cCar.h"
#include "cCollisionWorld.h"
#include "Server.h"

enum class Transform
{
    Translate,
    Rotate,
    Scale
};

GLuint program;

cServer server = cServer("72.39.116.94", 27015);
//cServer server = cServer("192.168.1.186", 27015);

std::string username;

// Global things are here:

cFlyCamera* g_FlyCamera = NULL;
float flyCameraSpeed = 5.0f;
bool g_MouseIsInsideWindow = false;

glm::vec3 cameraAt = glm::vec3(0.0f);
glm::vec3 cameraOffset;
glm::vec3 cameraSmooth;

cVAOManager     gVAOManager;
cShaderManager  gShaderManager;

cLightManager gTheLights;
cLightHelper gTheLightHelper;

cBasicTextureManager g_textureManager;

// List of objects to draw
std::vector<cMesh*>* g_vecMeshes;
std::vector<cGameObject> g_gameObjects;

cMesh* g_DebugSphere = NULL;

unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 0;

bool useTween = true;
bool useDeadReck = true;

Transform transformType = Transform::Translate;

cSceneLoader* sceneLoader;
std::string sceneName = "project2";

cCar* playerCar;

float tick = 41.0f;
float tickTimeElapsed = 0.0f;

bool clientSidePrediction = true;

//Method in DrawObjectFunction
void extern DrawObject(cMesh* curMesh, glm::mat4 matModel, GLint program, cVAOManager* VAOManager, cBasicTextureManager textureManager, std::map<std::string, GLint> uniformLocations);

static void GLFW_cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
       // std::cout << "Mouse cursor is over the window" << std::endl;
        ::g_MouseIsInsideWindow = true;
    }
    else
    {
        //std::cout << "Mouse cursor is no longer over the window" << std::endl;
        ::g_MouseIsInsideWindow = false;
    }
    return;
}
static void GLFW_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float mouseScrollWheelSensitivity = 0.1f;

    ::g_FlyCamera->setMouseWheelDelta(yoffset * mouseScrollWheelSensitivity);

    return;

}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
       /* if (sceneLoader->SaveScene(sceneName, g_FlyCamera->getEye()))
        {
            std::cout << "Saved scene: " << sceneName << std::endl;
        }*/
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        useTween = !useTween;
    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        useDeadReck = !useDeadReck;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        tick = 1000.0f;
    }else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        tick = 100.0f;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        tick = 10.0f;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        tick = 16.f;
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        tick = 41.f;
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        server.SendReady();
    }

    return;
}

bool DistanceToCameraPredicate(const cMesh* a, const cMesh* b)
{
    if (glm::distance(a->positionXYZ, g_FlyCamera->getEye()) > glm::distance(b->positionXYZ, g_FlyCamera->getEye()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// We call these every frame
void ProcessAsyncMouse(GLFWwindow* window, float deltaTime)
{

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    ::g_FlyCamera->setMouseXY(x, y);

    const float MOUSE_SENSITIVITY = 4.0f;


    // Mouse left (primary?) button pressed? 
    // AND the mouse is inside the window...
    if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        && ::g_MouseIsInsideWindow)
    {
        // Mouse button is down so turn the camera
        ::g_FlyCamera->Yaw_LeftRight(-::g_FlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY * deltaTime);

        ::g_FlyCamera->Pitch_UpDown(::g_FlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY * deltaTime);

    }

    // Adjust the mouse speed
    if (::g_MouseIsInsideWindow)
    {
        const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

        // Adjust the movement speed based on the wheel position
        ::g_FlyCamera->movementSpeed -= (::g_FlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);

        // Clear the mouse wheel delta (or it will increase constantly)
        ::g_FlyCamera->clearMouseWheelValue();


        if (::g_FlyCamera->movementSpeed <= 0.0f)
        {
            ::g_FlyCamera->movementSpeed = 0.0f;
        }
    }
    return;
}

void ProcessAsyncKeyboard(GLFWwindow* window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
        {
           // g_FlyCamera->MoveForward_Z(flyCameraSpeed * deltaTime);
            playerCar->Accelerate();
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        {
            playerCar->UnAccelerate();
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS)
        {
            playerCar->Brake();
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        {
            playerCar->UnBrake();
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
        {
            //g_FlyCamera->MoveLeftRight_X(-flyCameraSpeed * deltaTime);
            playerCar->TurnLeft(deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS)
        {
            //g_FlyCamera->MoveLeftRight_X(flyCameraSpeed * deltaTime);
            playerCar->TurnRight(deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS)
        {
            //g_FlyCamera->MoveUpDown_Y(-flyCameraSpeed * deltaTime);
        }
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS)
        {
            //g_FlyCamera->MoveUpDown_Y(flyCameraSpeed * deltaTime);
        }
    }
}

int main(void)
{
    std::cout << "Enter a username!: ";
    std::getline(std::cin, username);

    std::cout << std::endl;

    GLFWwindow* window;

    g_FlyCamera = new cFlyCamera();

//    GLuint vertex_buffer = 0;     // ** NOW in VAO Manager **

//    GLuint vertex_shader;     // Now in the "Shader Manager"
//    GLuint fragment_shader;   // Now in the "Shader Manager"
    program = 0;     // 0 means "no shader program"

    GLint mvp_location = -1;        // Because glGetAttribLocation() returns -1 on error
//    GLint vpos_location = -1;       // Because glGetAttribLocation() returns -1 on error
//    GLint vcol_location = -1;       // Because glGetAttribLocation() returns -1 on error

    glfwSetErrorCallback(error_callback);


    if ( ! glfwInit() )
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 640, "Ethan's Engine", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);	// was “key_callback”
    // These are new:
    glfwSetCursorEnterCallback(window, GLFW_cursor_enter_callback);
    glfwSetScrollCallback(window, GLFW_scroll_callback);

    glfwMakeContextCurrent(window);
// Tiny change from the original documentation code
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress);
//    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);


    // void glGetIntegerv(GLenum pname, GLint * data);
    // See how many active uniform variables (registers) I can have
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
        
    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (gShaderManager.createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = gShaderManager.getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << gShaderManager.getLastError() << std::endl;
    }

    glUseProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");

    //SUN
    gTheLights.theLights[0].position = glm::vec4(0.f, 0.f, 0.f, 1.0f);
    gTheLights.theLights[0].diffuse = glm::vec4(.89f, .95f, 1.0f, 1.0f);
    gTheLights.theLights[0].atten = glm::vec4(0.2f, 0.1f, 0.005f, 100000.0f);
    gTheLights.theLights[0].direction = glm::vec4(0.0f, -1.0f, 1.0f, 1.0f);
    //gTheLights.theLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
    gTheLights.theLights[0].param1.x = 2;
    gTheLights.TurnOnLight(0);  // Or this!
    gTheLights.SetUpUniformLocations(program, 0);

    sceneLoader = cSceneLoader::GetSceneLoaderInstance();

    //Gets the path of the texture manager to the texture folder
    g_textureManager.SetBasePath("assets/textures");

    //Loads the scene and the textures used in the scene
    std::cout << "Loading scene " << sceneName << "...." << std::endl;
    if (sceneLoader->LoadScene(sceneName, &g_textureManager))
    {
        std::cout << "Loaded scene: " << sceneName << std::endl << std::endl;
        std::cout << "Loading scene into VAO manager...." << std::endl;
        if (sceneLoader->LoadIntoVAO(&gVAOManager, program))
        {
            std::cout << "Loaded scene into VAO manager" << std::endl << std::endl;

            cameraOffset = sceneLoader->GetCameraStartingPosition();
            cameraSmooth = cameraOffset;
        }
        else
        {
            std::cout << "ERROR: Issue loading models into vao" << std::endl;
        }
        g_vecMeshes = sceneLoader->GetLoadedSceneMeshes();
    }
    else
    {
        std::cout << "ERROR: Issue loading scene" << std::endl;
        return -1;
    }

    std::map<std::string, GLint> uniformLocations;

    uniformLocations.insert(std::pair<std::string, GLint>("matModel", glGetUniformLocation(program, "matModel")));
    uniformLocations.insert(std::pair<std::string, GLint>("matModelInverseTranspose", glGetUniformLocation(program, "matModelInverseTranspose")));
    // Copy the whole object colour information to the sahder               

            // This is used for wireframe or whole object colour. 
            // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    uniformLocations.insert(std::pair<std::string, GLint>("bUseDebugColour", glGetUniformLocation(program, "bUseDebugColour")));
    uniformLocations.insert(std::pair<std::string, GLint>("objectDebugColour", glGetUniformLocation(program, "objectDebugColour")));

    // If true, then the lighting contribution is NOT used. 
    // This is useful for wireframe object
    uniformLocations.insert(std::pair<std::string, GLint>("bDontLightObject", glGetUniformLocation(program, "bDontLightObject")));

    // The "whole object" colour (diffuse and specular)
    uniformLocations.insert(std::pair<std::string, GLint>("wholeObjectDiffuseColour", glGetUniformLocation(program, "wholeObjectDiffuseColour")));
    uniformLocations.insert(std::pair<std::string, GLint>("bUseWholeObjectDiffuseColour", glGetUniformLocation(program, "bUseWholeObjectDiffuseColour")));
    uniformLocations.insert(std::pair<std::string, GLint>("wholeObjectSpecularColour", glGetUniformLocation(program, "wholeObjectSpecularColour")));
    uniformLocations.insert(std::pair<std::string, GLint>("bUseSpecular", glGetUniformLocation(program, "bUseSpecular")));

    uniformLocations.insert(std::pair<std::string, GLint>("texture_00", glGetUniformLocation(program, "texture_00")));
    uniformLocations.insert(std::pair<std::string, GLint>("texture_01", glGetUniformLocation(program, "texture_01")));
    uniformLocations.insert(std::pair<std::string, GLint>("texture_02", glGetUniformLocation(program, "texture_02")));
    uniformLocations.insert(std::pair<std::string, GLint>("texture_03", glGetUniformLocation(program, "texture_03")));

    uniformLocations.insert(std::pair<std::string, GLint>("textureRatios", glGetUniformLocation(program, "textureRatios")));

    uniformLocations.insert(std::pair<std::string, GLint>("bUseAlphaMask", glGetUniformLocation(program, "bUseAlphaMask")));
    uniformLocations.insert(std::pair<std::string, GLint>("alphaMask", glGetUniformLocation(program, "alphaMask")));

    uniformLocations.insert(std::pair<std::string, GLint>("carPositionPlusRadius", glGetUniformLocation(program, "carPositionPlusRadius")));

    /*sModelDrawInfo debugSphere;
    if (!gVAOManager.LoadModelIntoVAO("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply", debugSphere, program))
    {
        std::cout << "Couldn't load debug sphere" << std::endl;
    }*/

    g_DebugSphere = new cMesh();
    g_DebugSphere->meshName = "Beachball.ply";
    g_DebugSphere->bIsWireframe = true;

    cMesh* carMesh = new cMesh();
    carMesh->meshName = "car.ply";
    carMesh->friendlyName = "PlayerCar";
    carMesh->bUseWholeObjectDiffuseColour = true;
    carMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    ::g_vecMeshes->push_back(carMesh);

    playerCar = new cCar(carMesh);

    playerCar->Translate(glm::vec3(0.0f, 0.0f, -40.0f));
    playerCar->Rotate(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));

    cCollisionWorld world = cCollisionWorld();

    //Add checkpoints to world
    for (cCheckpointTriggerGenerator* checkpoint : server.checkpoints)
    {
        world.AddCheckpointTrigger(checkpoint);
    }

    world.SetCollider(playerCar->collider);
    world.SetServer(&server);


    const double MAX_DELTA_TIME = 0.3;	// 100 ms
    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;

        float tickTime = tick / 1000.0f;

        world.Integrate((float)deltaTime);

        float ratio;
        int width, height;
        glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
        glm::mat4 p;
        glm::mat4 v;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        // Turn on the depth buffer
        glEnable(GL_DEPTH);         // Turns on the depth buffer
        glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

        glViewport(0, 0, width, height);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        // *******************************************************
        // Screen is cleared and we are ready to draw the scene...
        // *******************************************************

        // Copy the light information into the shader to draw the scene
        gTheLights.CopyLightInfoToShader();

        // Place the "debug sphere" at the same location as the selected light (again)
        // HACK: Debug sphere is 5th item added
        p = glm::perspective(glm::radians(50.0f),
            ratio,
            0.1f,
            1000.0f);     

        v = glm::mat4(1.0f);

        //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        //Keeps the camera somewhat pointed to the middle of the track.
        glm::vec3 cameraLookAtPoint = (playerCar->Mesh()->positionXYZ - cameraAt) / 1.2f;

        float cameraSpeed = 1.5f;

        //Camera Smoothing
        if (cameraSmooth.x != g_FlyCamera->getEye().x + cameraOffset.x)
        {
            cameraSmooth.x -= cameraSpeed * (float)deltaTime * (cameraSmooth.x - (g_FlyCamera->getEye().x + cameraOffset.x));
        }

        if (cameraSmooth.z != g_FlyCamera->getEye().z + cameraOffset.z)
        {
            cameraSmooth.z -= cameraSpeed * (float)deltaTime * (cameraSmooth.z - (g_FlyCamera->getEye().z + cameraOffset.z));
        }
        

        g_FlyCamera->setEye(playerCar->Transform().position + cameraOffset);


        v = glm::lookAt(cameraSmooth,     // "eye"
            cameraLookAtPoint,  // "at"
            g_FlyCamera->getUpVector());

        //std::cout << playerCar->Transform().position.x << ", " << playerCar->Transform().position.y << ", " << playerCar->Transform().position.z << std::endl;

        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

        //Sort based on transparency
        std::vector<cMesh*> transparentMeshes;
        std::vector<cMesh*> opaqueMeshes;
        for (unsigned int index = 0; index != g_vecMeshes->size(); index++)
        {
            if (g_vecMeshes->at(index)->wholeObjectDiffuseRGBA.a < 1.0f || g_vecMeshes->at(index)->bUseAlphaMask && g_vecMeshes->at(index)->friendlyName != "minecraft")
            {
                transparentMeshes.push_back(g_vecMeshes->at(index));
            }
            else
            {
                opaqueMeshes.push_back(g_vecMeshes->at(index));
            }
        }

        //Adds cars to render list.
        for (std::map<std::string, cNetworkCar*>::iterator carIt = server.networkCars.begin(); carIt != server.networkCars.end(); carIt++)
        {
            if ((*carIt).second->Mesh()->wholeObjectDiffuseRGBA.a < 1.0f || (*carIt).second->Mesh()->bUseAlphaMask)
            {
                transparentMeshes.push_back((*carIt).second->Mesh());
            }
            else
            {
                opaqueMeshes.push_back((*carIt).second->Mesh());
            }
        }

        //Sort transparent objects.
        std::sort(transparentMeshes.begin(), transparentMeshes.end(), DistanceToCameraPredicate);

        //Draw non transparent objects
        for (unsigned int index = 0; index != opaqueMeshes.size(); index++)
        {         
            cMesh* curMesh = opaqueMeshes[index];
            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            //mat4x4_identity(m);

            DrawObject(curMesh, matModel, program, &gVAOManager, g_textureManager, uniformLocations);
        }//for (unsigned int index

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Draw transparent objects
        for (unsigned int index = 0; index != transparentMeshes.size(); index++)
        {
            cMesh* curMesh = transparentMeshes[index];
            matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            //mat4x4_identity(m);

            DrawObject(curMesh, matModel, program, &gVAOManager, g_textureManager, uniformLocations);
        }//for (unsigned int index

        //Sets the debug sphere to the current checkpoint

        if (server.gameStarted)
        {
            g_DebugSphere->positionXYZ = (*std::find_if(
                world.colliderTriggers.begin(), world.colliderTriggers.end(),
                [](cCheckpointTriggerGenerator* trig) { return trig->isActive == true; }))->position;
        }
        else
        {
            g_DebugSphere->positionXYZ = glm::vec3(0.0f, -10.0f, 0.0f);
        }

        g_DebugSphere->scale = 0.01f;
        g_DebugSphere->bDontLight = true;
        g_DebugSphere->bUseObjectDebugColour = true;
        g_DebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        glm::mat4 matModelDS = glm::mat4(1.0f);
        DrawObject(g_DebugSphere, matModelDS, program, &gVAOManager, g_textureManager, uniformLocations);

        // "Present" what we've drawn.
        glfwSwapBuffers(window);
        glfwPollEvents();

        ProcessAsyncMouse(window, (float)deltaTime);
        ProcessAsyncKeyboard(window, (float)deltaTime);

        //Integrate cars
        playerCar->Integrate(deltaTime);
        g_FlyCamera->Update(deltaTime);
        tickTimeElapsed += deltaTime;

        //Makes the player immobile to show they are ready.
        if (server.ready && !server.gameStarted)
        {
            playerCar->Mesh()->positionXYZ = glm::vec3(0.0f, 0.0f, -40.0f);
            playerCar->SetVelocity(glm::vec3(0.0f));
        }

        server.CheckReceive(deltaTime);

        if (server.isConnected())
        {

            //Send car info on tick
            if (tickTimeElapsed >= tickTime)
            {
                server.SendCarState(username, playerCar->Transform().position, playerCar->Velocity(), playerCar->Transform().rotation.y);
                tickTimeElapsed = 0.0f;
            }

            //Finds cars to delete
            std::vector<cNetworkCar*> carsToDelete;
            for (std::map<std::string, cNetworkCar*>::iterator carMapIt = server.networkCars.begin(); carMapIt != server.networkCars.end();)
            {
                cNetworkCar* car = (*carMapIt).second;

                car->SetTween(useTween);
                car->SetDeadReck(useDeadReck);

                if (!car->Integrate((float)deltaTime))
                {
                    carsToDelete.push_back((*carMapIt).second);
                }

                ++carMapIt;
            }

            //Deletes cars
            for (cNetworkCar* car : carsToDelete)
            {
                server.RemoveNetworkCar(car);
            }
        }
    }

    delete g_FlyCamera;
    delete playerCar;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

