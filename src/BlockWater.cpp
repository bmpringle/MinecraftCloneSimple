#include "BlockWater.h"

BlockWater::BlockWater() {

}

std::string BlockWater::getTextureName(SideEnum side) {
    return "water.png";
}

std::string BlockWater::getName() {
    return "water";
}

bool BlockWater::isSolid() {
    return false;
}