#ifndef VULKANRENDERER_H

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
#include "Frustrum.h"
#include "ModelRegister.h"

class World;

class VulkanRenderer {
    public:
        VulkanRenderer();

        void renderFrame(World* world);
        void setOverlayData(std::string overlayID, float rectangle[48], std::string texture);
        void setOverlayData(std::string overlayID, float rectangle[36]);
        void removeOverlayData(std::string overlayID);
        void renderBlockInWireframe(World* world, BlockPos pos);
        void updateAspectRatio(GLFWwindow* window);
        void updateWorldVBO(World* world);

        void textTextureBuffer(std::string id, std::string text);

        int getWidth();

        int getHeight();

        template<class T>
        static void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T>& vectorToAppend);

        static glm::mat3x3 calculateXRotationMatrix(double xRotation);
        static glm::mat3x3 calculateYRotationMatrix(double yRotation);

        static glm::mat4x4 calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar);

        std::pair<unsigned int, unsigned int> getTextureDimensions(std::string id);

        std::array<int, 2> overlayDimensions();

        void setupEntityRenderer();

        void cleanup();

        GLFWwindow* getWindowPtr();

    private:
        VKRenderer renderer = VKRenderer();
        //this is subject to change, since it's from the OpenGlRenderer class

        void renderSetup();
        void renderEntity(std::shared_ptr<Entity> entity, World* world);

        void updateChunkData(std::map<std::string, std::vector<int>>* blockTypeToPositionsMap, std::vector<BlockData>* blocksInChunk, TextureArrayCreator* texCreator);

        unsigned int compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath);

        void renderOverlay(float rectangle[48], std::string texture);

        void renderRectangle(float rectangle[36]);

        float vertices[18] = {
            -0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f
        }; 

        float aspectRatio = 1;

        void setUniforms(World* world, int programIndex);

        void setUniforms(World* world, int programIndex, Frustrum* frustrum);

        unsigned int VAO[4];
        unsigned int VBO[4];

        unsigned int shaderProgram[7];

        ModelRegister modelRegister;

        std::map<BlockPos, RenderChunkBuffer> renderChunkBuffers = std::map<BlockPos, RenderChunkBuffer>();

        std::vector<RenderChunkBuffer> renderChunkBuffersOld = std::vector<RenderChunkBuffer>();

        int width = 0;
        int height = 0;

        //map::first is the overlayID, map::second is -> pair::first is the rectangle data, pair::second is the texture id
        std::map<std::string, std::pair<std::array<float, 48>, std::string>> overlayIDToData = std::map<std::string, std::pair<std::array<float, 48>, std::string>>();

        //map::first is the overlayID, map::second is the rectangle data
        std::map<std::string, std::array<float, 36>> rectangleOverlayIDToData = std::map<std::string, std::array<float, 36>>();
};

#endif