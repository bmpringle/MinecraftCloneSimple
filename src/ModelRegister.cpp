#include "ModelRegister.h"

#include <iostream>
#include "Renderer.h"

ModelRegister::ModelRegister() {

};

#ifndef VULKAN_BACKEND

void ModelRegister::registerModel(TextureArrayCreator* texCreator, std::shared_ptr<Block> block, int metadata) {
    std::vector<float> buffer = std::vector<float>();

    BlockRenderedModel model = block->getRenderedModel(metadata);

    for(BlockFace face : model.renderedBlockModel) {
        int texID = texCreator->getTextureLayer(block->getTextureName(face.side, metadata));

        for(RenderedTriangle triangle : face.triangles) {
            buffer.push_back(triangle.a.x);
            buffer.push_back(triangle.a.y);
            buffer.push_back(triangle.a.z);
            buffer.push_back(triangle.a.u);
            buffer.push_back(triangle.a.v);
            buffer.push_back(texID);

            buffer.push_back(triangle.b.x);
            buffer.push_back(triangle.b.y);
            buffer.push_back(triangle.b.z);
            buffer.push_back(triangle.b.u);
            buffer.push_back(triangle.b.v);
            buffer.push_back(texID);

            buffer.push_back(triangle.c.x);
            buffer.push_back(triangle.c.y);
            buffer.push_back(triangle.c.z);
            buffer.push_back(triangle.c.u);
            buffer.push_back(triangle.c.v);
            buffer.push_back(texID);
        }
    }

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    blockToVAOAndVBOMap.try_emplace(block->getName() + "-" + std::to_string(metadata), std::tie(VAO, VBO));

    blockToBufferLengthMap.try_emplace(block->getName() + "-" + std::to_string(metadata), buffer.size());
};

std::tuple<unsigned int, unsigned int> ModelRegister::getVAOAndVBO(std::string key) {
    return blockToVAOAndVBOMap.at(key);
}

int ModelRegister::getBufferLength(std::string key) {
    return blockToBufferLengthMap.at(key);
}

#else
void ModelRegister::registerModel(TextureArrayCreator* texCreator, std::shared_ptr<Block> block, int metadata) {
    std::cout << "registerModel not defined for Vulkan Backend" << std::endl;
}

std::tuple<unsigned int, unsigned int> ModelRegister::getVAOAndVBO(std::string key) {
    std::cout << "getVAOAndVBO not defined for Vulkan Backend" << std::endl;
    return blockToVAOAndVBOMap.at(key);
}

int ModelRegister::getBufferLength(std::string key) {
    std::cout << "getBufferLength not defined for Vulkan Backend" << std::endl;
    return blockToBufferLengthMap.at(key);
}
#endif