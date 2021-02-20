#include "BlockCobblestone.h"

BlockCobblestone::BlockCobblestone() {

}

std::string BlockCobblestone::getTextureName(SideEnum side) {
    return "cobblestone.png";
}

std::string BlockCobblestone::getName() {
    return "cobblestone";
}