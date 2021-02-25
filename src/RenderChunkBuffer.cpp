#include "RenderChunkBuffer.h"
#include "RenderInclude.h"

RenderChunkBuffer::RenderChunkBuffer(std::vector<float> _renderDataO, std::vector<float> _renderDataT, BlockPos _pos) {
    renderDataT = _renderDataT;
    renderDataO = _renderDataO;
    pos = _pos;
    chunkBufferSizeO = renderDataO.size();
    chunkBufferSizeT = renderDataT.size();

    glGenVertexArrays(1, &VAO_OPAQUE);
    glBindVertexArray(VAO_OPAQUE);

    glGenBuffers(1, &VBO_OPAQUE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_OPAQUE);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSizeO * sizeof(float), renderDataO.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);


    glGenVertexArrays(1, &VAO_TRANSPARENT);
    glBindVertexArray(VAO_TRANSPARENT);

    glGenBuffers(1, &VBO_TRANSPARENT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TRANSPARENT);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSizeT * sizeof(float), renderDataT.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

void RenderChunkBuffer::setRenderData(std::vector<float> newDataOpaque, std::vector<float> newDataTransparent) {
    renderDataO = newDataOpaque;
    renderDataT = newDataTransparent;

    chunkBufferSizeO = renderDataO.size();
    chunkBufferSizeT = renderDataT.size();

    glBindVertexArray(VAO_OPAQUE);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_OPAQUE);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSizeO * sizeof(float), renderDataO.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO_TRANSPARENT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TRANSPARENT);
    glBufferData(GL_ARRAY_BUFFER, chunkBufferSizeT * sizeof(float), renderDataT.data(), GL_DYNAMIC_DRAW);
}

unsigned int RenderChunkBuffer::getRenderDataBufferOpaque() {
    return VBO_OPAQUE;
}

unsigned int RenderChunkBuffer::getRenderDataBufferTransparent() {
    return VBO_TRANSPARENT;
}

int RenderChunkBuffer::getRenderDataSizeOpaque() {
    return chunkBufferSizeO;
}

int RenderChunkBuffer::getRenderDataSizeTransparent() {
    return chunkBufferSizeT;
}

BlockPos RenderChunkBuffer::getPos() {
    return pos;
}

void RenderChunkBuffer::renderChunkOpaque() {
    glBindVertexArray(VAO_OPAQUE);
    glBindBuffer(GL_ARRAY_BUFFER, getRenderDataBufferOpaque());
    glDrawArrays(GL_TRIANGLES, 0, getRenderDataSizeOpaque() / 12);
}

void RenderChunkBuffer::renderChunkTransparent() {
    glBindVertexArray(VAO_TRANSPARENT);
    glBindBuffer(GL_ARRAY_BUFFER, getRenderDataBufferTransparent());
    glDrawArrays(GL_TRIANGLES, 0, getRenderDataSizeTransparent() / 12);
}

RenderChunkBuffer::RenderChunkBuffer() {
    
}