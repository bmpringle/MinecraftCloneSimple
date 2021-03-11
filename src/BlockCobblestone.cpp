#include "BlockCobblestone.h"

BlockCobblestone::BlockCobblestone() {

}

std::string BlockCobblestone::getTextureName(SideEnum side, int data = 0) {
    return "cobblestone.png";
}

std::string BlockCobblestone::getName() {
    return "cobblestone";
}