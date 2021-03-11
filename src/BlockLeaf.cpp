#include "BlockLeaf.h"

BlockLeaf::BlockLeaf() {

}

std::string BlockLeaf::getTextureName(SideEnum side, int data = 0) {
    return "leaf.png";
}

std::string BlockLeaf::getName() {
    return "leaf";
}