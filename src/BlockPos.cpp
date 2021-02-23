#include "BlockPos.h"
#include <iostream>
#include <vector>
#include <tuple>

BlockPos::BlockPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {

}

BlockPos BlockPos::operator+(BlockPos rhs) {
    BlockPos res = BlockPos(x + rhs.x, y + rhs.y, z + rhs.z);
    return res;
}

bool BlockPos::operator==(BlockPos rhs) {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

bool BlockPos::operator!=(BlockPos rhs) {
    return !((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
}

//todo: FIX FOR MAP
bool operator<(BlockPos lhs, BlockPos rhs) {
    return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
}

void BlockPos::print() {
    std::cout << "pos: " << x << " " << y << " " << z << std::endl;
}