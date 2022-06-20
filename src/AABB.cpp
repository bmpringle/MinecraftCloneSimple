#include "AABB.h"
#include "BlockPos.h"
#include "Pos.h"

#include <iostream>

void AABB::add(Pos p) {
    startX += p.x;
    startY += p.y;
    startZ += p.z;
}

void AABB::add(BlockPos p) {
    startX += p.x;
    startY += p.y;
    startZ += p.z;
}

float AABB::getWidth() {
    return xSize;
}

float AABB::getHeight() {
    return ySize;
}

float AABB::getDepth() {
    return zSize;
}

glm::vec3 min(glm::vec3 a, glm::vec3 b) {
    if(a.x < b.x) {
        return a;
    }else if(a.x == b.x) {
        if(a.y < b.y) {
            return a;
        }else if(a.y == b.y) {
            if(a.z < b.z) {
                return a;
            }else if(a.z == b.z) {
                return a;
            }else {
                return b;
            }
        }else {
            return b;
        }
    }else {
        return b;
    }
}

glm::vec3 max(glm::vec3 a, glm::vec3 b) {
    glm::vec3 minimum = min(a, b);
    if(minimum.x == a.x && minimum.y == a.y && minimum.z == a.z) {
        return b;
    }else {
        return a;
    }
}

void AABB::rotateX90(float x, float y, float z) {
    glm::vec3 p1 = glm::vec3(startX - x, startY - y, startZ - z);
    glm::vec3 p2 = glm::vec3(startX - x + xSize, startY - y, startZ) - z;
    glm::vec3 p3 = glm::vec3(startX - x, startY - y + ySize, startZ - z);
    glm::vec3 p4 = glm::vec3(startX - x, startY - y, startZ - z + zSize);
    glm::vec3 p5 = glm::vec3(startX - x + xSize, startY - y + ySize, startZ - z);
    glm::vec3 p6 = glm::vec3(startX - x, startY - y + ySize, startZ - z + zSize);
    glm::vec3 p7 = glm::vec3(startX - x + xSize, startY - y, startZ - z + zSize);
    glm::vec3 p8 = glm::vec3(startX - x + xSize, startY - y + ySize, startZ - z + zSize);

    p1 = glm::vec3(p1.x + x, -p1.z + y, p1.y + z);
    p2 = glm::vec3(p2.x + x, -p2.z + y, p2.y + z);
    p3 = glm::vec3(p3.x + x, -p3.z + y, p3.y + z);
    p4 = glm::vec3(p4.x + x, -p4.z + y, p4.y + z);
    p5 = glm::vec3(p5.x + x, -p5.z + y, p5.y + z);
    p6 = glm::vec3(p6.x + x, -p6.z + y, p6.y + z);
    p7 = glm::vec3(p7.x + x, -p7.z + y, p7.y + z);
    p8 = glm::vec3(p8.x + x, -p8.z + y, p8.y + z);

    glm::vec3 minp = min(min(min(p1, p2), min(p3, p4)), min(min(p5, p6), min(p7, p8)));
    glm::vec3 maxp = max(max(max(p1, p2), max(p3, p4)), max(max(p5, p6), max(p7, p8)));

    startX = minp.x;
    startY = minp.y;
    startZ = minp.z;
    xSize = maxp.x - minp.x;
    ySize = maxp.y - minp.y;
    zSize = maxp.z - minp.z;
}

void AABB::rotateY90(float x, float y, float z) {
    glm::mat4x4 rotation = glm::mat4x4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::vec4 p1h = rotation * glm::vec4(startX - x, startY - y, startZ - z, 0);
    glm::vec4 p2h = rotation * glm::vec4(startX - x + xSize, startY - y, startZ - z, 0);
    glm::vec4 p3h = rotation * glm::vec4(startX - x, startY - y + ySize, startZ - z, 0);
    glm::vec4 p4h = rotation * glm::vec4(startX - x, startY - y, startZ - z + zSize, 0);
    glm::vec4 p5h = rotation * glm::vec4(startX - x + xSize, startY - y + ySize, startZ - z, 0);
    glm::vec4 p6h = rotation * glm::vec4(startX - x, startY - y + ySize, startZ - z + zSize, 0);
    glm::vec4 p7h = rotation * glm::vec4(startX - x + xSize, startY - y, startZ - z + zSize, 0);
    glm::vec4 p8h = rotation * glm::vec4(startX - x + xSize, startY - y + ySize, startZ - z + zSize, 0);

    glm::vec3 p1 = glm::vec3(p1h.x + x, p1h.y + y, p1h.z + z);
    glm::vec3 p2 = glm::vec3(p2h.x + x, p2h.y + y, p2h.z + z);
    glm::vec3 p3 = glm::vec3(p3h.x + x, p3h.y + y, p3h.z + z);
    glm::vec3 p4 = glm::vec3(p4h.x + x, p4h.y + y, p4h.z + z);
    glm::vec3 p5 = glm::vec3(p5h.x + x, p5h.y + y, p5h.z + z);
    glm::vec3 p6 = glm::vec3(p6h.x + x, p6h.y + y, p6h.z + z);
    glm::vec3 p7 = glm::vec3(p7h.x + x, p7h.y + y, p7h.z + z);
    glm::vec3 p8 = glm::vec3(p8h.x + x, p8h.y + y, p8h.z + z);

    glm::vec3 minp = min(min(min(p1, p2), min(p3, p4)), min(min(p5, p6), min(p7, p8)));
    glm::vec3 maxp = max(max(max(p1, p2), max(p3, p4)), max(max(p5, p6), max(p7, p8)));
    
    startX = minp.x;
    startY = minp.y;
    startZ = minp.z;
    xSize = maxp.x - minp.x;
    ySize = maxp.y - minp.y;
    zSize = maxp.z - minp.z;
}

