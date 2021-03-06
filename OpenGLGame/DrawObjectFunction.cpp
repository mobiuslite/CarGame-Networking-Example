#include "cMesh.h" // glm::mat4
#include "GLCommon.h"
#include "cVAOManager.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cBasicTextureManager/cBasicTextureManager.h"

void SetUpTextures(cMesh* curMesh, cBasicTextureManager textureManager, std::map<std::string, GLint> uniformLocations)
{
    float ratioOne = curMesh->textures[0].ratio;
    float ratioTwo = curMesh->textures[1].ratio;
    float ratioThree = curMesh->textures[2].ratio;
    float ratioFour = curMesh->textures[3].ratio;

    glUniform4f(uniformLocations["textureRatios"],
        ratioOne, ratioTwo, ratioThree, ratioFour);

    if (curMesh->bUseAlphaMask)
    {
        glUniform1f(uniformLocations["bUseAlphaMask"], (float)GL_TRUE);

        GLint textureId = textureManager.getTextureIDFromName(curMesh->alphaMaskName);
        if (textureId != 0)
        {
            GLint unit = 0;
            glActiveTexture(unit + GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uniformLocations["alphaMask"], unit);
        }
    }
    else
    {
        glUniform1f(uniformLocations["bUseAlphaMask"], (float)GL_FALSE);
    }

    if(ratioOne > 0.0f)
    {
        GLint textureId = textureManager.getTextureIDFromName(curMesh->textures[0].name);
        if (textureId != 0)
        {
            GLint unit = 1;
            glActiveTexture(unit + GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glUniform1i(uniformLocations["texture_00"], unit);
        }
    }
    
    if (curMesh->textures[1].ratio > 0.0f)
    {
        GLint textureId = textureManager.getTextureIDFromName(curMesh->textures[1].name);
        GLint unit = 2;
        glActiveTexture(unit + GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(uniformLocations["texture_01"], unit);
    }
}

void DrawObject(cMesh* curMesh, glm::mat4 matModel, GLint program, cVAOManager* VAOManager, cBasicTextureManager textureManager, std::map<std::string, GLint> uniformLocations)
{
    SetUpTextures(curMesh, textureManager, uniformLocations);

    // *****************************************************
            // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        curMesh->positionXYZ);

    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        curMesh->orientationXYZ.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));

    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        curMesh->orientationXYZ.y,
        glm::vec3(0.0f, 1.0f, 0.0f));

    //matModel = matModel * rotateY;
    // *****************************************************

    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        curMesh->orientationXYZ.x,
        glm::vec3(1.0f, 0.0f, 0.0f));

    //matModel = matModel * rotateX;
    // *****************************************************


    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(curMesh->scale,  // Scale in X
            curMesh->scale,  // Scale in Y
            curMesh->scale));// Scale in Z

//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;     // <-- mathematically, this is 1st

    GLint matModel_Location = uniformLocations["matModel"];
    GLint matModelInverseTranspose_Location = uniformLocations["matModelInverseTranspose"];
    // Copy the whole object colour information to the sahder               

            // This is used for wireframe or whole object colour. 
            // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    GLint bUseDebugColour_Location = uniformLocations["bUseDebugColour"];
    GLint objectDebugColour_Location = uniformLocations["objectDebugColour"];

    // If true, then the lighting contribution is NOT used. 
    // This is useful for wireframe object
    GLint bDontLightObject_Location = uniformLocations["bDontLightObject"];

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = uniformLocations["wholeObjectDiffuseColour"];
    GLint bUseWholeObjectDiffuseColour_Location = uniformLocations["bUseWholeObjectDiffuseColour"];
    GLint wholeObjectSpecularColour_Location = uniformLocations["wholeObjectSpecularColour"];

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));

    if (curMesh->friendlyName == "PlayerCar")
    {
        glUniform4f(uniformLocations["carPositionPlusRadius"], curMesh->positionXYZ.x, curMesh->positionXYZ.y, curMesh->positionXYZ.z, 1.0f);
    }
    else
    {
        glUniform4f(uniformLocations["carPositionPlusRadius"], 0.0f, 0.0f, 0.0f, -1.0f);
    }

    if (curMesh->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location,
            curMesh->wholeObjectDiffuseRGBA.r,
            curMesh->wholeObjectDiffuseRGBA.g,
            curMesh->wholeObjectDiffuseRGBA.b,
            curMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location,
        curMesh->wholeObjectSpecularRGB.r,
        curMesh->wholeObjectSpecularRGB.g,
        curMesh->wholeObjectSpecularRGB.b,
        curMesh->wholeObjectShininess_SpecPower);


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (curMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
            curMesh->objectDebugColourRGBA.r,
            curMesh->objectDebugColourRGBA.g,
            curMesh->objectDebugColourRGBA.b,
            curMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    }

    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (curMesh->bDontLight)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }

    if (curMesh->bUseSpecular)
    {
        // Override the colour...
        glUniform1f(uniformLocations["bUseSpecular"], (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(uniformLocations["bUseSpecular"], (float)GL_FALSE);
    }

    // Wireframe
    if (curMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    sModelDrawInfo modelInfo;
    //        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
    //        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

    if (VAOManager->FindDrawInfoByModelName(curMesh->meshName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }


}