#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "BlockArrayData.h"
#include "glmh.h"
#include "TextureFetcher.h"
#include "RenderInclude.h"
#include "TextureArrayCreator.h"
#include "RenderChunkBuffer.h"

class World;

class WorldRenderer {
    public:
        WorldRenderer();

        void renderFrame(World* world);
        void renderOverlay(float rectangle[48], std::string texture);
        void renderBlockInWireframe(World* world, BlockPos pos);
        void updateAspectRatio(GLFWwindow* window);
        void updateWorldVBO(World* world);

        template<class T>
        static void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend);

        static glm::mat3x3 calculateXRotationMatrix(double xRotation);
        static glm::mat3x3 calculateYRotationMatrix(double yRotation);

        static glm::mat4x4 calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar);

    private:
        void renderSetup();

        unsigned int compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath);

        float vertices[18] = {
            -0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f
        }; 

        float aspectRatio = 1;

        void setUniforms(World* world, int programIndex);

        unsigned int VAO[3];
        unsigned int VBO[3];

        unsigned int shaderProgram[4];

        TextureFetcher textureFetcher;

        TextureArrayCreator textureArrayCreator;

        std::vector<RenderChunkBuffer> renderChunkBuffers = std::vector<RenderChunkBuffer>();
};
#endif