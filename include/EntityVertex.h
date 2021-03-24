#ifndef ENTITYVERTEX_H
#define ENTITYVERTEX_H
#include "glmh.h"

#pragma pack(push, 0)

struct EntityVertex  {
    EntityVertex(glm::vec4 _pos, glm::vec2 _uv) : pos(_pos), uv(_uv) {

    }
    glm::vec4 pos;
    glm::vec2 uv;
};

#pragma pack(pop)
#endif