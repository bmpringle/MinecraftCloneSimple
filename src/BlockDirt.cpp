#include "BlockDirt.h"

BlockDirt::BlockDirt() {

}

std::string BlockDirt::getTextureName(SideEnum side) {
    return "dirt.png";
}

std::string BlockDirt::getName() {
    return "dirt";
}