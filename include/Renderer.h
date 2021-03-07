#ifndef RENDERER_H
#define RENDERER_H

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
#include "FontLoader.h"

class World;

class Renderer {
    public:
        Renderer();

        void renderFrame(World* world);
        void renderOverlay(float rectangle[48], std::string texture);
        void renderOverlay(float rectangle[48], unsigned int TBO);
        void renderRectangle(float rectangle[36]);
        void renderBlockInWireframe(World* world, BlockPos pos);
        void updateAspectRatio(GLFWwindow* window);
        void updateWorldVBO(World* world);

        unsigned int textTextureBuffer(std::string text);

        template<class T>
        static void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend);

        static glm::mat3x3 calculateXRotationMatrix(double xRotation);
        static glm::mat3x3 calculateYRotationMatrix(double yRotation);

        static glm::mat4x4 calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar);

        int getWidth();

        int getHeight();

        std::array<int, 2> overlayDimensions();

        int getBitmapHeight();

        int getBitmapWidth();

    private:
        void renderSetup();

        void updateChunkData(std::vector<float>* buffer, std::vector<BlockData>* blocksInChunk, TextureArrayCreator* texCreator);

        unsigned int compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath);

        float vertices[18] = {
            -0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f
        }; 

        float aspectRatio = 1;

        void setUniforms(World* world, int programIndex);

        unsigned int VAO[4];
        unsigned int VBO[4];

        unsigned int shaderProgram[5];

        TextureFetcher textureFetcher;

        TextureArrayCreator textureArrayCreator;

        FontLoader fontLoader;

        std::map<BlockPos, RenderChunkBuffer> renderChunkBuffers = std::map<BlockPos, RenderChunkBuffer>();

        std::vector<RenderChunkBuffer> renderChunkBuffersOld = std::vector<RenderChunkBuffer>();

        int width = 0;
        int height = 0;
};
#endif