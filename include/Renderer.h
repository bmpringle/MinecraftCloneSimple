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
#include "Frustrum.h"
#include "ModelRegister.h"

#include "OpenGLRenderer.h"

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
        static void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T>& vectorToAppend);

        static glm::mat3x3 calculateXRotationMatrix(double xRotation);
        static glm::mat3x3 calculateYRotationMatrix(double yRotation);

        static glm::mat4x4 calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar);

        int getWidth();

        int getHeight();

        std::array<int, 2> overlayDimensions();

        int getBitmapHeight();

        int getBitmapWidth();

        void setupEntityRenderer();

        void cleanup();

    private:
        OpenGLRenderer openGLRenderer;
};
#endif
