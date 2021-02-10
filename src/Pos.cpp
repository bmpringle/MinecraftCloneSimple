#include "Pos.h"

Pos::Pos(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

}

Pos Pos::operator+(Pos rhs) {
    return Pos(x + rhs.x, y + rhs.y, z + rhs.z);
}

BlockPos Pos::toBlockPos() {
    return BlockPos((x >= 0) ? (int)x : (int)(x-1), (y >= 0) ? (int)y : (int)(y-1), (z >= 0) ? (int)z : (int)(z-1));
}