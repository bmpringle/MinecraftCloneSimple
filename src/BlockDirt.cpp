#include "BlockDirt.h"

BlockDirt::BlockDirt() {

}

std::string BlockDirt::getTextureName() {
    return "dirt.png";
}

std::string BlockDirt::getName() {
    return "dirt";
}

std::shared_ptr<Block> BlockDirt::create() {
    return std::shared_ptr<Block>(new BlockDirt());
}