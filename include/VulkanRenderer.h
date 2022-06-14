#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "BlockArrayData.h"
#include "glmh.h"
#include "TextureFetcher.h"
#include "RenderInclude.h"
#include "FontLoader.h"
#include "Frustrum.h"

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

        VKRenderer& getInternalRenderer();

    private:
        VKRenderer renderer = VKRenderer();

        void initTexturesAndModels();

        std::vector<BlockPos> chunkPositionsCurrentlyLoaded;

        void updateChunkData(Chunk* chunk);

        bool shouldRenderWireframe = false;

        bool wireframeIsRendered = false;

        BlockPos wireframeBlockPos = BlockPos(0, 0, 0);

        std::string wireframeModelName = "";

        float aspectRatio = 1;
};

#endif