#include "BlockLeaf.h"

BlockLeaf::BlockLeaf() {

}

std::string BlockLeaf::getTextureName(SideEnum side) {
    return "leaf.png";
}

std::string BlockLeaf::getName() {
    return "leaf";
}