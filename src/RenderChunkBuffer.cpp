#include "RenderChunkBuffer.h"
#include "RenderInclude.h"

RenderChunkBuffer::RenderChunkBuffer(std::vector<float> _renderData, BlockPos _pos) {
    renderData = _renderData;
    pos = _pos;
    chunkBufferSize = renderData.size();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSize * sizeof(float), renderData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

void RenderChunkBuffer::setRenderData(std::vector<float> newData) {
    renderData = newData;
    chunkBufferSize = renderData.size();

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSize * sizeof(float), renderData.data(), GL_DYNAMIC_DRAW);
}

unsigned int RenderChunkBuffer::getRenderDataBuffer() {
    return VBO;
}

int RenderChunkBuffer::getRenderDataSize() {
    return chunkBufferSize;
}

BlockPos RenderChunkBuffer::getPos() {
    return pos;
}

void RenderChunkBuffer::renderChunk() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, getRenderDataBuffer());
    glDrawArrays(GL_TRIANGLES, 0, getRenderDataSize() / 12);
}