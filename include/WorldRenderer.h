#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "BlockArrayData.h"
#include "simd/simd.h"
#include "TextureFetcher.h"

class World;

class WorldRenderer {
    public:
        WorldRenderer();

        void renderFrame(World* world);

    private:
        void renderSetup();

        unsigned int compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath);

        template<class T>
        void appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend);

        float vertices[18] = {
            -0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, /*color*/ 1.0f, 1.0f, 0.0f
        }; 

        matrix_float4x4 calculatePerspectiveMatrix(double FOV, double zNear, double zFar);

        matrix_float3x3 calculateXRotationMatrix(double xRotation);
        matrix_float3x3 calculateYRotationMatrix(double yRotation);

        void setUniforms(World* world, int programIndex);

        unsigned int VAO;
        unsigned int shaderProgram[2];

        TextureFetcher textureFetcher;
};
#endif