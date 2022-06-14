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

void AABB::rotateX90() {
    glm::vec3 p1 = glm::vec3(startX, startY, startZ);
    glm::vec3 p2 = glm::vec3(startX + xSize, startY, startZ);
    glm::vec3 p3 = glm::vec3(startX, startY + ySize, startZ);
    glm::vec3 p4 = glm::vec3(startX, startY, startZ + zSize);
    glm::vec3 p5 = glm::vec3(startX + xSize, startY + ySize, startZ);
    glm::vec3 p6 = glm::vec3(startX, startY + ySize, startZ + zSize);
    glm::vec3 p7 = glm::vec3(startX + xSize, startY, startZ + zSize);
    glm::vec3 p8 = glm::vec3(startX + xSize, startY + ySize, startZ + zSize);

    p1 = glm::vec3(p1.x, -p1.z, p1.y);
    p2 = glm::vec3(p2.x, -p2.z, p2.y);
    p3 = glm::vec3(p3.x, -p3.z, p3.y);
    p4 = glm::vec3(p4.x, -p4.z, p4.y);
    p5 = glm::vec3(p5.x, -p5.z, p5.y);
    p6 = glm::vec3(p6.x, -p6.z, p6.y);
    p7 = glm::vec3(p7.x, -p7.z, p7.y);
    p8 = glm::vec3(p8.x, -p8.z, p8.y);

    glm::vec3 minp = min(min(min(p1, p2), min(p3, p4)), min(min(p5, p6), min(p7, p8)));
    glm::vec3 maxp = max(max(max(p1, p2), max(p3, p4)), max(max(p5, p6), max(p7, p8)));

    startX = minp.x;
    startY = minp.y;
    startZ = minp.z;
    xSize = maxp.x - minp.x;
    ySize = maxp.y - minp.y;
    zSize = maxp.z - minp.z;
}

void AABB::rotateY90() {
    glm::mat4x4 rotation = glm::mat4x4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::vec4 p1h = rotation * glm::vec4(startX, startY, startZ, 0);
    glm::vec4 p2h = rotation * glm::vec4(startX + xSize, startY, startZ, 0);
    glm::vec4 p3h = rotation * glm::vec4(startX, startY + ySize, startZ, 0);
    glm::vec4 p4h = rotation * glm::vec4(startX, startY, startZ + zSize, 0);
    glm::vec4 p5h = rotation * glm::vec4(startX + xSize, startY + ySize, startZ, 0);
    glm::vec4 p6h = rotation * glm::vec4(startX, startY + ySize, startZ + zSize, 0);
    glm::vec4 p7h = rotation * glm::vec4(startX + xSize, startY, startZ + zSize, 0);
    glm::vec4 p8h = rotation * glm::vec4(startX + xSize, startY + ySize, startZ + zSize, 0);

    glm::vec3 p1 = glm::vec3(p1h.x, p1h.y, p1h.z);
    glm::vec3 p2 = glm::vec3(p2h.x, p2h.y, p2h.z);
    glm::vec3 p3 = glm::vec3(p3h.x, p3h.y, p3h.z);
    glm::vec3 p4 = glm::vec3(p4h.x, p4h.y, p4h.z);
    glm::vec3 p5 = glm::vec3(p5h.x, p5h.y, p5h.z);
    glm::vec3 p6 = glm::vec3(p6h.x, p6h.y, p6h.z);
    glm::vec3 p7 = glm::vec3(p7h.x, p7h.y, p7h.z);
    glm::vec3 p8 = glm::vec3(p8h.x, p8h.y, p8h.z);

    glm::vec3 minp = min(min(min(p1, p2), min(p3, p4)), min(min(p5, p6), min(p7, p8)));
    glm::vec3 maxp = max(max(max(p1, p2), max(p3, p4)), max(max(p5, p6), max(p7, p8)));
    
    startX = minp.x;
    startY = minp.y;
    startZ = minp.z;
    xSize = maxp.x - minp.x;
    ySize = maxp.y - minp.y;
    zSize = maxp.z - minp.z;
}

void AABB::rotateZ90() {
    glm::vec3 p1 = glm::vec3(startX, startY, startZ);
    glm::vec3 p2 = glm::vec3(startX + xSize, startY, startZ);
    glm::vec3 p3 = glm::vec3(startX, startY + ySize, startZ);
    glm::vec3 p4 = glm::vec3(startX, startY, startZ + zSize);
    glm::vec3 p5 = glm::vec3(startX + xSize, startY + ySize, startZ);
    glm::vec3 p6 = glm::vec3(startX, startY + ySize, startZ + zSize);
    glm::vec3 p7 = glm::vec3(startX + xSize, startY, startZ + zSize);
    glm::vec3 p8 = glm::vec3(startX + xSize, startY + ySize, startZ + zSize);

    p1 = glm::vec3(-p1.y, p1.x, p1.z);
    p2 = glm::vec3(-p2.y, p2.x, p2.z);
    p3 = glm::vec3(-p3.y, p3.x, p3.z);
    p4 = glm::vec3(-p4.y, p4.x, p4.z);
    p5 = glm::vec3(-p5.y, p5.x, p5.z);
    p6 = glm::vec3(-p6.y, p6.x, p6.z);
    p7 = glm::vec3(-p7.y, p7.x, p7.z);
    p8 = glm::vec3(-p8.y, p8.x, p8.z);

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
    WireframeVertex v1 = {{0, 0, 0}};
    WireframeVertex v2 = {{-xSize, 0, 0}};
    WireframeVertex v3 = {{0, ySize, 0}};
    WireframeVertex v4 = {{0, 0, zSize}};
    WireframeVertex v5 = {{-xSize, ySize, 0}};
    WireframeVertex v6 = {{0, ySize, zSize}};
    WireframeVertex v7 = {{-xSize, 0, zSize}};
    WireframeVertex v8 = {{-xSize, ySize, zSize}};

    std::vector<WireframeVertex> vertices;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v7);
    vertices.push_back(v4);
    vertices.push_back(v1);

    vertices.push_back(v3);
    vertices.push_back(v5);
    vertices.push_back(v2);
    vertices.push_back(v5);

    vertices.push_back(v8);
    vertices.push_back(v7);
    vertices.push_back(v8);

    vertices.push_back(v6);
    vertices.push_back(v4);
    vertices.push_back(v6);

    vertices.push_back(v3);

    return vertices;
}