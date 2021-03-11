#include "BlockGrass.h"

BlockGrass::BlockGrass() {

}

std::string BlockGrass::getTextureName(SideEnum side, int data = 0){
    switch(side) {
        case UP:
            return "grass_top.png";
        case DOWN:
            return "dirt.png";
        default:
            return "grass_side.png";
    }
}

std::string BlockGrass::getName() {
    return "grass";
}