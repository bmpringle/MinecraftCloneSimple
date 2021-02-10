#include "AABB.h"
#include "BlockPos.h"
#include "Pos.h"

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