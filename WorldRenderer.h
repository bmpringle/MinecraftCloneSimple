#ifndef WORLDRENDERER_H
#define WORLDRENDERER_H

#include "RenderInclude.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include "BlockArrayData.h"

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

        unsigned int VAO;
        unsigned int shaderProgram;
};
#endif