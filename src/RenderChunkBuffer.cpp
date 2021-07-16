#include "RenderChunkBuffer.h"
#include "RenderInclude.h"

RenderChunkBuffer::RenderChunkBuffer(std::map<std::string, std::vector<int>> _renderData, BlockPos _pos, ModelRegister* modelRegister) {
    pos = _pos;

    for(std::pair<const std::string, std::vector<int>>& pair : _renderData) {
        std::tuple<unsigned int, unsigned int> vboAndVao = modelRegister->getVAOAndVBO(pair.first);

        glBindVertexArray(std::get<0>(vboAndVao));
        unsigned int vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * pair.second.size(), pair.second.data(), GL_DYNAMIC_DRAW);

        renderData.try_emplace(pair.first, vbo);

        numberOfInstances.try_emplace(pair.first, pair.second.size() / 3);
    }
}

void RenderChunkBuffer::setRenderData(std::map<std::string, std::vector<int>> newData, ModelRegister* modelRegister) {
   renderData.clear();
   numberOfInstances.clear();
   
   for(std::pair<const std::string, std::vector<int>>& pair : newData) {
        std::tuple<unsigned int, unsigned int> vboAndVao = modelRegister->getVAOAndVBO(pair.first);

        glBindVertexArray(std::get<0>(vboAndVao));
        unsigned int vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * pair.second.size(), pair.second.data(), GL_DYNAMIC_DRAW);

        renderData.try_emplace(pair.first, vbo);

        numberOfInstances.try_emplace(pair.first, pair.second.size() / 3);
    }
}

BlockPos RenderChunkBuffer::getPos() {
    return pos;
}

void RenderChunkBuffer::renderChunk(ModelRegister* modelRegister) {
    for(std::pair<const std::string, unsigned int>& pair : renderData) {
        std::tuple<unsigned int, unsigned int> vboAndVao = modelRegister->getVAOAndVBO(pair.first);

        glBindVertexArray(std::get<0>(vboAndVao));

        glBindBuffer(GL_ARRAY_BUFFER, pair.second);
        glVertexAttribPointer(2, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glDrawArraysInstanced(GL_TRIANGLES, 0, modelRegister->getBufferLength(pair.first) / 6, numberOfInstances.at(pair.first)); 

        glBindVertexArray(0);

    }
}

RenderChunkBuffer::~RenderChunkBuffer() {

}
