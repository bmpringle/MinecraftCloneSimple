#include "BlockDirt.h"

BlockDirt::BlockDirt() {

}

std::string BlockDirt::getTextureName(SideEnum side, int data = 0) {
    return "dirt.png";
}

std::string BlockDirt::getName() {
    return "dirt";
}