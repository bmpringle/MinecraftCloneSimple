#include "Model.h"
#include "Player.h"
#include "Block.h"

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