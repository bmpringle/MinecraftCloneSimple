#include "ModelRegister.h"

#include <iostream>
#include "Renderer.h"

ModelRegister::ModelRegister() : vertexBuffer(std::vector<float>()),  idIndicesMap(std::map<int, std::vector<int>>()), nextID(0) {

};

bool ModelRegister::hasID(int id) {
    return idIndicesMap.count(id) == 1;
}

int ModelRegister::registerModel(std::vector<float>& vertexBufferIn, std::vector<int>& indices) {
    //Renderer::appendVectorWithVector(&vertexBuffer, vertexBufferIn);
    idIndicesMap[nextID] = indices;
    ++nextID;
    return nextID - 1;
}

std::vector<float>& ModelRegister::getVertexBuffer() {
    return vertexBuffer;
}

std::vector<int>& ModelRegister::getIndices(int id) {
    if(hasID(id)) {
        return idIndicesMap.at(id);
    }else {
        std::cout << "Tried to get indicies for a model that doesn't exist!" << std::endl;
        abort();
    }
}