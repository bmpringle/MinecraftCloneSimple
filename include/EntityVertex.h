#ifndef ENTITYVERTEX_H
#define ENTITYVERTEX_H
#include "glmh.h"

#pragma pack(push, 0)

struct EntityVertex  {
    EntityVertex(glm::vec4 _pos, glm::vec2 _uv, glm::vec3 _normal) : pos(_pos), uv(_uv), normal(_normal) {

    }
    glm::vec4 pos;
    glm::vec2 uv;
    glm::vec3 normal;
};

#pragma pack(pop)
#endif