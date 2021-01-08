#include "BlockCobblestone.h"

BlockCobblestone::BlockCobblestone() {

}

std::string BlockCobblestone::getTextureName() {
    return "cobblestone.png";
}

std::string BlockCobblestone::getName() {
    return "cobblestone";
}

std::shared_ptr<Block> BlockCobblestone::create() {
    return std::shared_ptr<Block>(new BlockCobblestone());
}