void AABB::rotateZ90(float x, float y, float z) {
    glm::vec3 p1 = glm::vec3(startX - x, startY - y, startZ - z);
    glm::vec3 p2 = glm::vec3(startX - x + xSize, startY - y, startZ - z);
    glm::vec3 p3 = glm::vec3(startX - x, startY - y + ySize, startZ - z);
    glm::vec3 p4 = glm::vec3(startX - x, startY - y, startZ - z + zSize);
    glm::vec3 p5 = glm::vec3(startX - x + xSize, startY - y + ySize, startZ - z);
    glm::vec3 p6 = glm::vec3(startX - x, startY - y + ySize, startZ - z + zSize);
    glm::vec3 p7 = glm::vec3(startX - x + xSize, startY - y, startZ - z + zSize);
    glm::vec3 p8 = glm::vec3(startX - x + xSize, startY - y + ySize, startZ - z + zSize);

    p1 = glm::vec3(-p1.y + x, p1.x + y, p1.z + z);
    p2 = glm::vec3(-p2.y + x, p2.x + y, p2.z + z);
    p3 = glm::vec3(-p3.y + x, p3.x + y, p3.z + z);
    p4 = glm::vec3(-p4.y + x, p4.x + y, p4.z + z);
    p5 = glm::vec3(-p5.y + x, p5.x + y, p5.z + z);
    p6 = glm::vec3(-p6.y + x, p6.x + y, p6.z + z);
    p7 = glm::vec3(-p7.y + x, p7.x + y, p7.z + z);
    p8 = glm::vec3(-p8.y + x, p8.x + y, p8.z + z);

    glm::vec3 minp = min(min(min(p1, p2), min(p3, p4)), min(min(p5, p6), min(p7, p8)));
    glm::vec3 maxp = max(max(max(p1, p2), max(p3, p4)), max(max(p5, p6), max(p7, p8)));

    startX = minp.x;
    startY = minp.y;
    startZ = minp.z;
    xSize = maxp.x - minp.x;
    ySize = maxp.y - minp.y;
    zSize = maxp.z - minp.z;
}

std::vector<WireframeVertex> AABB::getWireframeFromAABB() {
    /*WireframeVertex v1 = {{startX, startY, startZ}};
    WireframeVertex v2 = {{startX + xSize, startY, startZ}};
    WireframeVertex v3 = {{startX, startY - ySize, startZ}};
    WireframeVertex v4 = {{startX, startY, startZ + zSize}};
    WireframeVertex v5 = {{startX - xSize, startY + ySize, startZ}};
    WireframeVertex v6 = {{startX, startY + ySize, startZ + zSize}};
    WireframeVertex v7 = {{startX - xSize, startY, startZ + zSize}};
    WireframeVertex v8 = {{startX - xSize, startY + ySize, startZ + zSize}};*/

    std::vector<WireframeVertex> vertices;
    vertices.push_back({{-startX, startY, startZ}});
    vertices.push_back({{-startX, startY + ySize, startZ}});
    vertices.push_back({{-startX - xSize, startY + ySize, startZ}});
    vertices.push_back({{-startX - xSize, startY, startZ}});
    vertices.push_back({{-startX, startY, startZ}});

    vertices.push_back({{-startX, startY, startZ + zSize}});
    vertices.push_back({{-startX, startY + ySize, startZ + zSize}});
    vertices.push_back({{-startX - xSize, startY + ySize, startZ + zSize}});
    vertices.push_back({{-startX - xSize, startY, startZ + zSize}});
    vertices.push_back({{-startX, startY, startZ + zSize}});

    vertices.push_back({{-startX - xSize, startY, startZ + zSize}});
    vertices.push_back({{-startX - xSize, startY, startZ}});

    vertices.push_back({{-startX - xSize, startY + ySize, startZ}});
    vertices.push_back({{-startX - xSize, startY + ySize, startZ + zSize}});

    vertices.push_back({{-startX, startY + ySize, startZ + zSize}});
    vertices.push_back({{-startX, startY + ySize, startZ}});

    return vertices;
}