#include "BlockPlanks.h"

BlockPlanks::BlockPlanks() {

}

std::string BlockPlanks::getTextureName(SideEnum side, int data = 0) {
    return "planks.png";
}

std::string BlockPlanks::getName() {
    return "planks";
}