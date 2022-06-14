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

void BlockPos::print() const {
    std::cout << "pos: " << x << " " << y << " " << z << std::endl;
}

std::string BlockPos::to_string() const {
    return std::string("pos: " + std::to_string(x) +" " + std::to_string(y) + " " + std::to_string(z));
}

BlockPos BlockPos::getAbove() {
    return BlockPos(x, y + 1, z);
}

BlockPos BlockPos::getBelow() {
    return BlockPos(x, y - 1, z);
}

BlockPos BlockPos::getLeft() {
    return BlockPos(x - 1, y, z);
}

BlockPos BlockPos::getRight() {
    return BlockPos(x + 1, y, z);
}

BlockPos BlockPos::getFront() {
    return BlockPos(x, y, z + 1);
}

BlockPos BlockPos::getBehind() {
    return BlockPos(x, y, z - 1);
}